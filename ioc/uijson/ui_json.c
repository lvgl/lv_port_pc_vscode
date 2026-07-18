#include "ioc_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include <dirent.h>
#ifndef _WIN32
#include <sys/select.h>
#endif
#include <sys/time.h>
#ifndef _WIN32
#include <sys/eventfd.h>
#endif
#include <errno.h>

ioc_ui_json_t g_ui_json;
static bool s_ui_json_changed = false;
static int s_point_temp = -50; // 环境温度最大值

static int s_ui_json_sockfd = -1;
char g_ui_json_buffer[UI_JSON_BUFFER_SIZE];
static char s_devMasterIp[INET_ADDRSTRLEN] = {0};

int g_ui_json_eventfd = -1;

void ioc_send_udp(char * pt, int length)
{
    if (s_ui_json_sockfd > 0)
    {
        struct sockaddr_in ser_addr;  //
        memset(&ser_addr, 0, sizeof(ser_addr));
        ser_addr.sin_family = AF_INET;

#if (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202)
        ser_addr.sin_addr.s_addr = inet_addr("192.168.10.1");      // 注意网络序转换
#elif (COMPILER_FOR == COMPILE_FOR_AC_SSD202)
        if (strlen(s_devMasterIp) == 0)
        {
            LV_LOG_WARN("No valid target IP, cannot send UDP packet");
            return;
        }
        ser_addr.sin_addr.s_addr = inet_addr(s_devMasterIp);
#else
        ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");         // 注意网络序转换
#endif

        ser_addr.sin_port = htons(SENDUI_AM3354_SRC_PORT);     // 发往3354的端口号, 注意网络序转换

        socklen_t len = sizeof(struct sockaddr);
        size_t sent = sendto(s_ui_json_sockfd, pt, length, 0, (struct sockaddr *)&ser_addr, len);
        if (sent != length)
        {
            LV_LOG_ERROR("sendto failed: sent=%ld expected=%d, errno=%d (%s)", sent, length, errno, strerror(errno));
        }
    }
    else
    {
        LV_LOG_ERROR("Socket Error");
    }

    return;
}

void ioc_send_ConnSelected(int idx)
{
    char buf[128] = {0};

#if ((COMPILER_FOR == COMPILE_FOR_CABINET_AM3354) || (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202))
    snprintf(buf, sizeof(buf), "{\""UIMSG_ACTIVE_IDX"\":%d}",  g_ui_json.dispenserConfig.connIdx[idx]);
#else
    snprintf(buf, sizeof(buf), "{\""UIMSG_ACTIVE_IDX"\":%d}",  idx);
#endif

    LV_LOG_WARN("activeIdx SEND BUFF[%s]", buf);

    // ioc_mqtt_publish(idx, PUBLISH_UI, buf);
    ioc_send_udp(buf, strlen(buf));
}

void ioc_send_UseCreditCard(int idx, int PerAuth)
{
    char buf[128] = {0};

#if ((COMPILER_FOR == COMPILE_FOR_CABINET_AM3354) || (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202))
    // sprintf(connSelect, "{\""UIMSG_USE_CREDITCARD"\":%d}", idx);
    snprintf(buf, sizeof(buf), "{\""UIMSG_USE_CREDITCARD"\":%d,\""PREAUTH_AMOUNT"\":%d}", g_ui_json.dispenserConfig.connIdx[idx], PerAuth);
    // ioc_mqtt_publish(idx, PUBLISH_UI, buf);
#else
    snprintf(buf, sizeof(buf), "{\""UIMSG_USE_CREDITCARD"\":%d,\""PREAUTH_AMOUNT"\":%d}", idx, PerAuth);
#endif

    LV_LOG_WARN("UseCreditCard SEND BUFF[%s]", buf);
    ioc_send_udp(buf, strlen(buf));
}

void ioc_send_StopPlugCharging(int idx)
{
    char buf[128] = {0};

#if ((COMPILER_FOR == COMPILE_FOR_CABINET_AM3354) || (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202))
    snprintf(buf, sizeof(buf), "{\""STOP_PLUG_CHARGING"\":%d}", g_ui_json.dispenserConfig.connIdx[idx]);
#else
    snprintf(buf, sizeof(buf), "{\""STOP_PLUG_CHARGING"\":%d}", idx);
#endif

    LV_LOG_WARN("StopPlugCharging SEND BUFF[%s]", buf);
    ioc_send_udp(buf, strlen(buf));
}

