#ifndef __UI_JSON_H__
#define __UI_JSON_H__

#include "const_name.h"

#define BOARD_MAX_CONNECTOR_NUM     24      // 分体桩最多可能支持24个枪

#if (COMPILER_FOR == COMPILE_FOR_DC_AM3354)
#define UI_MAX_CONNECTOR_NUM    4
#else
#define UI_MAX_CONNECTOR_NUM    2
#endif

#define UIMSG_ACTIVE_IDX			"activeConnIdx"			// 界面切换枪号
#define UIMSG_USE_CREDITCARD		"useCreditCard"			// 用户选择信用卡
#define PREAUTH_AMOUNT				"PreAuth"				// 预授权额度
#define STOP_PLUG_CHARGING			"stopPlugCharging"		// 停止插枪即充启动的充电进程
#define START_CHARGING				"startCharging"			// 开始一个无卡充电

#define UI_JSON_I_UNIT  (0.1)
#define UI_JSON_V_UNIT  (0.1)
#define UI_JSON_Q_UNIT  (0.001)
#define UI_JSON_P_UNIT  (0.001)
#define UI_JSON_Q_4_DOT_UNIT  (0.0001)

#define IOC_PATH                "/tmp/Ioc/"

#define UI_JSON_BUFFER_SIZE (1024 * 16 * 6)
extern char g_ui_json_buffer[UI_JSON_BUFFER_SIZE];
extern int g_ui_json_eventfd ;
typedef enum
{
    TEST_READ,
    TEST_GREEN,
    TEST_BULE,

    TEST_COLOR_NUM
}trigger_test_color_type;
// -------------------- M1768相关 --------------------

typedef enum
{
	M1768_STATE_AC_1_Idle = 0,						// 1. 插枪 12V->9V (这一步不在1768监控范围内), 1768接收到启动命令后跳转到状态2
	M1768_STATE_AC_2_SelfCheckLock,					// 2. 桩自检并锁枪，成功则输出PWM 9V->9V PWM，进入状态3
	M1768_STATE_AC_3_Wait_S2_Close,					// 3. 桩进入等待S2状态, 车检测PWM后闭合S2，9V PWM-> 6V PWM， 此过程无超时检测，车可以在任意上时刻进入S2闭合，检测到之后进入状态4 SUSPENDEV状态开始
	M1768_STATE_AC_4_K1K2_ON,						// 4. 桩闭合K1K2（3秒内）, 6V PWM + K1K2闭合，等待实际电流输出，成功则进入状态5 离开SUSPENDEV状态
	M1768_STATE_AC_5_Charging,						// 5. 车启动充电，桩进入充电监控状态, 6V PWM + K1K2闭合 + 实际电流输出
	M1768_STATE_AC_V3_EV_STOP,						// V3. 桩检测到S2断开需要在100ms内打开K1K2，并保持PWM输出，9V PWM + K1K2闭合 + 小或无电流输出 -> 9V PWM + K1K2打开, 然后不等待，进入状态3
	M1768_STATE_AC_U1_EVSE_STOP,					// U1. 桩切换PWM到+12V 状态(关PWM，实际因为枪连接着，输出不是12V), 等待车响应并打开S2, 6V PWM + K1K2闭合 + 实际电流输出 -> 6V + K1K2闭合 + 实际电流输出
	M1768_STATE_AC_U2_K1K2_OFF,						// U2. 桩打开K1K2(S2断开后的100ms内), 9V(6V) + K1K2闭合 + 小或无电流输出 -> 9V(6V) + K1K2打开
	M1768_STATE_AC_U3_Unlock,						// U3. 桩在100ms内开始解锁，5秒内完成解锁，9V ，解锁成功进入U4，即1状态
	M1768_STATE_AC_END,
	M1768_STATE_AC_E_FAULT,							// 桩故障状态

	M1768_STATE_AC_NUM
} M1768_ConnState_AC_t;

