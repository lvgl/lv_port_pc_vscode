#include "ioc_app.h"

/**
 * 将字符串转换为枚举值
 * @param value 要转换的字符串
 * @param enum_values 枚举字符串数组
 * @param enum_num 枚举值的数量
 * @return 如果找到匹配的字符串，返回对应的枚举值；否则返回 -1
 */
int ioc_ui_json_string2enum(const char *value, const char **enum_values, int enum_num)
{
    for (int i = 0; i < enum_num; i++)
    {
        if (strcmp(value, enum_values[i]) == 0)
        {
            return i; // 找到匹配的字符串，返回对应的枚举值
        }
    }
    return -1; // 没有找到匹配的字符串
}

/**
 * 将枚举值转换为字符串
 * @param enum_value 要转换的枚举值
 * @param enum_values 枚举字符串数组
 * @param enum_num 枚举值的数量
 * @return 如果枚举值有效，返回对应的字符串；否则返回 NULL
 */
const char *ioc_ui_json_enum2string(int enum_value, const char **enum_values, int enum_num)
{
    if (enum_value >= 0 && enum_value < enum_num)
    {
        return enum_values[enum_value]; // 返回对应的字符串
    }
    return NULL; // 枚举值无效
}

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
const char * ioc_ui_json_get_conn_type_name(int type)
{
    return ioc_ui_json_enum2string(type, c_connectorName, CONN_TYPE_VALID + 1);
}

const char *c_names_ioc_ui_state_t[UI_CONNECTOR_STATE_MAX] = {
    "Idle",
    "Idle",
    "Starting",
    "Starting",
    "Charging",
    "Finished",
    "Reserved",
    "Stopping",
    "Error",
    "Fault",
    "Unavailable",
    "Starting",
    "Stopping",
    "Waiting",
    "Suspend",
    "Starting",
};
const char *ioc_ui_json_get_state_name(int state)
{
    return ioc_ui_json_enum2string(state, c_names_ioc_ui_state_t, UI_CONNECTOR_STATE_MAX);
}


// TODO 枪图标需要整理一下
#if (COMPILER_FOR == COMPILE_FOR_DC_AM3354)
const char * ui_conn_type_img(Charger_CONNType_t connType)
{
    switch (connType)
    {
    case HWCFG_CONN_TYPE_CHADEMO_DC:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("chademo_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("chademo_2connect");
            }
            else {
                return ioc_theme_getimgpath("chademo");
            }
        }
    case HWCFG_CONN_TYPE_GBT_DC:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("gb-dc_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("gb-dc_2connect");
            }
            else {
                return ioc_theme_getimgpath("gb-dc");
            }
        }
    case HWCFG_CONN_TYPE_SAE_DC:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("j1772_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("j1772_2connect");
            }
            else {
                return ioc_theme_getimgpath("j1772");
            }
        }
    case HWCFG_CONN_TYPE_GBT_AC:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("gb-ac_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("gb-ac_2connect");
            }
            else {
                return ioc_theme_getimgpath("gb-ac");
            }
        }
    case HWCFG_CONN_TYPE_IEC_DC:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("ccs2-dc_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("ccs2-dc_2connect");
            }
            else {
                return ioc_theme_getimgpath("ccs2-dc");
            }
        }
    case HWCFG_CONN_TYPE_SAE_AC:
    case HWCFG_CONN_TYPE_IEC_AC:
    case HWCFG_CONN_TYPE_GBT_AC_SOCK:
    case HWCFG_CONN_TYPE_SAE_AC_SOCK:
    case HWCFG_CONN_TYPE_IEC_AC_SOCK:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("j1772_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("j1772_2connect");
            }
            else {
                return ioc_theme_getimgpath("j1772");
            }
        }
    case HWCFG_CONN_TYPE_SCHUKO:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("europe_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("europe_2connect");
            }
            else {
                return ioc_theme_getimgpath("europe");
            }
        }
    case HWCFG_CONN_TYPE_NACS_DC:
        {
            if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_SINGLE ) {
                return ioc_theme_getimgpath("gb-t-ac_1connect");
            }
            else if ( UI_NUMOFCONNECTOR == CHARGER_GUN_TYPE_DUAL ) {
                return ioc_theme_getimgpath("gb-t-ac_2connect");
            }
            else {
                return ioc_theme_getimgpath("gb-t-ac");
            }
        }
    default:
        return NULL;
    }
}
#elif(COMPILER_FOR == COMPILE_FOR_AC_SSD202)
const char * ui_conn_type_img(Charger_CONNType_t connType, bool is_gun)
{
    switch (connType)
    {
    case HWCFG_CONN_TYPE_CHADEMO_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("chademo_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("chademo_2connect");
            }
        }
    case HWCFG_CONN_TYPE_GBT_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("gb-dc_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("gb-dc_2connect");
            }
        }
    case HWCFG_CONN_TYPE_SAE_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("ccs1_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("ccs1_2connect");
            }
        }
    case HWCFG_CONN_TYPE_GBT_AC:
    case HWCFG_CONN_TYPE_GBT_AC_SOCK:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("gb-ac_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("gb-ac_2connect");
            }
        }
    case HWCFG_CONN_TYPE_IEC_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("ccs2-dc_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("ccs2-dc_2connect");
            }
        }
    case HWCFG_CONN_TYPE_SAE_AC:
    case HWCFG_CONN_TYPE_SAE_AC_SOCK:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("j1772_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("j1772_2connect");
            }
        }
    case HWCFG_CONN_TYPE_IEC_AC:
    case HWCFG_CONN_TYPE_IEC_AC_SOCK:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("gb-ac_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("gb-ac_2connect");
            }
        }
    case HWCFG_CONN_TYPE_SCHUKO:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("europe_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("europe_2connect");
            }
        }
    case HWCFG_CONN_TYPE_NACS_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("nacs_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("nacs_2connect");
            }
        }
    default:
        return NULL;
    }
}
#else
const char * ui_conn_type_img(Charger_CONNType_t connType, bool is_gun)
{
    switch (connType)
    {
    case HWCFG_CONN_TYPE_CHADEMO_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("chademo_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("chademo_2connect");
            }
        }
    case HWCFG_CONN_TYPE_GBT_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("gb-dc_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("gb-dc_2connect");
            }
        }
    case HWCFG_CONN_TYPE_SAE_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("j1772_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("j1772_2connect");
            }
        }
    case HWCFG_CONN_TYPE_GBT_AC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("gb-ac_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("gb-ac_2connect");
            }
        }
    case HWCFG_CONN_TYPE_IEC_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("ccs2-dc_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("ccs2-dc_2connect");
            }
        }
    case HWCFG_CONN_TYPE_SAE_AC:
    case HWCFG_CONN_TYPE_IEC_AC:
    case HWCFG_CONN_TYPE_GBT_AC_SOCK:
    case HWCFG_CONN_TYPE_SAE_AC_SOCK:
    case HWCFG_CONN_TYPE_IEC_AC_SOCK:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("j1772_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("j1772_2connect");
            }
        }
    case HWCFG_CONN_TYPE_SCHUKO:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("europe_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("europe_2connect");
            }
        }
    case HWCFG_CONN_TYPE_NACS_DC:
        {
            if ( is_gun == false ) {
                return ioc_theme_getimgpath("gb-t-ac_1connect");
            }
            else if ( is_gun == true ) {
                return ioc_theme_getimgpath("gb-t-ac_2connect");
            }
        }
    default:
        return NULL;
    }
}
#endif