void ioc_send_StartCharging(int idx)
{
    char buf[128] = {0};

#if ((COMPILER_FOR == COMPILE_FOR_CABINET_AM3354) || (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202))
    snprintf(buf, sizeof(buf), "{\""START_CHARGING"\":%d}", g_ui_json.dispenserConfig.connIdx[idx]);
#else
    snprintf(buf, sizeof(buf), "{\""START_CHARGING"\":%d}", idx);
#endif

    LV_LOG_WARN("StartPlugCharging SEND BUFF[%s]", buf);

    ioc_send_udp(buf, strlen(buf));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const UI_CurrencyMap_t c_CurrencyMaps[] =
{
    {CURRENCY_USD, "USD", "$"},
    {CURRENCY_GBP, "GBP", "£"},
    {CURRENCY_EUR, "EUR", "€"},
    {CURRENCY_RUB, "RUB", "₽"},
    {CURRENCY_CNY, "CNY", "¥"},
    {CURRENCY_SEK, "SEK", "Kr"},
    {CURRENCY_PLN, "PLN", "zł"},
};

// 软件开关功能
typedef struct
{
    bool lcdIdleOnRF;
    bool lcdIdleShowWelcome;

}SwFunc_t;

static SwFunc_t sw_Func = {0};

#define FUNC_LCD_IDLE_ONRF               sw_Func.lcdIdleOnRF                 // 空闲状态下是否显示刷卡 0显示请插枪
#define FUNC_LCD_IDLE_SHOW_WELCOME       sw_Func.lcdIdleShowWelcome          // 未激活返回welcome界面

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void SwFuncCheck(uint32_t func)
{
    if(func & (1 << 0))
    {
        FUNC_LCD_IDLE_ONRF = true;
    }

    if(func & (1 << 13))
    {
        FUNC_LCD_IDLE_SHOW_WELCOME = true;
    }
}

void Check_1768(uint8_t idx)
{
    Starting_SubStep_t sub;
    uint8_t StateMaxNum, state, type;
    char **pCharSta = NULL;

    Charger_CONNType_t ConnType = g_ui_json.connectors[idx].conn_type;
    const char *state1768 = g_ui_json.connectors[idx].m1768Info_runState;
    uint8_t sub1768 = g_ui_json.connectors[idx].subState;

    switch (ConnType)
    {
    case HWCFG_CONN_TYPE_CHADEMO_DC:
        StateMaxNum = M1768_STATE_CHADEMO_NUM;
        pCharSta = (char**)&c_stateName_CHAdeMO;
        type = 0;
        break;
    case HWCFG_CONN_TYPE_GBT_DC:
        StateMaxNum = M1768_STATE_GBT_NUM;
        pCharSta = (char**)&c_stateName_GBT;
        type = 1;
        break;
    case HWCFG_CONN_TYPE_SAE_DC:
    case HWCFG_CONN_TYPE_IEC_DC:
    case HWCFG_CONN_TYPE_NACS_DC:
        StateMaxNum = M1768_STATE_GBT_NUM;
        pCharSta = (char**)&c_stateName_CCS;
        type = 2;
        break;
    default:
        LV_LOG_WARN("Invalid connection type[%d]", ConnType);
        return;
        break;
    }

    for (state = 0; state < StateMaxNum; state++)
    {
        if (strcmp(pCharSta[state], state1768) == 0)
        {
            break;
        }
    }

    if (type == 0)      // chademo
    {
        if (state == M1768_STATE_CHADEMO_START)
        {

            sub = STARTING_HANDSHAKE;
        }
        else if (state == M1768_STATE_CHADEMO_INFORMATION_EXCHANGE)
        {
            if (sub1768 <= 2)
            {
                sub = STARTING_HANDSHAKE;
            }
            else
            {
                sub = STARTING_PARAMTETER;
            }
        }
        else if (state == M1768_STATE_CHADEMO_LOCK)
        {
            sub = STARTING_INSULATION;
        }
        else if (state == M1768_STATE_CHADEMO_PREPARING)
        {
            sub = STARTING_PRECHARGE;
        }
    }
    else if (type == 1)     // GBT
    {
        if (state == M1768_STATE_GBT_START)
        {
            sub = STARTING_HANDSHAKE;
        }
        else if (state == M1768_STATE_GBT_SELF_CHECK)
        {
            sub = STARTING_HANDSHAKE;
        }
        else if (state == M1768_STATE_GBT_HANDSHAKE)
        {
            switch (sub1768)
            {
	        case GBT_SHAKEHANDS_INSU:       sub = STARTING_INSULATION; break;

	        case GBT_SHAKEHANDS_WAIT_BRM:
	        case GBT_SHAKEHANDS_WAIT_BCP:   sub = STARTING_PARAMTETER; break;

            case GBT_SHAKEHANDS_WAIT_BHM:
            default:                        sub = STARTING_HANDSHAKE;  break;
            }
        }
        else if (state == M1768_STATE_GBT_PRRAMETER)
        {
            switch (sub1768)
            {
	        case GBT_PRECHARGE_CHKBAT:
	        case GBT_PRECHARGE_WAIT_BCS:
	        case GBT_PRECHARGE_WAIT_K1K2:   sub = STARTING_PRECHARGE; break;

            case GBT_PRECHARGE_WAIT_BRO:
            default:                        sub = STARTING_PARAMTETER;  break;
            }
        }
    }
    else if (type == 2)     // RNL
    {
        if (state == M1768_STATE_GBT_START)
        {
            sub = STARTING_HANDSHAKE;
        }
        else if (state == M1768_STATE_GBT_SELF_CHECK)
        {
            sub = STARTING_HANDSHAKE;
        }
        else if (state == M1768_STATE_GBT_HANDSHAKE)
        {
            switch (sub1768)
            {
	        case RNL_SHAKEHANDS_INSU:       sub = STARTING_INSULATION; break;

	        case RNL_SHAKEHANDS_WAIT_BRM:
	        case RNL_SHAKEHANDS_WAIT_BCP:   sub = STARTING_PARAMTETER; break;

            case RNL_SHAKEHANDS_WAIT_BHM:
            default:                        sub = STARTING_HANDSHAKE;  break;
            }
        }
        else if (state == M1768_STATE_GBT_PRRAMETER)
        {
            switch (sub1768)
            {
	        case RNL_PRECHARGE_WAIT_BRO:   sub = STARTING_PARAMTETER; break;

	        case RNL_PRECHARGE_INSU:       sub = STARTING_INSULATION; break;

	        case RNL_PRECHARGE_WAIT_BCL:
            case RNL_PRECHARGE_WAIT_K1K2:
            case RNL_PRECHARGE_WAIT_BCS:
            default:                        sub = STARTING_PRECHARGE;  break;
            }
        }
        else    // AC
        {
            sub = STARTING_AC;
        }
    }

    g_ui_json.connectors[idx].startingSubStep = sub;
}


ioc_ui_connector_state_t ui_page_status_judgment(uint8_t gun_idx)
{
    // LV_LOG_WARN("idx[%d]  _status_real[%s]", gun_idx, g_ui_json.connectors[gun_idx]._status_real);

    // 测试用
    return UI_CONNECTOR_STATE_STARTING2;

    // start error
    if(g_ui_json.connectors[gun_idx].conn_type == HWCFG_CONN_TYPE_SAE_DC)
    {
        uint16_t m1768errCode = g_ui_json.connectors[gun_idx].m1768errCode;
        if((m1768errCode >= 12 && m1768errCode <= 16) ||
        (m1768errCode >= 151 && m1768errCode <= 155))
        {
            return UI_CONNECTOR_STATE_START_ERROR;
        }
    }

    const char *status = g_ui_json.connectors[gun_idx]._status_real;
    const char *conn   = g_ui_json.connectors[gun_idx].connStatus;

    // 先判断 _status_real
    if ( strcmp(g_ui_json.connectors[gun_idx]._status_real, "Available") == 0 ||
         strcmp(g_ui_json.connectors[gun_idx]._status_real, "Preparing") == 0 )
    {

#if (COMPILER_FOR == COMPILE_FOR_AC_SSD202  || COMPILER_FOR == COMPILE_FOR_SSD202_43)  // 交流桩有空闲时显示刷卡界面功能按钮
    if(strcmp(g_ui_json.connectors[gun_idx]._status_real,"Available") == 0)
        {
            if(FUNC_LCD_IDLE_ONRF)
            {
                // 判断是否启用POS机
                if(UI_ENABLEPOS)
                {
                    return UI_CONNECTOR_STATE_STARTING2;
                }

                return UI_CONNECTOR_STATE_STARTING1;
            }

            return  UI_CONNECTOR_STATE_IDLE;
        }
#endif
        // 判断是否插枪
        if ( strcmp(g_ui_json.connectors[gun_idx].connStatus, "Unplug") == 0)
        {
            return UI_CONNECTOR_STATE_IDLE;
        }
        else
        {
        // TODO 未授权跳转到刷卡界面，12Connecting授权失败也可能跳到这个界面，要添加字段
        // if(strcmp(g_ui_json.connectors[gun_idx].idTag, "") == 0)

            // 判断是否启用POS机
            if(UI_ENABLEPOS)
            {
                return UI_CONNECTOR_STATE_STARTING2;
            }

            return UI_CONNECTOR_STATE_STARTING1;
        }
    }
    else if(strcmp(g_ui_json.connectors[gun_idx]._status_real, "SuspendedEVSE") == 0 ||
            strcmp(g_ui_json.connectors[gun_idx]._status_real, "SuspendedEV") == 0)
    {
        return UI_CONNECTOR_STATE_SUSPEND;

    }
    else if(strcmp(g_ui_json.connectors[gun_idx]._status_real, "Reserved") == 0)
    {
        return UI_CONNECTOR_STATE_RESERVED;
    }
    else if(strcmp(g_ui_json.connectors[gun_idx]._status_real, "Unavailable") == 0)
    {
        return UI_CONNECTOR_STATE_UNAVAILABLE;
    }
    else if(strcmp(g_ui_json.connectors[gun_idx]._status_real, "Faulted") == 0)
    {
        return UI_CONNECTOR_STATE_FAULT;
    }
    else if(strcmp(g_ui_json.connectors[gun_idx]._status_real, "Finishing") == 0)
    {
        return UI_CONNECTOR_STATE_FINISHED;
    }
    else if(strcmp(g_ui_json.connectors[gun_idx]._status_real, "Charging") == 0)
    {
        // PnC/Auto
        if( strcmp(g_ui_json.connectors[gun_idx].idTag, "CCSPLUGCHARGE") == 0 )
        {
            return UI_CONNECTOR_STATE_CONNECTING;
        }

        // Idle
        if((strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Idle") == 0)
        || (strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Carbin") == 0))
        {
            return UI_CONNECTOR_STATE_IDLE;
        }

        // 判断是否等待轮充
        if( strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "ReqRes") == 0)
        {
#if ( COMPILER_FOR == COMPILE_FOR_SSD202_43)  // 交流桩跳过WAIT_IN_LINE界面
            return UI_CONNECTOR_STATE_CHARGING;
#else
            return UI_CONNECTOR_STATE_WAIT_IN_LINE;
#endif
        }

        // Suspend
        if((strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "SuspendEVSE") == 0)
        || (strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "SuspendEV") == 0))
        {
            return UI_CONNECTOR_STATE_SUSPEND;
        }

        // Finishd
        if( strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Finished") == 0)
        {
            return UI_CONNECTOR_STATE_FINISHED;
        }

        // Fault
        if( strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Fault") == 0)
        {
            return UI_CONNECTOR_STATE_FAULT;
        }

        // Stop
        if( strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Finishing") == 0)
        {
             // Stop
            uint8_t stop_code  = g_ui_json.connectors[gun_idx].stopCode;
            uint8_t conn_type  = g_ui_json.connectors[gun_idx].conn_type;
            uint8_t threshold = 0;

            switch (conn_type)
            {
                case HWCFG_CONN_TYPE_CHADEMO_DC:
                case HWCFG_CONN_TYPE_GBT_DC:
                case HWCFG_CONN_TYPE_SAE_DC:
                case HWCFG_CONN_TYPE_IEC_DC:
                case HWCFG_CONN_TYPE_NACS_DC:
                    threshold = 9;
                    break;
                case HWCFG_CONN_TYPE_GBT_AC:
                case HWCFG_CONN_TYPE_SAE_AC:
                case HWCFG_CONN_TYPE_IEC_AC:
                case HWCFG_CONN_TYPE_GBT_AC_SOCK:
                case HWCFG_CONN_TYPE_SAE_AC_SOCK:
                case HWCFG_CONN_TYPE_IEC_AC_SOCK:
                case HWCFG_CONN_TYPE_SCHUKO:
                    threshold = 2;
                    break;
                default:
                    threshold = 0;
                    break;
            }

            if(stop_code > threshold)
            {
                return UI_CONNECTOR_STATE_SESSION_ERROR;
            }

            return UI_CONNECTOR_STATE_STOPPING;
        }

        if((strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Starting") == 0)
        || (strcmp(g_ui_json.connectors[gun_idx].stateMngrInfo_runState, "Charging") == 0))
        {

#if (COMPILER_FOR == COMPILE_FOR_AC_SSD202  || COMPILER_FOR == COMPILE_FOR_SSD202_43)  // 交流桩跳过processing界面
            return UI_CONNECTOR_STATE_CHARGING;
#endif
            //  Processing
            const char *processStates[] = {
                "CHADEMO_START",
                "CHADEMO_INFORMATION_EXCHANGE",
                "CHADEMO_LOCK",
                "CHADEMO_PREPARING",
                "GBT_START",
                "GBT_SELFCHECK",
                "GBT_HANDSHAKE",
                "GBT_PRRAMETER",
                "GBT_HANDSHAKE",
                "CCS_START",
                "CCS_SELFCHECK",
                "CCS_HANDSHAKE",
                "CCS_PRRAMETER",
                "AC_Idle",
                "AC_Lock",
                "AC_WaitS2",
                "AC_K1K2ON",
                "AC_UserStop",
            };
            const int numStates = sizeof(processStates) / sizeof(processStates[0]);
            for (int i = 0; i < numStates; i++)
            {
                if (strcmp(g_ui_json.connectors[gun_idx].m1768Info_runState, processStates[i]) == 0)
                {
                    // 判断子状态
                    Check_1768(gun_idx);
                    return UI_CONNECTOR_STATE_PROCESSING;
                }
            }

            // Charging
            if((strstr(g_ui_json.connectors[gun_idx].m1768Info_runState, "_CHARGING") != NULL) ||
               (strcmp(g_ui_json.connectors[gun_idx].m1768Info_runState, "AC_Charging") == 0))
            {
                return UI_CONNECTOR_STATE_CHARGING;
            }
        }
    }
    else
    {
        return UI_CONNECTOR_STATE_HOME;
    }

    return UI_CONNECTOR_STATE_HOME;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool ioc_ui_json_set_sign_currency(const char *currency)
{
   // 参数保护
   if (currency == NULL)
       return false;

   // 如果 code 相同，不更新
   if (strcmp(g_ui_json.currencyCode, currency) == 0)
       return false;

   // 遍历已知币种映射表
   for (int i = 0; i < CURRENCY_NUM; i++)
   {
       if (strcmp(c_CurrencyMaps[i].code, currency) == 0)
       {
           const char *newIcon = c_CurrencyMaps[i].icon;

           // 更新 code
           strncpy(g_ui_json.currencyCode, currency, 3);
           g_ui_json.currencyCode[3] = '\0';

           // 更新 icon
           strncpy(g_ui_json.currencyIcon, newIcon, sizeof(g_ui_json.currencyIcon) - 1);
           g_ui_json.currencyIcon[sizeof(g_ui_json.currencyIcon) - 1] = '\0';

           return true;
       }
   }

   // 币种未知：清空 code + icon
   bool changed = false;

   if (g_ui_json.currencyCode[0] != '\0')
   {
       g_ui_json.currencyCode[0] = '\0';
       changed = true;
   }

   if (g_ui_json.currencyIcon[0] != '\0')
   {
       g_ui_json.currencyIcon[0] = '\0';
       changed = true;
   }

   return changed;
}

static bool ioc_ui_json_set_sign_costPriceDecimalPlaces(uint8_t decimalPlaces)
{
    if(g_ui_json.costPriceDecimalPlaces != decimalPlaces)
    {
        g_ui_json.costPriceDecimalPlaces = decimalPlaces;
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ioc_ui_json_set_con_num(int num)
{
    // num = 1; // TODO: remove this line
    if ((g_ui_json.connector_num == 0) && g_ui_json.connector_num != num)   // 枪号只允许修改1次
    {
        LV_LOG_WARN("connector_num %d->%d", g_ui_json.connector_num, num);
        g_ui_json.connector_num = num;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_con_type(int idx, const char *type_str)
{
    if (type_str == NULL) return false;
    if (idx >= UI_NUMOFCONNECTOR) return false;

    const char * c_connectorName[] =
    {
        "CHAdeMO",
        "GBT(DC)",
        "CCS1(DC)",
        "GBT(AC)",
        "CCS2(DC)",
        "J1772(AC)",
        "TYPE2(AC)",
        "GBT-S(AC)",
        "SAE-S(AC)",
        "TYPE2-S(AC)",
        "SCHUKO",
        "NACS(DC)"
    };
    for (int i = 0; i < sizeof(c_connectorName) / sizeof(c_connectorName[0]); i++)
    {
        if (strcmp(type_str, c_connectorName[i]) == 0)
        {
            if (g_ui_json.connectors[idx].conn_type != i)
            {
                // LV_LOG_WARN("[%d]type %d->%d", idx, g_ui_json.connectors[idx].conn_type, i);
                g_ui_json.connectors[idx].conn_type = i;
                return true;
            }
        }
    }

    return false;
}


static inline const char * ioc_ui_connector_state_to_str(ioc_ui_connector_state_t state)
{
    if (state >= 0 && state < UI_CONNECTOR_STATE_MAX &&
        ioc_ui_connector_state_str[state] != NULL) {
        return ioc_ui_connector_state_str[state];
    }
    return "UNKNOWN";
}

bool ioc_ui_json_set_conn_state(int idx, ioc_ui_connector_state_t state)
{
    // #if(COMPILER_FOR==COMPILE_FOR_SSD202_43)
    // g_ui_json.connectors[idx].state = state;
    // #endif
    if (idx >= UI_NUMOFCONNECTOR) return false;


// TODO 本功能是印尼客户定制，后续在印尼客户补丁处体现
// #if((COMPILER_FOR!=COMPILE_FOR_AC_SSD202) && (COMPILER_FOR!=COMPILE_FOR_SSD202_43))

//     if ( UI_CONNECTOR_STATE(idx) ==  UI_CONNECTOR_STATE_SESSION_ERROR ||
//          UI_CONNECTOR_STATE(idx) ==  UI_CONNECTOR_STATE_START_ERROR )
//     {
//         if ( g_ui_json.isUiJsonControlLocked[idx] == true ) return false;
//         g_ui_json.isUiJsonControlLocked[idx] = true;
//     }
// #endif
    // if (idx == 0) state = 14; // TODO: remove this line

    if (g_ui_json.connectors[idx].state != state)
    {
        LV_LOG_WARN("[UI] connector[%d] state: %d(%s) -> %d(%s)\n", idx, g_ui_json.connectors[idx].state,
        ioc_ui_connector_state_to_str(g_ui_json.connectors[idx].state), state, ioc_ui_connector_state_to_str(state));
        g_ui_json.connectors[idx].state = state;
        return true;
    }

    // LV_LOG_WARN("[%d].state:%d", idx, g_ui_json.connectors[idx].state);
    return false;
}

static bool ioc_ui_json_set_con_rated_power(int idx, int rated_power)
{
    if (idx >= UI_NUMOFCONNECTOR) return false;

    if (g_ui_json.connectors[idx].rated_power != rated_power)
    {
        // LV_LOG_WARN("[%d]rated_power %d->%d", idx, g_ui_json.connectors[idx].rated_power, rated_power);
        g_ui_json.connectors[idx].rated_power = rated_power;
        return true;
    }

    return false;
}


static bool ioc_ui_json_set_con_stop_code(int idx, int code)
{
    if (idx >= UI_NUMOFCONNECTOR) return false;

    if (g_ui_json.connectors[idx].stopCode != code)
    {
        LV_LOG_WARN("[%d]stopCode %d->%d", idx, g_ui_json.connectors[idx].stopCode, code);
        g_ui_json.connectors[idx].stopCode = code;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_m1768_err_code(int idx, int code)
{
    if (idx >= UI_NUMOFCONNECTOR) return false;

    if (g_ui_json.connectors[idx].m1768errCode != code)
    {
        // LV_LOG_WARN("[%d]m1768errCode %d->%d", idx, g_ui_json.connectors[idx].m1768errCode, code);
        g_ui_json.connectors[idx].m1768errCode = code;
        return true;
    }

    return false;
}




static bool ioc_ui_json_set_string(int idx, const char *str, char *dest_field, size_t dest_size, const char *field_name)
{
    if (str == NULL) return false;
    if (idx >= UI_NUMOFCONNECTOR) return false;

    if (strcmp(str, dest_field) != 0)
    {
        // DEBUG_INFO("[%d]%s %s->%s", idx, field_name, dest_field, str);
        snprintf(dest_field, dest_size, "%s", str);
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_con_m1768_runstate(int idx, const char *state_str)
{
    return ioc_ui_json_set_string(idx, state_str,
                                  g_ui_json.connectors[idx].m1768Info_runState,
                                  sizeof(g_ui_json.connectors[idx].m1768Info_runState),
                                  "m1768_runstate");
}

static bool ioc_ui_json_set_con_m1768_connstatus(int idx, const char *state_str)
{
    return ioc_ui_json_set_string(idx, state_str,
                                  g_ui_json.connectors[idx].connStatus,
                                  sizeof(g_ui_json.connectors[idx].connStatus),
                                  "m1768_connstatus");
}

static bool ioc_ui_json_set_con_statemng_runstate(int idx, const char *state_str)
{
    return ioc_ui_json_set_string(idx, state_str,
                                  g_ui_json.connectors[idx].stateMngrInfo_runState,
                                  sizeof(g_ui_json.connectors[idx].stateMngrInfo_runState),
                                  "statemng_runstate");
}

static bool ioc_ui_json_set_con_status_real(int idx, const char *state_str)
{
    bool Breturn=ioc_ui_json_set_string(idx, state_str,
                                  g_ui_json.connectors[idx]._status_real,
                                  sizeof(g_ui_json.connectors[idx]._status_real),
                                  "status_real");
    return Breturn;
}

static bool ioc_ui_json_set_con_errcode(int idx, const char *errcode)
{
    // LV_LOG_WARN("idx[%d]===errcode %s", idx, errcode);
    return ioc_ui_json_set_string(idx, errcode,
                                  g_ui_json.connectors[idx]._errCode,
                                  sizeof(g_ui_json.connectors[idx]._errCode),
                                  "errcode");
}

static bool ioc_ui_json_set_con_qrcode(int idx, const char *qrcode)
{
    return ioc_ui_json_set_string(idx, qrcode,
                                  g_ui_json.connectors[idx].state_data.starting.connQR,
                                  sizeof(g_ui_json.connectors[idx].state_data.starting.connQR),
                                  "qrcode");
}

static bool ioc_ui_json_set_idTag(int idx, const char *idTag)
{
    return ioc_ui_json_set_string(idx, idTag,
                                  g_ui_json.connectors[idx].idTag,
                                  sizeof(g_ui_json.connectors[idx].idTag),
                                  "idTag");
}

static bool ioc_ui_json_set_httpAddr(int idx, const char *httpAddr_str)
{
    return ioc_ui_json_set_string(idx, httpAddr_str,
                                  g_ui_json.drvPointInfo.httpAddr,
                                  sizeof(g_ui_json.drvPointInfo.httpAddr),
                                  "httpAddr");
}

static bool ioc_ui_json_set_idType(int idx, IdTagType_t idType)
{
    if (idx >= UI_NUMOFCONNECTOR) return false;

    if (g_ui_json.connectors[idx].idType != idType)
    {
        // LV_LOG_WARN("[%d]m1768errCode %d->%d", idx, g_ui_json.connectors[idx].m1768errCode, code);
        g_ui_json.connectors[idx].idType = idType;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_con_code(int idx, const char *idTag)
{
    return ioc_ui_json_set_string(idx, idTag,
                                  g_ui_json.connectors[idx].connCode,
                                  sizeof(g_ui_json.connectors[idx].connCode),
                                  "connCode");
}

static bool ioc_ui_json_set_timeZone(uint8_t timeZone)
{
    if (g_ui_json.timeZone != timeZone)
    {
        LV_LOG_WARN("timeZone %d->%d", g_ui_json.timeZone, timeZone);
        g_ui_json.timeZone = timeZone;
        return true;
    }

    return false;
}

// 更新激活时间，如果大于当前激活时间，则更新，另外在触摸处理时也会跟新这时间戳
static void ioc_ui_json_set_activeTime(time_t activeTime)
{
    if(activeTime > g_ui_json.biright.activeTime)
    {
        g_ui_json.biright.activeTime = activeTime;
    }

    return;
}

static bool ioc_ui_json_set_price(UI_PriceInfo_t* old, UI_PriceInfo_t* new)
{
    if (memcmp(old, new, sizeof(UI_PriceInfo_t)) != 0)
    {
        // 不相同则整体赋值
        *old = *new;
        return true;
    }
    return false;
}

static bool ioc_ui_json_set_meterdata(UI_MeterData_t* old, UI_MeterData_t* new)
{
    if (memcmp(old, new, sizeof(UI_MeterData_t)) != 0)
    {
        // 不相同则整体赋值
        *old = *new;
        return true;
    }
    return false;
}
static bool ioc_ui_json_set_u8(uint8_t* old, uint8_t* new)
{
    if (* old != * new)
    {
        *old = *new;
        return true;
    }
    return false;
}

static bool ioc_ui_json_set_int(int* old, int* new)
{
    if (* old != * new)
    {
        *old = *new;
        return true;
    }
    return false;
}


static bool ioc_ui_json_set_con_m1768_subState(int idx, uint8_t subState)
{
    if (idx >= UI_NUMOFCONNECTOR) return false;

    if (g_ui_json.connectors[idx].subState != subState)
    {
        g_ui_json.connectors[idx].subState = subState;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_sever_time(time_t new)
{
    time_t local_now = time(NULL);
    double diff = difftime(new, local_now);

    // 若服务器时间与本地时间差超过2秒，则同步本地时间
    if (diff > 2 || diff < -2)
    {
#ifdef _WIN32
        LV_LOG_WARN("[TIME SYNC] settimeofday is not supported on Windows build");
#else
        struct timeval tv;
        tv.tv_sec = new;
        tv.tv_usec = 0;

        if (settimeofday(&tv, NULL) == 0)
        {
            LV_LOG_USER("[TIME SYNC] Local time adjusted to server time (%ld)\n", new);
        }
        else
        {
            LV_LOG_WARN("[TIME SYNC] settimeofday failed");
        }
#endif
    }

    return false;
}

static bool ioc_ui_json_set_time(time_t* old, time_t* new)
{
    if (* old != * new)
    {
        *old = *new;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_double(double* old, double* new)
{
    if (* old != * new)
    {
        *old = *new;
        return true;
    }
    return false;
}


static bool ioc_ui_json_set_cost(UI_CostInfo_t* old, UI_CostInfo_t* new)
{
    if (memcmp(old, new, sizeof(UI_CostInfo_t)) != 0)
    {
        // 不相同则整体赋值
        *old = *new;
        return true;
    }
    return false;
}

static bool ioc_ui_json_set_swFunc(uint32_t swFunc)
{
    if (g_ui_json.swFunc != swFunc)
    {
        g_ui_json.swFunc = swFunc;
        // 软件功能开关更新
        SwFuncCheck(swFunc);
        return true;
    }

    return false;
}


static bool ioc_ui_json_set_keyFunc(int idx,  uint32_t keyFunc)
{
    if (idx < 0 || idx >= KEY_FUNC_COUNT)
    {
        LV_LOG_WARN("Invalid keyFunc index: %d", idx);
        return false;
    }

    if (g_ui_json.keyFunc[idx] != keyFunc)
    {
        // LV_LOG_WARN("[%d]m1768errCode %d->%d", idx, g_ui_json.connectors[idx].m1768errCode, code);
        g_ui_json.keyFunc[idx] = keyFunc;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_isAutoCCS(int idx, int isAutoCCS)
{
    if (idx >= UI_NUMOFCONNECTOR) return false;

    bool ret = (isAutoCCS == cJSON_True);

    if (g_ui_json.isAutoCCS != ret)
    {
        // LV_LOG_WARN("[%d]m1768errCode %d->%d", idx, g_ui_json.connectors[idx].m1768errCode, code);
        g_ui_json.isAutoCCS = ret;
        return true;
    }

    return false;
}

static bool ioc_ui_json_set_bool(bool *field, int jsonValue)
{
    bool ret = (jsonValue == cJSON_True);

    if (*field != ret)
    {
        // LV_LOG_WARN("======================");
        *field = ret;
        return true;
    }

    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ioc_ui_trigger_parse(cJSON * trigger)
{
    const char * trigger_str = cJSON_GetStringValue(cJSON_GetObjectItem(trigger, "Trigger"));
    // 以下字段不一定有，需要根据Trigger类型进行判断
    cJSON* seconds = cJSON_GetObjectItem(trigger, "seconds");
    cJSON* percent = cJSON_GetObjectItem(trigger, "percent");
    cJSON* Msg = cJSON_GetObjectItem(trigger, "Msg");
    cJSON* Content = cJSON_GetObjectItem(trigger, "Content");
    cJSON* qrCodeText = cJSON_GetObjectItem(trigger, "qrCodeText");
    cJSON* qrCodeReceiptText = cJSON_GetObjectItem(trigger, "qrCodeReceiptText");
    cJSON* idx = cJSON_GetObjectItem(trigger, "idx");
    cJSON* action = cJSON_GetObjectItem(trigger, "Action");  //  外设命令PeripheralOperation时使用，目前只支持重启

    if(seconds != NULL)
    {
        g_ui_json.trigger.seconds = seconds->valueint;
    }
    if(percent != NULL)
    {
        g_ui_json.trigger.percent = (unsigned short)percent->valueint;
    }
    if(Msg != NULL)
    {
        g_ui_json.trigger.msg = (uint8_t)Msg->valueint;
    }
    if(Content != NULL)
    {
        snprintf(g_ui_json.trigger.content, sizeof(g_ui_json.trigger.content), "%s", Content->valuestring);
    }
    if(qrCodeText != NULL)
    {
        snprintf(g_ui_json.trigger.billUrlStr, sizeof(g_ui_json.trigger.billUrlStr), "%s", qrCodeText->valuestring);
    }
    if(qrCodeReceiptText != NULL)
    {
        snprintf(g_ui_json.trigger.receiptUrlStr, sizeof(g_ui_json.trigger.receiptUrlStr), "%s", qrCodeReceiptText->valuestring);
    }
    if(idx != NULL)
    {
        g_ui_json.trigger.idx = -1;


#if ((COMPILER_FOR == COMPILE_FOR_CABINET_AM3354) || (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202))
        for ( int i = 0; i < UI_NUMOFCONNECTOR; i++)
        {
            if(idx->valueint == g_ui_json.dispenserConfig.connIdx[i])
            {
                g_ui_json.trigger.idx = i;
            }
        }
#else
        g_ui_json.trigger.idx = idx->valueint;
#endif

    }
    if (action != NULL)
    {
        snprintf(g_ui_json.trigger.action, sizeof(g_ui_json.trigger.action), "%s", action->valuestring);
    }

    #ifdef COLOR_TEST
    if(strcmp(trigger_str, "Test") == 0)
    {
        if(Msg != NULL)
        {
            uint8_t color_idx = (uint8_t)Msg->valueint;
            ui_trigger_panel_show(color_idx);
        }

    }
    #endif
    if(strcmp(trigger_str, "Bill") == 0)
    {
        ui_popup_show_bill();
    }
    else if (strcmp(trigger_str, "Cancel") == 0)
    {
        ui_popup_hide_bill();
        #ifdef COLOR_TEST
        ui_trigger_panel_hide();
        #endif
        ui_popup_common_handle(trigger_str);
    }
    else if(strcmp(trigger_str, "ActiveIdx") == 0)
    {
        ioc_ui_json_set_active_idx(g_ui_json.trigger.idx);
    }
    else
    {
        ui_popup_common_handle(trigger_str);
    }

    // 更新激活时间
    g_ui_json.biright.activeTime = time(NULL);

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static bool parseMeterInfo(const cJSON *meterInfo, UI_MeterData_t *meterData)
// {
//     const char *keys[3] = {"voltage", "current", "Q"};
//     double *dest[3]     = { meterData->voltage,
//                             meterData->current,
//                             meterData->Q };
//     const double unit[3]= { UI_JSON_V_UNIT,
//                             UI_JSON_I_UNIT,
//                             UI_JSON_Q_UNIT };

//     int count = sizeof(keys) / sizeof(keys[0]);

//     for (int k = 0; k < count; k++)
//     {
//         const cJSON *arr = cJSON_GetObjectItem(meterInfo, keys[k]);
//         if (!cJSON_IsArray(arr))
//         {
//             LV_LOG_WARN("'%s' is not an array or not found", keys[k]);
//             return false;
//         }

//         int cnt = cJSON_GetArraySize(arr);
//         if (cnt != (PHASE_NUM + 1))
//         {
//             LV_LOG_WARN("Warning: '%s' length %d != %d", keys[k], cnt, PHASE_NUM + 1);
//             return false;
//         }

//         for (int i = 0; i < (PHASE_NUM + 1); ++i)
//         {
//             const cJSON *item = cJSON_GetArrayItem(arr, i);
//             if (cJSON_IsNumber(item))
//             {
//                 dest[k][i] = item->valuedouble * unit[k];
//             }
//             else
//             {
//                 LV_LOG_WARN("'%s'[%d] is not a number, defaulting to 0.0", keys[k], i);
//                 dest[k][i] = 0.0;
//             }
//         }
//     }

//     return true;
// }

// 拼接四位小数
static void ioc_merge_fourth_decimal(double *q3, uint8_t fourthQ)
{
    int int_part  = (int)(*q3);
    int dec3_part = (int)(*q3 * 1000) % 1000;
    int full4     = dec3_part * 10 + fourthQ;
    *q3 = int_part + full4 / 10000.0;
}


// 根据枪的状态解析更新
static bool ioc_ui_json_parse_state_data(uint8_t idx, cJSON * conn)
{
    // char *formatted = cJSON_Print(conn);
    // printf("Formatted JSON:\n%s\n", formatted);
    // free(formatted);

    // 解析
    bool changed  = false;
    // bool is_Q4dot = false;  // 电量显示四位小数

    UI_MeterData_t meterData;
    memset(&meterData, 0x00, sizeof(UI_MeterData_t));
    UI_PriceInfo_t  currPrice;
    memset(&currPrice, 0x00, sizeof(UI_PriceInfo_t));
    UI_PriceInfo_t  nextPrice;
    memset(&nextPrice, 0x00, sizeof(UI_PriceInfo_t));
    UI_CostInfo_t   cost;
    memset(&cost, 0x00, sizeof(UI_CostInfo_t));

    time_t startTime = 0;       // 起充时间
    time_t stopTime = 0;        // 停充时间
    int duration = 0;           // 充电时长
    int Soc = 0;                // 实时Soc
    int startSoc = 0;           // 起充Soc
    int stopSoc = 0;            // 停止Soc
    time_t reverseTime = 0;     // 预约开始时间
    double actualMeterVal = 0;      // 实际充电电量
    double actualMeter4dot = 0;     // 实际充电电量第四位小数

    cJSON* sessionConnInfo = cJSON_GetObjectItem(conn, "sessionConnInfo"); if(sessionConnInfo == NULL) return false;
    cJSON* saveData = cJSON_GetObjectItem(sessionConnInfo, "saveData"); if(saveData == NULL) return false;
    cJSON* _startTime = cJSON_GetObjectItem(saveData, "_startTime"); if (_startTime == NULL) return false;
    cJSON* _meterStart = cJSON_GetObjectItem(saveData, "_meterStart"); if (_meterStart == NULL) return false;

    cJSON* ramData  = cJSON_GetObjectItem(sessionConnInfo, "ramData"); if(ramData == NULL) return false;
    cJSON* actualStopTime  = cJSON_GetObjectItem(ramData, "actualStopTime"); if(actualStopTime == NULL) return false;
    cJSON* actualDuration  = cJSON_GetObjectItem(ramData, "actualDuration"); if(actualDuration == NULL) return false;
    cJSON* actualMeter     = cJSON_GetObjectItem(ramData, "actualMeter"); if(actualMeter == NULL) return false;

    duration = actualDuration->valueint;
    stopTime = (time_t)(actualStopTime->valueint);

    // 因为在Finish时actualStopTime会被清空，所以用结束时间减去充电时长来计算起充时间
    if(g_ui_json.connectors[idx].state == UI_CONNECTOR_STATE_FINISHED)
    {
        startTime = (time_t)(actualStopTime->valueint - actualDuration->valueint);
    }
    else
    {
        startTime = (time_t)(_startTime->valueint);
    }

    // Soc相关字段只有直流桩有，所以有可能是空
    cJSON* m1768Info = cJSON_GetObjectItem(conn, "m1768Info"); if(m1768Info == NULL) return false;
    cJSON* SocItem = cJSON_GetObjectItem(m1768Info, "SoC");
    if(SocItem != NULL)
    {
        Soc = SocItem->valueint;
    }
    cJSON* startSoCItem = cJSON_GetObjectItem(m1768Info, "startSoC");
    if(startSoCItem != NULL)
    {
        startSoc = startSoCItem->valueint;
    }

    cJSON* stateMngrInfo = cJSON_GetObjectItem(conn, "stateMngrInfo"); if(stateMngrInfo == NULL) return false;
    cJSON* stopSocItem = cJSON_GetObjectItem(stateMngrInfo, "stopSoc");
    if(stopSocItem != NULL)
    {
        stopSoc = stopSocItem->valueint;
    }

    cJSON* reserveConnInfo = cJSON_GetObjectItem(conn, "reserveConnInfo"); if(reserveConnInfo == NULL) return false;
    cJSON* expireDate = cJSON_GetObjectItem(reserveConnInfo, "expireDate"); if(expireDate == NULL) return false;
    reverseTime = (time_t)expireDate->valueint;


    // 解析电表数据
    cJSON * meterInfo   = cJSON_GetObjectItem(conn, "meterInfo");    if(meterInfo == NULL) return false;
    cJSON * qArray =  cJSON_GetObjectItem(meterInfo, "Q");
    cJSON * q3 = cJSON_GetArrayItem(qArray, 3);
    meterData.Q[3] = q3->valueint;

#if ((COMPILER_FOR != COMPILE_FOR_AC_SSD202) && (COMPILER_FOR != COMPILE_FOR_SSD202_43))
    cJSON * outputCurArray = cJSON_GetObjectItem(m1768Info, "outputCur");       // Array
    cJSON * outputVolArray = cJSON_GetObjectItem(m1768Info, "outputVol");       // Array
    int sizeOut = cJSON_GetArraySize(outputCurArray);
    for (int j=0; j<sizeOut; j++)
    {
        cJSON * outputCur = cJSON_GetArrayItem(outputCurArray, j);              // double
        meterData.current[j] = outputCur->valuedouble * UI_JSON_I_UNIT;

        cJSON * outputVol = cJSON_GetArrayItem(outputVolArray, j);              // double
        meterData.voltage[j] = outputVol->valuedouble * UI_JSON_V_UNIT;
    }
    #else
    cJSON * Varray = cJSON_GetObjectItem(meterInfo, "voltage");
    cJSON * Carray = cJSON_GetObjectItem(meterInfo, "current");

    int sizeOut = cJSON_GetArraySize(Varray);
    for (int j = 0; j < sizeOut; j++)
    {
        cJSON * outputCur = cJSON_GetArrayItem(Carray, j);      // double
        meterData.current[j] = outputCur ? (outputCur->valuedouble * UI_JSON_I_UNIT) : 0.0;

        cJSON * outputVol = cJSON_GetArrayItem(Varray, j);      // double
        meterData.voltage[j] = outputVol ? (outputVol->valuedouble * UI_JSON_V_UNIT) : 0.0;
    }

    // 计算电流平均值
    if (meterData.current[3] == 0)
    {
        double sum = 0.0;
        int count = 0;

        for (int k = 0; k < PHASE_NUM; k++)
        {
            if (meterData.current[k] > 0)    // 0 不计入平均
            {
                sum += meterData.current[k];
                count++;
            }
        }
        if (count > 0)
        {
            meterData.current[3] = sum / count;
        }
    }


#endif

    // if(!parseMeterInfo(meteinfo, &meterData))  return false;

    // 本地计算功率
    meterData.power = ((meterData.voltage[0] * meterData.current[0]) +
                       (meterData.voltage[1] * meterData.current[1]) +
                       (meterData.voltage[2] * meterData.current[2])) * UI_JSON_P_UNIT;


    int meterStart = _meterStart->valueint;

    double realQ = 0; // 实际使用功率 当前电表值 - 电表初始值   显示四位小数时单位：0.0001kWh  否则单位：0.001kWh

    cJSON * dotQ4      = cJSON_GetObjectItem(meterInfo, "dotQ4");
    cJSON* startQ4dot = cJSON_GetObjectItem(saveData, "_startQ4dot");
    if ((dotQ4 != NULL) && (startQ4dot != NULL)) // 电量显示四位小数
    {
        g_ui_json.connectors[idx].isQ4dot = true;
        meterData.fourthQ  = (uint8_t)(cJSON_GetArrayItem(dotQ4, 3)->valueint);
        meterData.startQ4dot = (uint8_t)startQ4dot->valueint;

        realQ = (meterData.Q[3] * 10 + meterData.fourthQ) - (meterStart * 10 + meterData.startQ4dot);
        realQ = realQ * 0.1f;
    }
    else
    {
        realQ =  meterData.Q[3] - meterStart;
    }

    meterData.Q[3] = realQ * UI_JSON_Q_UNIT;

    actualMeterVal = actualMeter->valuedouble * UI_JSON_Q_UNIT;


    cJSON* actualQ4  = cJSON_GetObjectItem(ramData, "actualQ4");
    if (actualQ4 != NULL)
    {
        // 有这个字段表示需要显示四位小数
        g_ui_json.connectors[idx].isQ4dot = true;
        actualMeter4dot  = (uint8_t)(actualQ4->valueint);
        ioc_merge_fourth_decimal(&actualMeterVal, actualMeter4dot);
    }

    // 测试用
    // g_ui_json.connectors[idx].isQ4dot = true;

    // 解析价格费用信息
    cJSON * priceInfo = cJSON_GetObjectItem(conn, "priceInfo");
    if(priceInfo != NULL)
    {
        // 有这个字段表示要显示价格和费用信息
        g_ui_json.connectors[idx].showPrice = true;

        cJSON* costItem = cJSON_GetObjectItem(sessionConnInfo, "cost"); if(costItem == NULL) return false;
        cost.totalCost        = cJSON_GetObjectItem(costItem, "totalCost")->valuedouble;
        cost.reserveIdleCost  = cJSON_GetObjectItem(costItem, "reserveIdleCost")->valuedouble;
        cost.suspendIdleCost  = cJSON_GetObjectItem(costItem, "suspendIdleCost")->valuedouble;
        cost.stopIdleCost     = cJSON_GetObjectItem(costItem, "stopIdleCost")->valuedouble;

        cJSON* currPriceItem = cJSON_GetObjectItem(priceInfo, "currPrice"); if(currPriceItem == NULL) return false;
        cJSON* nextPriceItem = cJSON_GetObjectItem(priceInfo, "nextPrice"); if(nextPriceItem == NULL) return false;

        currPrice.startMin  = cJSON_GetObjectItem(currPriceItem, "startMin")->valueint;
        currPrice.kWhPrice  = cJSON_GetObjectItem(currPriceItem, "kWhPrice")->valuedouble;
        currPrice.hourPrice = cJSON_GetObjectItem(currPriceItem, "hourPrice")->valuedouble;
        currPrice.flatFee   = cJSON_GetObjectItem(currPriceItem, "flatFee")->valuedouble;

        nextPrice.startMin  = cJSON_GetObjectItem(nextPriceItem, "startMin")->valueint;
        nextPrice.endMin    = cJSON_GetObjectItem(nextPriceItem, "endMin")->valueint;
        nextPrice.kWhPrice  = cJSON_GetObjectItem(nextPriceItem, "kWhPrice")->valuedouble;
        nextPrice.hourPrice = cJSON_GetObjectItem(nextPriceItem, "hourPrice")->valuedouble;
        nextPrice.flatFee   = cJSON_GetObjectItem(nextPriceItem, "flatFee")->valuedouble;

        cJSON* priceText = cJSON_GetObjectItem(priceInfo, "priceText");
        if(priceText != NULL)
        {
            g_ui_json.connectors[idx].isOcppPrice = true;

            snprintf(currPrice.priceText, sizeof(currPrice.priceText), "%s", priceText->valuestring);
        }
        else
        {
            g_ui_json.connectors[idx].isOcppPrice = false;
        }
    }
    else
    {
        g_ui_json.connectors[idx].showPrice = false;
    }

#if ((COMPILER_FOR != COMPILE_FOR_AC_SSD202) && (COMPILER_FOR != COMPILE_FOR_SSD202_43))
    // charging界面detailed里的数据在此更新
    if(g_ui_json.connectors[idx].state == UI_CONNECTOR_STATE_CHARGING)
    {
        // 在其他地方已经解析过的数据在此不重复解析，直接调用
        // m1768Info_runState
        double  tempVal = cJSON_GetObjectItem(m1768Info, "reqVol")->valuedouble / 10;
        changed |= ioc_ui_json_set_double(&g_ui_json.connectors[idx].reqVol, &tempVal);
        tempVal = cJSON_GetObjectItem(m1768Info, "reqCur")->valuedouble / 10;
        changed |= ioc_ui_json_set_double(&g_ui_json.connectors[idx].reqCur, &tempVal);
        tempVal = cJSON_GetObjectItem(m1768Info, "offerVol")->valuedouble / 10;
        changed |= ioc_ui_json_set_double(&g_ui_json.connectors[idx].offerVol, &tempVal);
        tempVal = cJSON_GetObjectItem(m1768Info, "offerCur")->valuedouble / 10;
        changed |= ioc_ui_json_set_double(&g_ui_json.connectors[idx].offerCur, &tempVal);
        tempVal = cJSON_GetObjectItem(m1768Info, "outLimit")->valuedouble / 10;
        changed |= ioc_ui_json_set_double(&g_ui_json.connectors[idx].outLimit, &tempVal);
        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].compGroup, &cJSON_GetObjectItem(m1768Info, "compGroup")->valueint);
        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].startSoc, &startSoc);

        // 温度比较特殊，需要根据环境的温度和枪的温度进行判断
        cJSON * tempConnArray = cJSON_GetObjectItem(m1768Info, "tempConn");       // Array
        int sizeOut = cJSON_GetArraySize(tempConnArray);
        double temp[3] = {0};
        int conn_temp = -60;
        for (int j=0; j < sizeOut; j++)
        {
            cJSON * tempConn = cJSON_GetArrayItem(tempConnArray, j);              // double
            temp[j] = tempConn->valuedouble;
            if (temp[j] > conn_temp)
            {
                conn_temp = temp[j];
            }
        }
        int maxTemp = (conn_temp > s_point_temp) ? conn_temp : s_point_temp;

        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].temp, &maxTemp);

        changed |= ioc_ui_json_set_string(idx, cJSON_GetObjectItem(m1768Info, "unit")->valuestring,
            g_ui_json.connectors[idx].outLimitUnit,
            sizeof(g_ui_json.connectors[idx].outLimitUnit),
            "outLimitUnit");
    }
#endif

    // 数据更新
    switch (g_ui_json.connectors[idx].state)
    {
    case UI_CONNECTOR_STATE_IDLE:
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.idle.currPrice,  &currPrice);
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.idle.nextPrice,  &nextPrice);
        break;
    case UI_CONNECTOR_STATE_STARTING1:
    case UI_CONNECTOR_STATE_STARTING2:
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.starting.currPrice,  &currPrice);
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.starting.nextPrice,  &nextPrice);
        break;
    case UI_CONNECTOR_STATE_CHARGING:
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.charging.currPrice,  &currPrice);
        changed |= ioc_ui_json_set_cost(&g_ui_json.connectors[idx].state_data.charging.cost,  &cost);
        changed |= ioc_ui_json_set_meterdata(&g_ui_json.connectors[idx].state_data.charging.meterData,  &meterData);
        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].state_data.charging.soc, &Soc);
        changed |= ioc_ui_json_set_time(&g_ui_json.connectors[idx].state_data.charging.startTime, &startTime);

        break;
    case UI_CONNECTOR_STATE_FINISHED:
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.charging.currPrice,  &currPrice);
        changed |= ioc_ui_json_set_cost(&g_ui_json.connectors[idx].state_data.finished.cost,  &cost);
        changed |= ioc_ui_json_set_double(&g_ui_json.connectors[idx].state_data.finished.actualMeter,  &actualMeterVal);
        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].state_data.finished.startSoc, &startSoc);
        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].state_data.finished.stopSoc, &stopSoc);
        changed |= ioc_ui_json_set_time(&g_ui_json.connectors[idx].state_data.finished.startTime, &startTime);
        changed |= ioc_ui_json_set_time(&g_ui_json.connectors[idx].state_data.finished.stopTime, &stopTime);
        changed |= ioc_ui_json_set_int(&g_ui_json.connectors[idx].state_data.finished.duration, &duration);
        break;
    case UI_CONNECTOR_STATE_RESERVED:
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.reserved.currPrice,  &currPrice);
        changed |= ioc_ui_json_set_price(&g_ui_json.connectors[idx].state_data.reserved.nextPrice,  &nextPrice);
        changed |= ioc_ui_json_set_time(&g_ui_json.connectors[idx].state_data.reserved.reverseTime, &reverseTime);
        break;
    default:
        break;
    }

    return changed;
}

bool ioc_ui_json_parse(const char *json_str)
{
    bool changed = false;
    bool rtn = false;
    uint8_t tempValue_U8 = 0;
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL)
    {
        LV_LOG_ERROR("parse json failed");
        return false;
    }

    //  LV_LOG_WARN("recv==========================");

    // DEBUG_WARN("[%s]", json_str);

    // 收到Trigger做特殊处理
    if(cJSON_GetObjectItem(root, "Trigger"))
    {
        LV_LOG_WARN("[%s]", json_str);
        changed = ioc_ui_trigger_parse(root);
    }
    else
    {
        cJSON * global = cJSON_GetObjectItem(root, "global");
        if ((global == NULL) || (!cJSON_IsObject(global))) goto FAILED_JSON;

        cJSON * activeInfo = cJSON_GetObjectItem(global, "activeInfo");
        if (activeInfo != NULL)
        {
            cJSON * activeTime = cJSON_GetObjectItem(global, "activeTime");
            if (activeTime != NULL)
            {
                ioc_ui_json_set_activeTime((time_t)activeTime->valueint);
            }
        }

        cJSON * userConfigInfo = cJSON_GetObjectItem(global, "userConfigInfo");
        if (userConfigInfo != NULL)
        {
            cJSON * language = cJSON_GetObjectItem(userConfigInfo, "language");
            if (language != NULL)
            {
                changed |= ioc_ui_json_set_language(language->valueint);
            }
            cJSON *signCurrency = cJSON_GetObjectItem(userConfigInfo, "signCurrency");
            if (signCurrency != NULL)
            {
                changed |= ioc_ui_json_set_sign_currency(signCurrency->valuestring);
            }

            // 价格和费用的小数位长度，没有的话默认为2
            cJSON *costPriceDecimalPlaces = cJSON_GetObjectItem(userConfigInfo, "costPriceDecimalPlaces");
            if (costPriceDecimalPlaces != NULL)
            {
                changed |= ioc_ui_json_set_sign_costPriceDecimalPlaces((uint8_t)costPriceDecimalPlaces->valueint);
            }
#if (COMPILER_FOR == COMPILE_FOR_AC_SSD202)
            // 时间
            cJSON *time = cJSON_GetObjectItem(userConfigInfo, "time");
            if (time != NULL)
            {
                time_t new_time = (time_t)time->valueint;
                changed |= ioc_ui_json_set_sever_time(new_time);
            }
#endif
            // 设置亮度相关配置
            cJSON *brightIdle = cJSON_GetObjectItem(userConfigInfo, "brightIdle");
            if (brightIdle != NULL)
            {
                tempValue_U8 = (uint8_t)brightIdle->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.biright.brightIdle, &tempValue_U8);
            }
            cJSON *brightActive = cJSON_GetObjectItem(userConfigInfo, "brightActive");
            if (brightActive != NULL)
            {
                tempValue_U8 = (uint8_t)brightActive->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.biright.brightActive, &tempValue_U8);
            }
            cJSON *brightCharging = cJSON_GetObjectItem(userConfigInfo, "brightCharging");
            if (brightCharging != NULL)
            {
                tempValue_U8 = (uint8_t)brightCharging->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.biright.brightCharging, &tempValue_U8);
            }
            cJSON *timeoutActive = cJSON_GetObjectItem(userConfigInfo, "timeoutActive");
            if (timeoutActive != NULL)
            {
                tempValue_U8 = (uint8_t)timeoutActive->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.biright.timeoutActive, &tempValue_U8);
            }

            // 功能开关
            cJSON * swFunc = cJSON_GetObjectItem(userConfigInfo, "swFunc");
            if (swFunc != NULL)
            {
                changed |= ioc_ui_json_set_swFunc((uint32_t)swFunc->valueint);
            }

            // 按钮功能
            cJSON * keyFuncArry = cJSON_GetObjectItem(userConfigInfo, "keyFunc");
            if (keyFuncArry != NULL)
            {
                int size = cJSON_GetArraySize(keyFuncArry);
                for (int j = 0; j < size; j++)
                {
                    cJSON * keyFunc = cJSON_GetArrayItem(keyFuncArry, j);              // double
                    changed |= ioc_ui_json_set_keyFunc(j, (uint32_t)keyFunc->valueint);
                }
            }
        }

        cJSON * sysconfigInfo = cJSON_GetObjectItem(global, "sysconfigInfo");
        if (sysconfigInfo != NULL)
        {
            cJSON * timeZone = cJSON_GetObjectItem(sysconfigInfo, "timeZone");
            if (timeZone != NULL)
            {
                changed |= ioc_ui_json_set_timeZone((uint8_t)timeZone->valueint);
            }
        }

        cJSON * m1768Peris = cJSON_GetObjectItem(global, "m1768Peri");
        if (m1768Peris != NULL)
        {
            cJSON * m1768Peri = cJSON_GetArrayItem(m1768Peris, 0);
            if (m1768Peri != NULL)
            {
                s_point_temp = -20;

                cJSON * temp1 = cJSON_GetObjectItem(m1768Peri, "temp1");
                if (temp1 != NULL)
                {
                    if (temp1->valueint > s_point_temp)
                    {
                        s_point_temp = temp1->valueint;
                    }
                }
#if ((COMPILER_FOR != COMPILE_FOR_AC_SSD202)||(COMPILER_FOR != COMPILE_FOR_SSD202_43))
                cJSON * temp2 = cJSON_GetObjectItem(m1768Peri, "temp2");
                if (temp2 != NULL)
                {
                    if (temp2->valueint > s_point_temp)
                    {
                        s_point_temp = temp2->valueint;
                    }
                }
#endif
            }

        }

        cJSON * drvPointInfo = cJSON_GetObjectItem(global, "drvPointInfo");
        if (drvPointInfo != NULL)
        {
            cJSON * meterFault = cJSON_GetObjectItem(drvPointInfo, "meterFault");
            if(meterFault != NULL)
            {
                // LV_LOG_WARN("======================");
                changed |= ioc_ui_json_set_bool(&g_ui_json.drvPointInfo.meterFault, meterFault->type);
            }

            cJSON * rfFault = cJSON_GetObjectItem(drvPointInfo, "rfFault");
            if(rfFault != NULL)
            {
                changed |= ioc_ui_json_set_bool(&g_ui_json.drvPointInfo.rfFault, rfFault->type);
            }

            cJSON * online = cJSON_GetObjectItem(drvPointInfo, "online");
            if(online != NULL)
            {
                changed |= ioc_ui_json_set_bool(&g_ui_json.drvPointInfo.online, online->type);
            }

            cJSON * ethStatus = cJSON_GetObjectItem(drvPointInfo, "ethStatus");
            if(ethStatus != NULL)
            {
                tempValue_U8 = (uint8_t)ethStatus->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.drvPointInfo.ethStatus, &tempValue_U8);
            }

            cJSON * wifiStatus = cJSON_GetObjectItem(drvPointInfo, "wifiStatus");
            if(wifiStatus != NULL)
            {
                tempValue_U8 = (uint8_t)wifiStatus->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.drvPointInfo.wifiStatus, &tempValue_U8);
            }

            cJSON * m4gStatus = cJSON_GetObjectItem(drvPointInfo, "m4gStatus");
            if(m4gStatus != NULL)
            {
                tempValue_U8 = (uint8_t)m4gStatus->valueint;
                changed |= ioc_ui_json_set_u8(&g_ui_json.drvPointInfo.m4gStatus, &tempValue_U8);
            }

            cJSON * httpAddr = cJSON_GetObjectItem(drvPointInfo, "httpAddr");
            if(httpAddr != NULL)
            {
                changed |= ioc_ui_json_set_httpAddr(0, cJSON_GetStringValue(httpAddr));
            }
        }

        cJSON * conns = cJSON_GetObjectItem(root, "conns");
        if ((conns == NULL) || (!cJSON_IsArray(conns))) goto FAILED_JSON;

        int conns_len = cJSON_GetArraySize(conns);