// ======= 子阶段, 用于lcd显示
// 握手阶段
typedef enum {
	RNL_SHAKEHANDS_WAIT_BHM = 1,
	RNL_SHAKEHANDS_INSU,
	RNL_SHAKEHANDS_WAIT_BRM,
	RNL_SHAKEHANDS_WAIT_BCP
} Charger_RNL_Shakehands_t;

// 参数配置阶段
typedef enum {
	RNL_PRECHARGE_WAIT_BRO = 1,
	RNL_PRECHARGE_INSU,
	RNL_PRECHARGE_WAIT_BCL,
	RNL_PRECHARGE_WAIT_K1K2,
	RNL_PRECHARGE_WAIT_BCS,
} Charge_RNL_PreCharge_t;

// 握手阶段
typedef enum {
	GBT_SHAKEHANDS_WAIT_BHM = 1,
	GBT_SHAKEHANDS_INSU,
	GBT_SHAKEHANDS_WAIT_BRM,
	GBT_SHAKEHANDS_WAIT_BCP
} Charge_Shakehands_t;

// 参数配置阶段
typedef enum {
	GBT_PRECHARGE_WAIT_BRO = 1,
	GBT_PRECHARGE_CHKBAT,
	GBT_PRECHARGE_WAIT_BCS,
	GBT_PRECHARGE_WAIT_K1K2
} Charge_PreCharge_t;
typedef enum
{
	M1768_STATE_GBT_IDLE = 0,
	M1768_STATE_GBT_START,
	M1768_STATE_GBT_SELF_CHECK,
	M1768_STATE_GBT_HANDSHAKE,
	M1768_STATE_GBT_PRRAMETER,
	M1768_STATE_GBT_CHARGING,
	M1768_STATE_GBT_START_TERMING,
	M1768_STATE_GBT_TERMING,
	M1768_STATE_GBT_END,
	M1768_STATE_GBT_FAULT,

	M1768_STATE_GBT_NUM
} M1768_ConnState_GBT_t;

typedef enum
{
	M1768_STATE_CHADEMO_IDLE = 0,
	M1768_STATE_CHADEMO_START,
	M1768_STATE_CHADEMO_INFORMATION_EXCHANGE,
	M1768_STATE_CHADEMO_LOCK,
	M1768_STATE_CHADEMO_PREPARING,
	M1768_STATE_CHADEMO_CHARGING,
	M1768_STATE_CHADEMO_BEGIN_TERM,
	M1768_STATE_CHADEMO_TERMING,
	M1768_STATE_CHADEMO_UNLOCK,
	M1768_STATE_CHADEMO_END,
	M1768_STATE_CHADEMO_FAULT,

	M1768_STATE_CHADEMO_NUM
} M1768_ConnState_CHAdeMO_t;

// ----------------------------------------------------------------


// 定义功能按键的功能
#define KEY_FUNC_SWITCH_OUTPUT              ((uint32_t)1 << 0)            // 调节输出电流
#define KEY_FUNC_SWTICH_DELAYTIMER          ((uint32_t)1 << 1)            // 调节延时Timer
#define KEY_FUNC_SELECT_CONN                ((uint32_t)1 << 2)            // 在A/B枪之间切换
#define KEY_FUNC_SELECT_CONN_A              ((uint32_t)1 << 3)            // 切换到A枪
#define KEY_FUNC_SELECT_CONN_B              ((uint32_t)1 << 4)            // 切换到B枪
#define KEY_FUNC_RFID_START                 ((uint32_t)1 << 5)            // 无卡启动充电
#define KEY_FUNC_RFID_STOP                  ((uint32_t)1 << 6)            // 无卡强行停止充电 （直流桩虚拟按钮强制停止各种类型启动的充电）
#define KEY_FUNC_SWITCH_LANGUAGE            ((uint32_t)1 << 7)            // 切换lcd语言
#define KEY_FUNC_SWITCH_ECOMODE             ((uint32_t)1 << 8)            // 切换负载均衡的 master.chargerMode


