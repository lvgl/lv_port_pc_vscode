#ifndef __CHARGER_ERROR_CODE_H__
#define __CHARGER_ERROR_CODE_H__

#ifdef __cplusplus
extern "C" {
#endif
// 返回值标准定义 U32
// 最高位1表示错误，0表示无错误
// 31   30-24         23-16         15-0
// Err  Module Main   Module Sub    ErrorCode

// 所有模块的错误码在本文件定义，不允许私自定义
// ErrorCode总共16位，其中0-255保留给通用，各模块自定义errorCode从256开始编号
// Debug模块为每个类型的枚举设定一个标准的字符串数组用于描述故障

#define GENMOD_ERROR_SUCCESS_BASE   0
#define GENMOD_ERROR_FAIL_BASE      16
#define SUBMOD_ERROR_CODE_BASE      256

// SUCCESS CODE 只能在Charger_Errcode_GeneralSuc_t 中间选择，最多16个
#define RTN_SUC(SUCCODE)        (((Charger_U32)0 << 31) | ((Charger_U32)MOD << 24) | ((Charger_U32)SUBMOD << 16) | ((Charger_U32)SUCCODE << 0))

// ERROR CODE 可以在Charger_Errcode_GeneralFail_t 和各个子模块类似Charger_Errcode_1768Fail_t 中选取
#define RTN_ERR(ERRCODE)        (((Charger_U32)1 << 31) | ((Charger_U32)MOD << 24) | ((Charger_U32)SUBMOD << 16) | ((Charger_U32)ERRCODE << 0))

#define IS_RTNSUC(CODE)         (((Charger_U32)(CODE)&0x80000000) == 0x00000000)
//#define IS_RTNERR(CODE)         (((Charger_U32)(CODE)&0x80000000) == 0x80000000)
#define ERRCOEE(CODE)           ((Charger_U32)(CODE)&0xFF)

typedef enum
{
    DEBUG_LEVEL_NONE = 0,
    DEBUG_LEVEL_ERROR,
    DEBUG_LEVEL_WARN,
    DEBUG_LEVEL_KEYI,
    DEBUG_LEVEL_INFO,

    DEBUG_LEVEL_NUM
} Debug_Level_t;

#include "zlog.h"

extern zlog_category_t *c_charger;
extern zlog_category_t *c_hex;
#if 1
#define ESC_START       "\033["
#define ESC_END         "\033[0m"
// #define COLOR_FATAL     "31;40;5m"
// #define COLOR_ALERT     "31;40;1m"
// #define COLOR_CRIT      "31;40;1m"
#define COLOR_ERROR     "0;33m"
#define COLOR_WARN      "0;36m"
#define COLOR_NOTICE    "0m"
#define COLOR_INFO      "0m"
// #define COLOR_DEBUG     "36;40;1m"
// #define COLOR_TRACE     "37;40;1m"
#else
#define ESC_START
#define ESC_END
// #define COLOR_FATAL
// #define COLOR_ALERT
// #define COLOR_CRIT
#define COLOR_ERROR
#define COLOR_WARN
#define COLOR_NOTICE
#define COLOR_INFO
// #define COLOR_DEBUG     "36;40;1m"
// #define COLOR_TRACE     "37;40;1m"
#endif

int Charger_zlog_init();
void Charger_zlog_release();

#define C_ZLOG_FATAL(format, args...) zlog_fatal(c_charger, ESC_START COLOR_ERROR format ESC_END, ##args)
#define C_ZLOG_ERR(format, args...) zlog_error(c_charger, ESC_START COLOR_ERROR format ESC_END, ##args)
#define C_ZLOG_WARN(format, args...) zlog_warn(c_charger, ESC_START COLOR_WARN format ESC_END, ##args)
#define C_ZLOG_NOTICE(format, args...) zlog_notice(c_charger, ESC_START COLOR_NOTICE format ESC_END, ##args)
#define C_ZLOG_INFO(format, args...) zlog_info(c_charger, ESC_START COLOR_INFO format ESC_END, ##args)
#define C_ZLOG_DEBUG(format, args...) zlog_debug(c_charger, ESC_START COLOR_INFO format ESC_END, ##args)

#define CHARGER_DEBUG_FATAL(_X_)   C_ZLOG_FATAL _X_
#define CHARGER_DEBUG_ERROR(_X_)   C_ZLOG_ERR _X_
#define CHARGER_DEBUG_WARN(_X_)    C_ZLOG_WARN _X_
#define CHARGER_DEBUG_KEYI(_X_)    C_ZLOG_NOTICE _X_
#define CHARGER_DEBUG_INFO(_X_)    C_ZLOG_INFO _X_
#define CHARGER_DEBUG_DEBUG(_X_)   C_ZLOG_DEBUG _X_
#define CHARGER_DEBUG_DUMPDATA(_X_,_L_) hzlog_notice(c_hex, _X_,_L_)


#define DEBUG_ERR(ERRCODE,FMT,ARG...) CHARGER_DEBUG_ERROR((#ERRCODE " " FMT"", ##ARG))
#define DEBUG_WARN(FMT,ARG...)  CHARGER_DEBUG_WARN((FMT"", ##ARG))
#define DEBUG_KEYI(FMT,ARG...) CHARGER_DEBUG_KEYI((FMT"", ##ARG))
#define DEBUG_INFO(FMT,ARG...)  CHARGER_DEBUG_INFO((FMT"", ##ARG))
#define DEBUG_BUFF(BUFF,LEN)    CHARGER_DEBUG_DUMPDATA(BUFF,LEN)

#define UDP_BLUETOOTH_PORT      23460       // 蓝牙通讯端口, 蓝牙Python模块固定向此端口发送指令
#define UDP_SELFCHECK_HOST_PORT 23459       // selfCheck默认发送的PC工具端口
#define UDP_SELFCHECK_PORT      23458       // selfCheck程序绑定端口, 接收PC工具指令
#define UDP_COMM7628_PORT       23457       // 7628-103 UDP通信, 7628端固定绑定端口
#define UDP_RO_PORT             23456       // RO端口, 接收激活工具应答
#define UDP_CONFIG_PORT         23455       // UdpConfig端口
#define UDP_PRINT_PORT          23454       // 103主板UDP打印端口
#define UDP_HUB_CT_PORT         22405       // hub上报

#define MODBUS_TCP_DEFAULT_LISTEN_PORT  22401
#define MODBUS_GROUP_DEFAULT_LISTEN_PORT  10000 // master组广播基本端口 实际端口+组号

#define SENDUI_AM3354_SRC_PORT	20000       // 7628-LCD通讯端口, 7628/3354模块固定绑定此端口
#define SENDUI_LCD3354_DST_PORT	20001       // 7628-LCD通讯端口, LCD模块固定绑定此端口
#define UDP_CLIENT_PORT         13202       // 7628-103 UDP通信, 103端固定绑定端口

#define VPOS_TCP_PORT 			33401       // POS机 TCP通讯端口

#define OPEN_DOOR_CHECK_PROT    11000       // 接收wifi模块上报的的开门检测信息

#define DEV_AUTO_GROUP_PORT     12000       // 外设自组网端口

typedef enum
{
    MOD_GENERAL = 0,
    MOD_SYSCONFIG,
    MOD_HAL,
    MOD_DRVBOARD,
    MOD_DRVPOINT,
    MOD_1768,
    MOD_CTRL,

    MOD_NUM
} Charger_ModuleLevel_t;

typedef enum
{
    MOD_GENERAL_MAIN = 0,

    MOD_GENERAL_NUM
} Charger_ModGeneral_SubMod_t;

typedef enum
{
    MOD_SYSCONFIG_MAIN = 0,

    MOD_SYSCONFIG_NUM
} Charger_ModSysConfig_SubMod_t;

typedef enum
{
    MOD_HAL_MAIN = 0,

    MOD_HAL_NUM
} Charger_ModHAL_SubMod_t;

typedef enum
{
    MOD_DRVBOARD_MAIN = 0,
    MOD_DRVBOARD_BASE,
    MOD_DRVBOARD_GPIO,
    MOD_DRVBOARD_ADC,
    MOD_DRVBOARD_RTC,
    MOD_DRVBOARD_PWM,
    MOD_DRVBOARD_TIM,
    MOD_DRVBOARD_SPI,
    MOD_DRVBOARD_UART,
    MOD_DRVBOARD_WDG,
	MOD_DRVBOARD_JDQ,
    MOD_DRVBOARD_RCD,
    MOD_DRVBOARD_UDP,

    MOD_BOARD_NUM
} Charger_ModDrvBoard_SubMod_t;

typedef enum
{
    MOD_DRVPOINT_MAIN = 0,
    MOD_DRVPOINT_BUTTON,
    MOD_DRVPOINT_BEEP,
    MOD_DRVPOINT_EMERGENCY,
    MOD_DRVPOINT_FLASHEX,
    MOD_DRVPOINT_METER,
    MOD_DRVPOINT_RF,
    MOD_DRVPOINT_CONNECTOR,
    MOD_DRVPOINT_ETHERNET,
    MOD_DRVPOINT_4G,
    MOD_DRVPOINT_WIFI,
    MOD_DRVPOINT_LED,
    MOD_DRVPOINT_LCD,
    MOD_DRVPOINT_DEBUG,
    MOD_DRVPOINT_TF,
    MOD_DRVPOINT_WEBSOCKET,
    MOD_DRVPOINT_HTTPFTP,
    MOD_DRVPOINT_IPCOM,
    MOD_DEVPOINT_MQTT,

    MOD_DRVPOINT_NUM
} Charger_ModDrvPoint_SubMod_t;

typedef enum
{
    MOD_1768_MNGR = 0,
    MOD_1768_UPG,

    MOD_1768_NUM
} Charger_Mod1768_SubMod_t;

enum
{
    MOD_CTRL_A_SECURITY = 0,
    MOD_CTRL_B_PROVISIONING,
    MOD_CTRL_C_AUTHORIZATION,
    MOD_CTRL_D_LOCALAUTH,
    MOD_CTRL_E_TRANSACTION,
    MOD_CTRL_F_REMOTECONTROL,
    MOD_CTRL_G_AVAILABLITY,
    MOD_CTRL_H_RESERVATION,
    MOD_CTRL_I_TARIFFCOST,
    MOD_CTRL_J_METERVALUES,
    MOD_CTRL_K_SMARTCHARING,
    MOD_CTRL_L_FIRMWAREMNGR,
    MOD_CTRL_M_ISO15118,
    MOD_CTRL_N_DIAGNOSTICS,
    MOD_CTRL_O_DISPLAYMESSAGE,
    MOD_CTRL_P_DATATRANSFER,

    MOD_CTRL_0_CTRL = 0x30,
    MOD_CTRL_0_CTRLMAIN,
    MOD_CTRL_1_STATEMNGR,
    MOD_CTRL_2_RESMNGR,
    MOD_CTRL_3_LCD,
    MOD_CTRL_3_UI,
    MOD_CTRL_4_OCPP,
    MOD_CTRL_5_MODBUSTCP,

    MOD_CTRL_STORAGE_OCPP20,

    MOD_CTRL_NUM_OCPP20
};

enum
{
    MOD_CTRL_MAIN = 0,
    MOD_CTRL_CTRL,
    MOD_CTRL_STORAGE,
    MOD_CTRL_AUTH,
    MOD_CTRL_RECORDS,
    MOD_CTRL_SESSION,
    MOD_CTRL_RESERVATION,
    MOD_CTRL_STATEMNGR,
    MOD_CTRL_SMARTCHARGING,
    MOD_CTRL_OCPP,
    MOD_CTRL_UPGRADE,
    MOD_CTRL_STATUSNOTIFY,
    MOD_CTRL_DIAGNOSTICS,
    MOD_CTRL_DATATRANSFER,
    MOD_CTRL_RESMNGR,
    MOD_CTRL_SETPLAN,
    MOD_CTRL_MODBUS,
    MOD_CTRL_VPOS,
    MOD_CTRL_MQTT,

    MOD_CTRL_NUM_OCPP16
};

typedef enum
{
    S_OK = GENMOD_ERROR_SUCCESS_BASE,
    S_WAITING,

    GENERAL_S_NUM
} Charger_Errcode_GeneralSuc_t;

typedef enum
{
    E_FAILED = GENMOD_ERROR_FAIL_BASE,
    E_BAD_PARAM,
    E_NOT_FOUND,
    E_TIMEOUT,
    E_OVERFLOW,
    E_OUT_OF_RANGE,
    E_NO_ENOUGH_MEMORY,
    E_NO_AUTH,

    GENERAL_F_NUM
} Charger_Errcode_GeneralFail_t;

typedef enum
{
    E_SYSCONFIG_UNMATCH = SUBMOD_ERROR_CODE_BASE,
    E_SYSCONFIG_CHECKSUM,

    E_SYSCONFIG_NUM
} Charger_Errcode_SysconfigFail_t;



typedef enum
{
    E_DRVBOARD_HAL_ERROR = SUBMOD_ERROR_CODE_BASE,

    E_DRVBOARD_NUM
} Charger_Errcode_DrvBoardFail_t;

typedef enum
{
    E_CP_UNPLUGGED = SUBMOD_ERROR_CODE_BASE,
    E_CP_UNKNOWN,
    E_CC_UNPLUGGED,
    E_CC_UNKNOWN,
    E_K1K2_FAULT,

    E_1768_NUM
} Charger_Errcode_1768Fail_t;


typedef enum
{
    E_NORMAL_FAIL = SUBMOD_ERROR_CODE_BASE,

    E_LOCAL_LIST_CONFLICT_REPORT, //LocalListConflict

    E_1768_REJECT,
    E_STATE_CONFLICT,

    E_RECORDS_CONFLICT,

    E_AUTH_NUM
} Charger_Errcode_CtrlFail_t;


#ifndef OCPP20_IMPL
extern const char * c_ChargePointStatusName[];
extern const char * c_ChargePointErrorCodeName[];

extern const char * c_1768_stateName[];

extern const char * c_ocppStateNames[];
extern const char * c_csCmdNames[];
extern const char * c_scCmdNames[];
extern const char * c_callErrorNames[];

extern const char * s_stateMngr_stateName[];
#endif

extern const char* c_cpStateName[];
extern const char* c_ccStateName[];

extern const char * c_connectorName[];
extern const char * c_lockName[];
extern const char * c_meterName[];
extern const char * c_relayName[];

extern const char ** M1768_GetStateNameArray(int connIdx);
extern const char * c_connStatusName[];

#ifndef OCPP20_IMPL
extern const char * c_c2s_notifyStatusName[];

extern const char * c_ChargingProfilePurposeTypeName[];
extern const char * c_ChargingProfileKindTypeName[];
extern const char * c_ChargingRateUnitTypeName[];
extern const char * c_RecurrencyKindTypeName[];
#endif

extern const char * c_stateName_CHAdeMO[];
extern const char * c_stateName_GBT[];
extern const char * c_stateName_CCS[];
extern const char * c_stateName_AC[];
extern const char * c_connStatusName[];

extern const char * c_1768_stopReasonName[];
extern const char * c_1768_commonErrorName[];

extern const char *c_EcoModeName[];

extern const char * c_CCNAME[];
extern const char * c_CPNAME[];

extern const char * c_CurrLimitSourceName[];
extern const char * c_CurrLimitTargetName[];

extern const char * c_modbus_protocolName[];

#ifdef __cplusplus
}
#endif
#endif