#if ((COMPILER_FOR == COMPILE_FOR_DC_AM3354) || (COMPILER_FOR == COMPILE_FOR_AC_SSD202)||(COMPILER_FOR == COMPILE_FOR_SSD202_43))
        if (conns_len > UI_MAX_CONNECTOR_NUM) goto FAILED_JSON;
        changed |= ioc_ui_json_set_con_num(conns_len);   // 分体桩在MQTT处设置
#else
    if(conns_len != UI_NUMOFCONNECTOR) goto FAILED_JSON;  // 等MQTT将所有枪数据收集齐再处理
#endif

        // LV_LOG_WARN("conns_len[%d]", conns_len);
        cJSON * creditCardInfo = cJSON_GetObjectItem(global, "creditCardInfo");
        if (creditCardInfo != NULL)
        {
            // 有这个字段表示启用了POS机
            g_ui_json.enPOS = true;

            cJSON *enSoftKey = cJSON_GetObjectItem(creditCardInfo, "enSoftKey");

            if (enSoftKey != NULL && cJSON_IsBool(enSoftKey))
            {
                g_ui_json.enSoftKey = cJSON_IsTrue(enSoftKey);
                // LV_LOG_WARN("enSoftKey: %d", g_ui_json.enSoftKey);
            }
        }
        else
        {
            g_ui_json.enPOS = false;
        }


        // 测试用
         g_ui_json.enPOS = true;

        // 解析枪数据
        for (int i = 0; (i < conns_len && i < UI_NUMOFCONNECTOR); i++)
        {
            int idx = i;
            cJSON * conn = cJSON_GetArrayItem(conns, idx);
            if ((conn == NULL) || (!cJSON_IsObject(conn))) goto FAILED_JSON;

            cJSON * meterInfo = cJSON_GetObjectItem(conn, "meterInfo");
            cJSON * connConfig = cJSON_GetObjectItem(conn, "connConfig");
            if (connConfig != NULL)
            {
                cJSON * ratePower = cJSON_GetObjectItem(connConfig, "ratePower");
                if (ratePower != NULL)
                {
                    changed |= ioc_ui_json_set_con_rated_power(i, ratePower->valueint);
                }
                cJSON * connQR = cJSON_GetObjectItem(connConfig, "connQR");
                if (connQR != NULL)
                {
                    changed |= ioc_ui_json_set_con_qrcode(i, connQR->valuestring);
                }
                cJSON * connCode = cJSON_GetObjectItem(connConfig, "connCode");
                if (connCode != NULL)
                {
                    changed |= ioc_ui_json_set_con_code(i, connCode->valuestring);
                }

            }
            cJSON * m1768Info = cJSON_GetObjectItem(conn, "m1768Info");
            if (m1768Info != NULL)
            {
                changed |= ioc_ui_json_set_con_type(i, cJSON_GetStringValue(cJSON_GetObjectItem(m1768Info, "connType")));
                changed |= ioc_ui_json_set_con_m1768_runstate(i, cJSON_GetStringValue(cJSON_GetObjectItem(m1768Info, "runState")));
                cJSON * subState = cJSON_GetObjectItem(m1768Info, "subState");
                if (subState != NULL)
                {
                    changed |= ioc_ui_json_set_con_m1768_subState(i, (uint8_t)subState->valueint);
                }

                changed |= ioc_ui_json_set_con_m1768_connstatus(i, cJSON_GetStringValue(cJSON_GetObjectItem(m1768Info, "connStatus")));


// TODO 从未插枪到插枪表示激活哪根枪， 分体用ActiveIdx控制，现在homeOCpp授权组还没合并到主分支，后续合并后此处考虑修改
#if ((COMPILER_FOR == COMPILE_FOR_DC_AM3354) || (COMPILER_FOR == COMPILE_FOR_AC_SSD202))

            // LV_LOG_WARN("==idx[%d].connStatus[%s] plug[%d] ", i, g_ui_json.connectors[i].connStatus, g_ui_json.connectors[i].plug);
            if (strcmp(g_ui_json.connectors[i].connStatus, "Unplug") != 0)
            {
                if(!g_ui_json.connectors[i].plug)
                {
                    ioc_ui_json_set_active_idx(i);
                    g_ui_json.connectors[i].plug = true;
                }
            }
            else
            {
                g_ui_json.connectors[i].plug = false;
            }
#else
            if (strcmp(g_ui_json.connectors[i].connStatus, "Unplug") != 0)
            {
                if(!g_ui_json.connectors[i].plug)
                {
                    // 更新激活时间
                    g_ui_json.biright.activeTime = time(NULL); // TODO此处插枪后点亮屏幕，未进行激活枪，激活枪还是由homeOCPP下发，此处逻辑待优化
                    g_ui_json.connectors[i].plug = true;
                }
            }
            else
            {
                g_ui_json.connectors[i].plug = false;
            }
#endif


                cJSON * errCode = cJSON_GetObjectItem(m1768Info, "errCode");
                if ( errCode != NULL )
                {
                    changed |= ioc_ui_json_set_m1768_err_code(i, errCode->valueint);
                }
            }
            cJSON * plcInfo = cJSON_GetObjectItem(conn, "plcInfo");
            cJSON * stateMngrInfo = cJSON_GetObjectItem(conn, "stateMngrInfo");
            if (stateMngrInfo != NULL)
            {
                changed |= ioc_ui_json_set_con_statemng_runstate(i, cJSON_GetStringValue(cJSON_GetObjectItem(stateMngrInfo, "runState")));
                cJSON * stopCode = cJSON_GetObjectItem(stateMngrInfo, "stopCode");
                if ( stopCode != NULL )
                {
                    changed |= ioc_ui_json_set_con_stop_code(i, stopCode->valueint);
                }
            }
            cJSON * reserveConnInfo = cJSON_GetObjectItem(conn, "reserveConnInfo");
            cJSON * sessionConnInfo = cJSON_GetObjectItem(conn, "sessionConnInfo");
            if (sessionConnInfo != NULL)
            {
                cJSON * saveData = cJSON_GetObjectItem(sessionConnInfo, "saveData");
                if(saveData != NULL)
                {
                    cJSON * _idAuth   = cJSON_GetObjectItem(saveData , "_idAuth");
                    changed |= ioc_ui_json_set_idTag(i, cJSON_GetStringValue(cJSON_GetObjectItem(_idAuth, "idTag")));
                    cJSON * idType = cJSON_GetObjectItem(_idAuth, "idType");
                    if(idType != NULL)
                    {
                        changed |= ioc_ui_json_set_idType(i, (IdTagType_t)idType->valueint);
                    }

                    cJSON * isAutoCCS = cJSON_GetObjectItem(saveData, "isAutoCCS");
                    if(isAutoCCS != NULL)
                    {
                        changed |= ioc_ui_json_set_isAutoCCS(i, isAutoCCS->type);
                    }
                }

                cJSON * ramData = cJSON_GetObjectItem(sessionConnInfo, "ramData");
                if ( ramData != NULL )
                {
                    changed |= ioc_ui_json_set_con_status_real(i, cJSON_GetStringValue(cJSON_GetObjectItem(ramData, "_status_real")));
                    changed |= ioc_ui_json_set_con_errcode(i, cJSON_GetStringValue(cJSON_GetObjectItem(ramData, "_errCode")));
                }
            }

            ioc_ui_connector_state_t conn_state = ui_page_status_judgment(i);
            // LV_LOG_WARN("[%d] ========= conn_state: %d", i, conn_state);
            changed |= ioc_ui_json_set_conn_state(i, conn_state);

            // 解析各个状态的数据
            changed |= ioc_ui_json_parse_state_data(i, conn);
        }
    }

    // 测试用
    // changed = true;

    rtn = changed;

    // LV_LOG_WARN("changed[%d]==========================", rtn);