// -------------------- 软件功能开关 --------------------
#define SW_FUNC_LCD_IDLE_ONRF               ((uint32_t)1 << 0)           // 空闲状态下是否显示刷卡 0显示请插枪
#define SW_FUNC_HIDE_TEMP_DISPLAY           ((uint32_t)1 << 1)            // 20220316 废弃原功能, 替换为隐藏温度功能 香港客户提出的特殊需求, Smart Charging的Duration当成充电持续时间
#define SW_FUNC_CHARGE_ON_PLUG              ((uint32_t)1 << 2)           // 插枪即充
#define SW_FUNC_ICCARD_FORMAT_BIG_ENDIAN    ((uint32_t)1 << 3)           // IC卡号是否大端模式
#define SW_FUNC_TOUCH_LCD                   ((uint32_t)1 << 4)           // 是否是迪文的触摸串口屏 (由于显示相同逻辑, 只是增加了触摸功能, 顾不用RO设置, 改为软件设置)
#define SW_FUNC_ENABLE_ENGINEERING_MODE     ((uint32_t)1 << 5)           // 是否允许工程师模式
#define SW_FUNC_NOT_PERMIT_CONCURRENT_TX    ((uint32_t)1 << 6)           // 是否允许Auth返回ConnCurrentTx继续充电, 默认允许
#define SW_FUNC_OFFLINE_ADD_RFID            ((uint32_t)1 << 7)           // 是否允许离线增/删RFID
#define SW_FUNC_ENABLE_STATION_CODE         ((uint32_t)1 << 8)           // 是否启用StationCode追加在二维码中
#define SW_FUNC_ENABLE_CHECK_LOCK           ((uint32_t)1 << 9)           // 是否启用锁枪检测
#define SW_FUNC_KEEP_LOCK_FINISH            ((uint32_t)1 << 10)          // 停充的时候是否保持锁定,默认否, 如果是, 需要远程下发Unlock命令解锁
#define SW_FUNC_PREVENT_START_PE_RCD        ((uint32_t)1 << 11)          // 当因为PE/RCD故障停止充电时, 是否阻止再次启动, 默认否10秒后可以再次启动, 选为是则必须断电重启
#define SW_FUNC_QR_WIFI                     ((uint32_t)1 << 12)          // 二维码显示WiFi-AP+SN信息
#define SW_FUNC_LCD_IDLE_SHOW_WELCOME       ((uint32_t)1 << 13)          // 空闲时返回welcome界面
#define SW_FUNC_DISABLE_RECOVERY_SESSION    ((uint32_t)1 << 14)          // 异常断电是否自动恢复充电, 默认自动恢复
#define SW_FUNC_CURRLMT_REPORT              ((uint32_t)1 << 15)          // 是否上报StateMngr判断电流限制变化给后台, 默认关闭, 防止某些后台无法处理
#define SW_FUNC_METERVALUES_WITH_DEFAULT    ((uint32_t)1 << 16)          // MeterValues上报时是否上传完整内容, 默认关闭以节省流量, 打开以适应部分运营商
#define SW_FUNC_ENABLE_CCS_PLUG_CHARGE      ((uint32_t)1 << 17)          // CCS充电枪是否支持Plug & Charge功能, 当未授权且插枪时, 启动充电, 使用evccid来Authorization
#define SW_FUNC_METERVALUES_OF_CREDIT       ((uint32_t)1 << 18)          // 信用卡启动的充电不上报MeterValues, 默认不上报, 打开上报，以适应部分运营商
#define SW_FUNC_EXCLUDE_VIN_MARK_IN_IDTAG   ((uint32_t)1 << 19)          // VIN前缀不上报后台只报值部分, 默认需要上报(主流后台), 打开则不上报，以适应部分运营商
#define SW_FUNC_SUSPEND_BY_EV_STOP          ((uint32_t)1 << 20)          // 与车数字通讯(包括PLC)的情况下, EV正常停充(SoC到达)时, 进入SuspendEv状态, 默认false 进入Finished状态
#define SW_FUNC_DISABLE_STOPBTN_OF_CREDIT   ((uint32_t)1 << 21)          // 禁止按键停充信用卡启动的充电