const char * c_LanguageName[] =
{
    "en-US",    // 英语
    "fr-FR",    // 法语
    "de-DE",    // 德语
    "es-ES",    // 西班牙语
    "it-IT",    // 意大利语
    "pt-PT",    // 葡萄牙语
    "ru-RU",    // 俄语
    "",         // 捷克语
    "fi-FI",     // 芬兰语
    "",         // 简体中文
    "zh-TW",    // 繁体中文(台湾)
    "",         // 罗马尼亚语
    "",         // 泰国语
    "",         // 阿拉伯语
    "",         // 土耳其语
    "pl-PL",    // 波兰语
    "nl-NL",    // 荷兰语
    "",         // 阿尔巴尼亚语
    "",         // 希伯来语
    "el-GR",    // 希腊语
    "nb-NO",    // 挪威语
};


const char * c_stateName_CHAdeMO[M1768_STATE_CHADEMO_NUM] ={
    "CHADEMO_IDLE",
    "CHADEMO_START",
    "CHADEMO_INFORMATION_EXCHANGE",
    "CHADEMO_LOCK",
    "CHADEMO_PREPARING",
    "CHADEMO_CHARGING",
    "CHADEMO_BEGIN_TERM",
    "CHADEMO_TERMING",
    "CHADEMO_UNLOCK",
    "CHADEMO_END",
    "CHADEMO_FAULT",
};
const char * c_stateName_GBT[M1768_STATE_GBT_NUM] ={
    "GBT_IDLE",
    "GBT_START",
    "GBT_SELFCHECK",
    "GBT_HANDSHAKE",
    "GBT_PRRAMETER",
    "GBT_CHARGING",
    "GBT_STARTTERMING",
    "GBT_TERMING",
    "GBT_END",
    "GBT_FAULT",
};
const char * c_stateName_CCS[M1768_STATE_GBT_NUM] ={
    "CCS_IDLE",
    "CCS_START",
    "CCS_SELFCHECK",
    "CCS_HANDSHAKE",
    "CCS_PRRAMETER",
    "CCS_CHARGING",
    "CCS_STARTTERMING",
    "CCS_TERMING",
    "CCS_END",
    "CCS_FAULT",
};



const char * const ioc_ui_connector_state_str[UI_CONNECTOR_STATE_MAX] = {
    [UI_CONNECTOR_STATE_HOME]          = "HOME",
    [UI_CONNECTOR_STATE_IDLE]          = "IDLE",
    [UI_CONNECTOR_STATE_STARTING1]     = "STARTING1",
    [UI_CONNECTOR_STATE_STARTING2]     = "STARTING2",
    [UI_CONNECTOR_STATE_CHARGING]      = "CHARGING",
    [UI_CONNECTOR_STATE_FINISHED]      = "FINISHED",
    [UI_CONNECTOR_STATE_RESERVED]      = "RESERVED",
    [UI_CONNECTOR_STATE_SESSION_ERROR] = "SESSION_ERROR",
    [UI_CONNECTOR_STATE_START_ERROR]   = "START_ERROR",
    [UI_CONNECTOR_STATE_FAULT]         = "FAULT",
    [UI_CONNECTOR_STATE_UNAVAILABLE]   = "UNAVAILABLE",
    [UI_CONNECTOR_STATE_CONNECTING]    = "CONNECTING",
    [UI_CONNECTOR_STATE_STOPPING]      = "STOPPING",
    [UI_CONNECTOR_STATE_WAIT_IN_LINE]  = "WAIT_IN_LINE",
    [UI_CONNECTOR_STATE_SUSPEND]        = "SUSPEND",
    [UI_CONNECTOR_STATE_PROCESSING]    = "PROCESSING",
};