FAILED_JSON:
    cJSON_Delete(root);
    return rtn;
}

bool ioc_ui_json_create_udp_socket()
{
    struct sockaddr_in serv_addr;

#ifdef _WIN32
    static bool wsa_initialized = false;
    if (!wsa_initialized)
    {
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        {
            LV_LOG_ERROR("WSAStartup failed");
            return false;
        }
        wsa_initialized = true;
    }
#endif

    // 创建UDP套接字
    s_ui_json_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (s_ui_json_sockfd < 0)
    {
        LV_LOG_ERROR("create socket failed: %s", strerror(errno));
        return false;
    }
#if (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202)
    // 打开广播选项
    int opt = 1;
    if (setsockopt(s_ui_json_sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) < 0)
    {
        LV_LOG_ERROR("setsockopt SO_BROADCAST failed: %s", strerror(errno));
#ifdef _WIN32
        closesocket(s_ui_json_sockfd);
#else
        close(s_ui_json_sockfd);
#endif
        return false;
    }
#endif

    // 设置套接字为非阻塞模式
#ifdef _WIN32
    u_long non_blocking = 1;
    if (ioctlsocket(s_ui_json_sockfd, FIONBIO, &non_blocking) != 0)
    {
        LV_LOG_ERROR("FIONBIO failed");
        closesocket(s_ui_json_sockfd);
        return false;
    }
#else
    int flags = fcntl(s_ui_json_sockfd, F_GETFL, 0);
    if (fcntl(s_ui_json_sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        LV_LOG_ERROR("O_NONBLOCK failed: %s", strerror(errno));
        close(s_ui_json_sockfd);
        return false;
    }
#endif

    // 初始化服务器地址结构
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // 绑定到所有可用的网络接口
    serv_addr.sin_port = htons(SENDUI_LCD3354_DST_PORT);      // 绑定到端口20001

    // 绑定套接字到指定端口
    if (bind(s_ui_json_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        LV_LOG_WARN("bind failed: %s", strerror(errno));
#ifdef _WIN32
        closesocket(s_ui_json_sockfd);
#else
        close(s_ui_json_sockfd);
#endif
        return false;
    }

    LV_LOG_WARN("Socket OK");
    return true;
}

// 在 dir_path 目录下查找形如 "AddrDispenser<digits>" 的文件， 并返回数字部分；失败返回 0。
uint8_t static ioc_extract_dispenser_index_in_dir(const char *dir_path)
{
    DIR *dp = opendir(dir_path);
    if (!dp)
    {
        LV_LOG_WARN("Open dir %s failed", dir_path);
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL)
    {
        int idx;
        // 只匹配文件名，不区分文件/目录，用 %n 检查是否全匹配
        int n;
        if (sscanf(entry->d_name, "AddrDispenser%d%n", &idx, &n) == 1
            && entry->d_name[n] == '\0') {
            closedir(dp);
            return idx;
        }
    }

    closedir(dp);

    // 没找到
    LV_LOG_WARN("No AddrDispenser found in %s", dir_path);
    return 0;
}

bool ioc_ui_json_init(void)
{
    memset(&g_ui_json, 0, sizeof(g_ui_json));
    g_ui_json.active_connector_idx = -1;    // 初始化设置为未选中枪
    g_ui_json.costPriceDecimalPlaces = 2;   // 价格和费用小数位默认为2
    g_ui_json.language = -1;
    g_ui_json.isFirstUIjson = true;
    // 亮度配置
    g_ui_json.biright.brightActive = 5;
    g_ui_json.biright.brightIdle = 2;
    g_ui_json.biright.brightCharging = 3;
    g_ui_json.biright.timeoutActive = 1;

    // 电表和读卡器状态信息
    g_ui_json.drvPointInfo.meterFault = true;
    g_ui_json.drvPointInfo.rfFault = true;


#if ((COMPILER_FOR == COMPILE_FOR_CABINET_AM3354) || (COMPILER_FOR == COMPILE_FOR_CABINET_SSD202))
    g_ui_json.dispenserConfig.location = ioc_extract_dispenser_index_in_dir(IOC_PATH); // 从本地获取屏幕位置信息
    g_ui_json.dispenserConfig.connNum = 1;     // 默认一根枪
    for (int i = 0; i < UI_MAX_CONNECTOR_NUM; i++)
    {
         g_ui_json.dispenserConfig.connIdx[i] = 0;
    }

    LV_LOG_WARN("location[%d]", g_ui_json.dispenserConfig.location);

    g_ui_json_eventfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
#endif

    // 测试用
    // ioc_ui_json_set_con_num(2);

    return ioc_ui_json_create_udp_socket();
}

bool ioc_ui_json_step(void)
{
    fd_set readfds;
    struct timeval timeout;
    int n;

    // 初始化fd集合
    FD_ZERO(&readfds);
    int max_fd = -1;

    if (s_ui_json_sockfd >= 0)
    {
        FD_SET(s_ui_json_sockfd, &readfds);
        max_fd = s_ui_json_sockfd;
    }

    if (g_ui_json_eventfd >= 0)
    {
        FD_SET(g_ui_json_eventfd, &readfds);
        if (g_ui_json_eventfd > max_fd)
        {
            max_fd = g_ui_json_eventfd;
        }
    }

    // 设置超时时间为0，实现非阻塞检查
    timeout.tv_sec = 0;  // 超时时间为0秒
    timeout.tv_usec = 0; // 超时时间为0微秒

    // 使用select检查套接字是否有数据可读
    n = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
    if (n > 0)
    {
#if ((COMPILER_FOR == COMPILE_FOR_DC_AM3354) || (COMPILER_FOR == COMPILE_FOR_AC_SSD202) ||(COMPILER_FOR == COMPILE_FOR_SSD202_43))
        if(FD_ISSET(s_ui_json_sockfd, &readfds))
        {
            // 有数据可读, 暂时不考虑分包问题
            struct sockaddr_in cliaddr;
            socklen_t cliaddrlen = sizeof(cliaddr);
            memset(g_ui_json_buffer, 0x00, UI_JSON_BUFFER_SIZE);
            int bytes_received = recvfrom(s_ui_json_sockfd, g_ui_json_buffer, UI_JSON_BUFFER_SIZE, 0,
                                        (struct sockaddr *)&cliaddr, &cliaddrlen);
            if (bytes_received < 0)
            {
                LV_LOG_ERROR("recvfrom failed: %s", strerror(errno));
                return false;
            }
            else
            {
                inet_ntop(AF_INET, &cliaddr.sin_addr, s_devMasterIp, sizeof(s_devMasterIp));
                uint16_t sender_port = ntohs(cliaddr.sin_port);

                LV_LOG_INFO("Received %ld bytes from %s:%d", bytes_received, s_devMasterIp, sender_port);
            }
            // printf("%s\n", g_ui_json_buffer);

            return ioc_ui_json_parse(g_ui_json_buffer);
        }
#else
        if(FD_ISSET(g_ui_json_eventfd, &readfds))
        {
            uint64_t notify;
            read(g_ui_json_eventfd, &notify, sizeof(notify));
            return ioc_ui_json_parse(g_ui_json_buffer);
        }
#endif
    }

    return s_ui_json_changed;
}


void ioc_ui_json_set_active_idx(int idx)
{
    // g_ui_json.active_connector_idx = 0;
    // return;
    // LV_LOG_WARN("====idx[%d] active_idx[%d]", idx, g_ui_json.active_connector_idx);

    if (idx != g_ui_json.active_connector_idx)
    {
        LV_LOG_WARN("active_idx %d->%d", g_ui_json.active_connector_idx, idx);
        g_ui_json.active_connector_idx = idx;

        s_ui_json_changed = true;
    }
}

void ioc_ui_json_handle_over(void)
{
    s_ui_json_changed = false;
}

void ioc_ui_overtime_changed_page(void)
{
    s_ui_json_changed = true;
}

bool ioc_conntype_isAC(int idx)
{
    return ((UI_CONNECTOR_CONN_TYPE(idx) == HWCFG_CONN_TYPE_GBT_AC) || ((UI_CONNECTOR_CONN_TYPE(idx) >= HWCFG_CONN_TYPE_SAE_AC) && (UI_CONNECTOR_CONN_TYPE(idx) <= HWCFG_CONN_TYPE_SCHUKO)));
}