// ---------------------------------------------------------
typedef enum
{
    IdTagType_Normal = 0,           // 普通idTag (RFID/RemoteId)

    IdTagType_LocalStart,           // 本地自动启动充电的卡号(按钮/即插充), 默认 FFFFFFFF

    IdTagType_PlugChargeDefault,    // CCS EIM(EVCCID) GBT(VIN) CCS PnC 方式启动充电时的初始默认卡号
    IdTagType_CCS_EVCCID,           // CCS EIM(EVCCID)
    IdTagType_GBT_VIN,              // GBT EIM(VIN)
    IdTagType_CCS_PnC,              // CCS PnC

    IdTagType_CreditCard,           // 信用卡启动的

    IdTagType_NUM
} IdTagType_t;
typedef enum
{
    CURRENCY_USD,
    CURRENCY_GBP,
    CURRENCY_EUR,
    CURRENCY_RUB,
    CURRENCY_CNY,
    CURRENCY_SEK,
    CURRENCY_PLN,

    CURRENCY_NUM
}CurrencyType_t;

typedef struct
{
    CurrencyType_t idx;
    char * code;
    char * icon;

}UI_CurrencyMap_t;

typedef struct
{
    uint16_t  startMin;   // 开始时间，距离00：00的分钟数
    uint16_t  endMin;     // 结束时间

    double  kWhPrice;     // 按电量的价格
    double  hourPrice;    // 按时间的价格
    double  flatFee;      // 固定费用

    char priceText[255 + 1];   // OcppPrice需要显示的默认价格文本

} UI_PriceInfo_t;

typedef struct
{
    double  totalCost;          // 总费用
    double  reserveIdleCost;    // 预约费用
    double  suspendIdleCost;    // 充电中占位费
    double  stopIdleCost;       // 充电完成后的占位费

} UI_CostInfo_t;

typedef enum
{
    PHASE_L1 = 0,
    PHASE_L2 = 1,
    PHASE_L3 = 2,

    PHASE_NUM
} PhaseNum_t;

typedef struct
{
    double             voltage[PHASE_NUM+1];   // 3相电压, 最后一个表示三相平均电压(无效相不参与计算)

    double             current[PHASE_NUM+1];   // 3相电流, 最后一个表示三相平均电流(无效相不参与计算)
    double             Q[PHASE_NUM+1];         // kWh, 最后一个表示总电能
    uint8_t            fourthQ;                // Q 的第4位小数
    uint8_t            startQ4dot;             // Q 起始的第4位小数

    double             power;                  // kW功率

}UI_MeterData_t;

typedef enum
{
    CHARGER_SCREEN_MSG_NO_CONN_SELECTED,   // Please select a connector firstly
    CHARGER_SCREEN_MSG_FAILED_TO_START_CHARGING,    //Failed to start charging
    CHARGER_SCREEN_MSG_FAULT_FOUND_STOP_CHARGING,    //Fault found, Stop Charging
    CHARGER_SCREEN_MSG_NOT_PERMIT,   //Not Permit
    CHARGER_SCREEN_MSG_ADDRFID,
    CHARGER_SCREEN_MSG_CLEARRFID,

	CHARGER_SCREEN_MSG_CCS_AUTOCHARGE_BIND,	// 提醒用户刷卡或远程启动完成自动充电

    // CHARGER_SCREEN_MSG_GROUND_FAULT,

    CHARGER_SCREEN_MSG_NONE
} Charger_Screen_Display_Msg_t;

typedef enum
{
    CHARGER_SCREEN_AUTH_FAIL_OTHER,
    CHARGER_SCREEN_AUTH_FAIL_BLOCKED,
    CHARGER_SCREEN_AUTH_FAIL_EXPIRED,
    CHARGER_SCREEN_AUTH_FAIL_INVALID,
    CHARGER_SCREEN_AUTH_FAIL_CONCURRENTTX,
    CHARGER_SCREEN_AUTH_SUCCESS,
}Charger_Screen_Auth_Reason_t;

typedef enum
{
    CHARGER_SCREEN_CREDIT_POINT,
    CHARGER_SCREEN_CREDIT_NO_CONN_SELECTED,
    CHARGER_SCREEN_CREDIT_PREAUTH_FAILED,		// 预授权失败
    CHARGER_SCREEN_CREDIT_PREAUTH_SUCCESS,	    // 预授权成功
    CHARGER_SCREEN_CREDIT_SETTLEMENT_FAILED,	// 结算失败
    CHARGER_SCREEN_CREDIT_SETTLEMENT_SUCCESS,	// 结算成功
    CHARGER_SCREEN_CREDIT_WATTING,				// "POS机正在处理请求，请等待"
    CHARGER_SCREEN_CREDIT_VPOS_BUSY,			// "POS机正在处理其他请求，请稍后再试"
    CHARGER_SCREEN_CREDIT_VPOS_FAULT,			// "POS机出现故障，请联系工作人员"
}Charger_Screen_CreditCard_t;

/*
   ***********************************************************************
   以下属于触发性命令
   {
       "Trigger" : "Downloading",       // "Cancel", "Downloading", "Installing", "Debug", "Prompt", "Auth", "Test",
                                           "Message","Notify","MessageCancel","NotifyCancel", "CreditCard"
        "percent" : 35,                 // "Downloading" "Installing" 才支持
        "enter" : true,                 // "Debug" 才支持
        "Msg" : 0,                      // Prompt => Charger_Screen_Display_Msg_t, Auth => Charger_Screen_Auth_Reason_t
                                        // Test: 0:显示红色 1:显示绿色 2:显示蓝色,  CreditCard => Charger_Screen_CreditCard_t
        "seconds" : 1,                  // 提示框显示秒数
        "Content"："1234567"            // 弹窗显示的文本内容

   }

    "Cancel": 立即清除所有提示框
    "Downloading",: 显示下载中提示框, 不调用Cancel就永远显示
    "Installing",: 显示升级中提示框, 不调用Cancel就永远显示,
    "Debug": 显示调试信息, 需要显示"homeOcpp.%04d-%02d-%02d.log"文件的内容, 实现tail -f功能
    "Prompt": 显示提示框, 对应于 Charger_Screen_Msg_Display 函数
    "Auth"：显示提示框, 对应于 Charger_Screen_Auth_Reason_Set 函数
    "Test": 显示测试界面，根据Msg内容显示 0:显示红色 1:显示绿色 2:显示蓝色
    "Message": 显示中部弹窗
    "Notify": 显示底部弹窗
    "MessageCancel": 清除中部弹窗
    "NotifyCancel": 清除底部弹窗
    "CreditCard": 信用卡提示界面

   ***********************************************************************
 */
typedef struct
{
    char trigger[16];
    unsigned short percent;
    uint8_t msg;
    int seconds;
    char content[512 + 1];
    char billUrlStr[256 + 1];
    char receiptUrlStr[256 + 1];
    int idx;
    char action[64 + 1];

}ioc_ui_trigger_t;


typedef enum {
    UI_CONNECTOR_STATE_HOME = 0,
    UI_CONNECTOR_STATE_IDLE,
    UI_CONNECTOR_STATE_STARTING1,
    UI_CONNECTOR_STATE_STARTING2,
    UI_CONNECTOR_STATE_CHARGING,
    UI_CONNECTOR_STATE_FINISHED,
    UI_CONNECTOR_STATE_RESERVED,
    UI_CONNECTOR_STATE_SESSION_ERROR,
    UI_CONNECTOR_STATE_START_ERROR,
    UI_CONNECTOR_STATE_FAULT,
    UI_CONNECTOR_STATE_UNAVAILABLE,
    UI_CONNECTOR_STATE_CONNECTING,
    UI_CONNECTOR_STATE_STOPPING,
    UI_CONNECTOR_STATE_WAIT_IN_LINE,
    UI_CONNECTOR_STATE_SUSPEND,
    UI_CONNECTOR_STATE_PROCESSING,

    UI_CONNECTOR_STATE_MAX,
} ioc_ui_connector_state_t;

typedef enum
{
	STARTING_HANDSHAKE = 0,
	STARTING_INSULATION,
	STARTING_PARAMTETER,
	STARTING_PRECHARGE,

    STARTING_AC,   // AC充电无子状态

	STARTING_NUM,

} Starting_SubStep_t;

typedef struct
{
    Charger_CONNType_t conn_type;
    int rated_power;

    ioc_ui_connector_state_t state;      // 根据UI.json计算出当前枪应该显示哪个状态
    bool showPrice;                      // 是否显示价格
    bool isOcppPrice;                    // 是否是OcppPrice体系，如果是的话默认价格显示文本
    bool isQ4dot;                        // 电量显示四位小数
    struct
    {
        struct {
            UI_PriceInfo_t  currPrice;  // 当前价格
            UI_PriceInfo_t  nextPrice;  // 下一时段价格

        } idle;

        struct {
            UI_PriceInfo_t  currPrice;
            UI_PriceInfo_t  nextPrice;
            char connQR[255+1];

        } starting;

        struct {
            UI_PriceInfo_t  currPrice;  // 当前价格
            UI_CostInfo_t   cost;       // 费用信息
            UI_MeterData_t  meterData;  // 电表数据
            int soc;
            time_t startTime;           // 起充时间

        } charging;

        struct {
            UI_CostInfo_t   cost;       // 费用信息
            double  actualMeter;        // 充电电量
            uint8_t   fourthQ;         // actualMeter 的第4位小数
            int duration;               // 充电时长
            int startSoc;               // 起充Soc
            int stopSoc;                // 停止Soc
            time_t  startTime;          // 开始启动充电的时间
            time_t  stopTime;           // 结束充电的时间

        }finished;

        struct {
            UI_PriceInfo_t  currPrice;
            UI_PriceInfo_t  nextPrice;
            time_t reverseTime;         // 预约结束的时间点

        }reserved;

    } state_data;

    char  _status_real[20+1];
    char  stateMngrInfo_runState[20+1];
    char  m1768Info_runState[50+1];
    uint16_t m1768errCode;
    uint8_t  stopCode;
    char connStatus[20+1];
    char idTag[20+1];
    char _errCode[50+1];
    char connCode[20+1];
    IdTagType_t idType;
    bool plug; //  是否插枪,用于保存插枪状态

    // charging detail data
    uint8_t subState;
    int startSoc;
    double reqVol;
    double reqCur;
    int    temp;
    double offerVol;
    double offerCur;
    double outLimit;
    char outLimitUnit[5+1];
    int compGroup;

    Starting_SubStep_t startingSubStep;

} ioc_ui_json_connector_t;

typedef enum {
    CHARGER_GUN_TYPE_SINGLE = 1,   // 单枪
    CHARGER_GUN_TYPE_DUAL   = 2,   // 双枪
    CHARGER_GUN_TYPE_TRIPLE = 3,   // 三枪
    CHARGER_GUN_TYPE_QUAD   = 4    // 四枪
} ChargerGunType_t;


typedef struct
{
    uint8_t location;   // 屏幕在哪个终端上
    uint8_t connNum;    // 屏幕显示多少根枪
    uint8_t connIdx[UI_MAX_CONNECTOR_NUM];  // 屏幕显示哪几根枪
    uint8_t autGroup;    // 屏幕属于哪个授权组

}ioc_ui_dispenser_config_t;

typedef struct
{
    time_t           activeTime;                     // 桩下发激活的时间戳

    uint8_t          brightIdle;                     // 待机状态亮度, 0-5, 0全黑, 5最亮
    uint8_t          brightActive;                   // 激活状态亮度, 1-5, 1最暗, 5最亮
    uint8_t          brightCharging;                 // 充电状态亮度, 1-5, 1最暗, 5最亮
    uint8_t          timeoutActive;                  // 激活->待机状态超时时间, 1-5分钟, 充电中常亮

} ioc_ui_biright_t;

typedef struct
{
    bool meterFault;
    bool rfFault;
    bool online;
    uint8_t ethStatus;
    uint8_t wifiStatus;
    uint8_t m4gStatus;

    char httpAddr[100 + 1];

}ioc_ui_drvPointInfo_t;


# define KEY_FUNC_COUNT 3
typedef struct
{
    ioc_ui_trigger_t trigger;

    Language_t language;

    ioc_ui_json_connector_t connectors[UI_MAX_CONNECTOR_NUM];
    int connector_num;

    int active_connector_idx;
    bool enPOS;     // 是否启用了POS机
    bool enSoftKey; // 是否启用了软键盘
    uint8_t timeZone; // 配置的时区

    char currencyIcon[3 + 1];        // 货币单位图标
    char currencyCode[3 + 1];        // 货币代码
    uint8_t costPriceDecimalPlaces;  // 费用价格小数位数
    bool isUiJsonControlLocked[CHARGER_GUN_TYPE_QUAD];

    bool getMqttDataFlag;            // 从MQTT获取UI数据标志
    ioc_ui_dispenser_config_t dispenserConfig;

    bool isFirstUIjson;  //  是否是第一次获取UIjson数据

    ioc_ui_biright_t biright; // 设置屏幕亮度相关配置

    uint32_t swFunc;      // 功能开关
    uint32_t keyFunc[KEY_FUNC_COUNT];  // 三个功能按钮的功能
    bool isAutoCCS;      // 是否CCS 自动充电时

    ioc_ui_drvPointInfo_t drvPointInfo; // rf meter net等状态信息

} ioc_ui_json_t;


bool ioc_ui_json_init(void);
bool ioc_ui_json_step(void);
void ioc_ui_json_handle_over(void);

extern ioc_ui_json_t g_ui_json;
extern const UI_CurrencyMap_t c_CurrencyMaps[];

#define UI_NUMOFCONNECTOR                       (g_ui_json.connector_num)
#define UI_ENABLEPOS                            (g_ui_json.enPOS)
#define UI_ENABLESOFTKEY                        (g_ui_json.enSoftKey)
#define UI_CONNECTOR_ACTIVE_IDX                 (g_ui_json.active_connector_idx)
// #define UI_CONNECTOR_STARID                     (g_ui_json.dispenserConfig.connStarId)
#define UI_FIRST_JSON                           (g_ui_json.isFirstUIjson)
#define UI_SWFUNC                               (g_ui_json.swFunc)
#define UI_KEYFUNC(indx)                        (g_ui_json.keyFunc[indx])


#define UI_CONNECTOR_CONN_TYPE(idx)             (g_ui_json.connectors[idx].conn_type)
#define UI_CONNECTOR_CONN_TYPE_NAME(idx)        (ioc_ui_json_get_conn_type_name(g_ui_json.connectors[idx].conn_type))
#define UI_CONNECTOR_RATED_POWER(idx)           (g_ui_json.connectors[idx].rated_power)
#define UI_CONNECTOR_STATE(idx)                 (g_ui_json.connectors[idx].state)
#define UI_CONNECTOR_STATE_NAME(idx)            (ioc_ui_json_get_state_name(g_ui_json.connectors[idx].state))
#define UI_CONNECTOR_SHOWPRICE(idx)             (g_ui_json.connectors[idx].showPrice)
#define UI_CONNECTOR_ISOCPPRICE(idx)            (g_ui_json.connectors[idx].isOcppPrice)
#define UI_CONNECTOR_ISQ4DOT(idx)               (g_ui_json.connectors[idx].isQ4dot)
#define UI_CONNECTOR_ERRCODE                    (g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX]._errCode)
#define UI_CONNECTOR_IDTAG(idx)                 (g_ui_json.connectors[idx].idTag)
#define UI_CONNECTOR_IDTYPE(idx)                 (g_ui_json.connectors[idx].idType)


void ioc_ui_json_set_active_idx(int idx);
void ioc_ui_overtime_changed_page(void);
void ioc_send_UseCreditCard(int idx, int PerAuth);
void ioc_send_ConnSelected(int idx);
void ioc_send_StopPlugCharging(int idx);
void ioc_send_StartCharging(int idx);
bool ioc_ui_json_set_conn_state(int idx, ioc_ui_connector_state_t state);
bool ioc_conntype_isAC(int idx);
bool ioc_ui_json_parse(const char *json_str);
bool ioc_ui_json_set_con_num(int num);

#endif
