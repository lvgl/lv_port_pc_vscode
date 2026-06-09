#include "pages_mngr.h"


typedef enum
{
    // CHARGING_CONNECTOR_LEFT,
    // CHARGING_CONNECTOR_RIGHT,
    CONNECTOR_CARD_DUAL,
    CONNECTOR_CARD_QUAD,
    CONNECTOR_CARD_TRIP,
    STATE_MID_CONTAINER_TOP,
    STATE_HOME,
    STATE_IDLE,
    STATE_STARTING1,
    STATE_STARTING2,
    STATE_CHARGING,
    STATE_FINISHED,
    STATE_RESERVED,
    STATE_SESSION_ERROR,
    STATE_START_ERROR,
    STATE_FAULT,
    STATE_UNAVAILABLE,
    STATE_CONNECTING,
    STATE_STOPPING,
    STATE_WAIT_IN_LINE,
    STATE_SUSPEND,
    STATE_PROCESSING,
    STATU_BAR,
    STATU_HELP1,
    STATU_HELP2,
    STATU_LANGUAGE,
    STATU_BILL,
    STATU_PRE_AUTHORIZATION,
    STATU_DETAILED_DATA,
    STATU_POPUP_COMMON,
    

    STATU_ENUM
}app_register_e;

/**
 * !!!!!! 注意, 父亲和孩子同时存在的时候, 必须先写孩子后写父亲, 否则会导致孩子无法执行
 * 例如: "main.connector_group.group*" 必须在"main.connector_group.group1.connector*"后面
 */
ioc_obj_handler_t s_preset_handler[STATU_ENUM];

void s_preset_handler_init(void)
{   static int times = 0;
    if ( times <= 1)
    {
        // charging_connector_left 页面
        // s_preset_handler[CHARGING_CONNECTOR_LEFT].name = UI_NAME_05_CHARGING_CONNECTOR_LEFT;
        // s_preset_handler[CHARGING_CONNECTOR_LEFT].after_create = ui_after_create_05_charging_connector_left;
        // s_preset_handler[CHARGING_CONNECTOR_LEFT].before_create = NULL;

        // // charging_connector_right 页面

        // s_preset_handler[CHARGING_CONNECTOR_RIGHT].name = UI_NAME_05_CHARGING_CONNECTOR_RIGHT;
        // s_preset_handler[CHARGING_CONNECTOR_RIGHT].after_create = ui_after_create_05_charging_connector_right;
        // s_preset_handler[CHARGING_CONNECTOR_RIGHT].before_create = NULL;

        // 四种卡片和卡片组的注册
        s_preset_handler[CONNECTOR_CARD_DUAL].name = UI_NAME_CONNECTOR_CARD_DUAL_PREFIX;
        s_preset_handler[CONNECTOR_CARD_DUAL].after_create = ui_after_create_connector_card;
        s_preset_handler[CONNECTOR_CARD_DUAL].before_create = NULL;


        s_preset_handler[CONNECTOR_CARD_QUAD].name = UI_NAME_CONNECTOR_CARD_QUAD_PREFIX;
        s_preset_handler[CONNECTOR_CARD_QUAD].after_create = ui_after_create_connector_card;
        s_preset_handler[CONNECTOR_CARD_QUAD].before_create = NULL;


        s_preset_handler[CONNECTOR_CARD_TRIP].name = UI_NAME_CONNECTOR_CARD_TRIP_PREFIX;
        s_preset_handler[CONNECTOR_CARD_TRIP].after_create = ui_after_create_connector_card;
        s_preset_handler[CONNECTOR_CARD_TRIP].before_create = NULL;


        // 中间顶层状态栏的注册
        s_preset_handler[STATE_MID_CONTAINER_TOP].name = UI_NAME_STATE_MID_CONTAINER_TOP;
        s_preset_handler[STATE_MID_CONTAINER_TOP].after_create = ui_after_create_state_mid_container_top;
        s_preset_handler[STATE_MID_CONTAINER_TOP].before_create = NULL;

        // home 页面
        s_preset_handler[STATE_HOME].name = UI_NAME_STATE_HOME;
        s_preset_handler[STATE_HOME].after_create = ui_after_create_state_home;
        s_preset_handler[STATE_HOME].before_create = NULL;

        // idle 页面
        s_preset_handler[STATE_IDLE].name = UI_NAME_02_IDLE;
        s_preset_handler[STATE_IDLE].after_create = ui_after_create_02_idle;
        s_preset_handler[STATE_IDLE].before_create = NULL;

        // starting1 页面
        s_preset_handler[STATE_STARTING1].name = UI_NAME_03_STARTING1;
        s_preset_handler[STATE_STARTING1].after_create = ui_after_create_03_starting1;
        s_preset_handler[STATE_STARTING1].before_create = NULL;

        // starting2 页面
        s_preset_handler[STATE_STARTING2].name = UI_NAME_04_STARTING2;
        s_preset_handler[STATE_STARTING2].after_create = ui_after_create_04_starting2;
        s_preset_handler[STATE_STARTING2].before_create = NULL;

        // charging 页面
        s_preset_handler[STATE_CHARGING].name = UI_NAME_05_CHARGING;
        s_preset_handler[STATE_CHARGING].after_create = ui_after_create_05_charging;
        s_preset_handler[STATE_CHARGING].before_create = NULL;

        // finished 页面
        s_preset_handler[STATE_FINISHED].name = UI_NAME_06_FINISHED;
        s_preset_handler[STATE_FINISHED].after_create = ui_after_create_06_finished;
        s_preset_handler[STATE_FINISHED].before_create = NULL;

        // reserved 页面
        s_preset_handler[STATE_RESERVED].name = UI_NAME_07_RESERVED;
        s_preset_handler[STATE_RESERVED].after_create = ui_after_create_07_reserved;
        s_preset_handler[STATE_RESERVED].before_create = NULL;

        // session_error 页面
        s_preset_handler[STATE_SESSION_ERROR].name = UI_NAME_08_SESSION_ERROR;
        s_preset_handler[STATE_SESSION_ERROR].after_create = ui_after_create_08_session_error;
        s_preset_handler[STATE_SESSION_ERROR].before_create = NULL;

        // start_error 页面
        s_preset_handler[STATE_START_ERROR].name = UI_NAME_09_START_ERROR;
        s_preset_handler[STATE_START_ERROR].after_create = ui_after_create_09_start_error;
        s_preset_handler[STATE_START_ERROR].before_create = NULL;

        // falut 页面
        s_preset_handler[STATE_FAULT].name = UI_NAME_10_FAULT;
        s_preset_handler[STATE_FAULT].after_create = ui_after_create_10_fault;
        s_preset_handler[STATE_FAULT].before_create = NULL;

        // unavailable 页面
        s_preset_handler[STATE_UNAVAILABLE].name = UI_NAME_11_UNAVAILABLE;
        s_preset_handler[STATE_UNAVAILABLE].after_create = ui_after_create_11_unavailable;
        s_preset_handler[STATE_UNAVAILABLE].before_create = NULL;

        // connecting 页面
        s_preset_handler[STATE_CONNECTING].name = UI_NAME_12_CONNECTING;
        s_preset_handler[STATE_CONNECTING].after_create = ui_after_create_12_connecting;
        s_preset_handler[STATE_CONNECTING].before_create = NULL;

        // stopping 页面
        s_preset_handler[STATE_STOPPING].name = UI_NAME_14_STOPPING;
        s_preset_handler[STATE_STOPPING].after_create = ui_after_create_14_stopping;
        s_preset_handler[STATE_STOPPING].before_create = NULL;

        // wait_in_line 页面
        s_preset_handler[STATE_WAIT_IN_LINE].name = UI_NAME_15_WAIT_IN_LINE;
        s_preset_handler[STATE_WAIT_IN_LINE].after_create = ui_after_create_15_wait_in_line;
        s_preset_handler[STATE_WAIT_IN_LINE].before_create = NULL;

        // suspend 页面
        s_preset_handler[STATE_SUSPEND].name = UI_NAME_16_SUSPEND;
        s_preset_handler[STATE_SUSPEND].after_create = ui_after_create_16_suspend;
        s_preset_handler[STATE_SUSPEND].before_create = NULL;

        // processing 页面
        s_preset_handler[STATE_PROCESSING].name = UI_NAME_17_PROCESSING;
        s_preset_handler[STATE_PROCESSING].after_create = ui_after_create_17_processing;
        s_preset_handler[STATE_PROCESSING].before_create = NULL;

        // 状态栏的注册
        s_preset_handler[STATU_BAR].name = UI_NAME_STATUS_BAR;
        s_preset_handler[STATU_BAR].after_create = ui_after_create_status_bar;
        s_preset_handler[STATU_BAR].before_create = NULL;


        // 帮助界面
        s_preset_handler[STATU_HELP1].name = UI_NAME_HELP1;
        s_preset_handler[STATU_HELP1].after_create = ui_after_create_help1;
        s_preset_handler[STATU_HELP1].before_create = NULL;


        s_preset_handler[STATU_HELP2].name = UI_NAME_HELP2;
        s_preset_handler[STATU_HELP2].after_create = ui_after_create_help2;
        s_preset_handler[STATU_HELP2].before_create = NULL;


        // 语言界面
        s_preset_handler[STATU_LANGUAGE].name = UI_NAME_LANGUAGE;
        s_preset_handler[STATU_LANGUAGE].after_create = ui_after_create_language;
        s_preset_handler[STATU_LANGUAGE].before_create = NULL;


        // 账单界面
        s_preset_handler[STATU_BILL].name = UI_NAME_BILL;
        s_preset_handler[STATU_BILL].after_create = ui_after_create_bill;
        s_preset_handler[STATU_BILL].before_create = NULL;


        // 软键盘界面
        s_preset_handler[STATU_PRE_AUTHORIZATION].name = UI_NAME_PRE_AUTHORIZATION;
        s_preset_handler[STATU_PRE_AUTHORIZATION].after_create = ui_after_create_preauthorization;
        s_preset_handler[STATU_PRE_AUTHORIZATION].before_create = NULL;

        // 充电详细信息界面
        s_preset_handler[STATU_DETAILED_DATA].name = UI_NAME_DETAILED_DATA;
        s_preset_handler[STATU_DETAILED_DATA].after_create = ui_after_create_detaile_data;
        s_preset_handler[STATU_DETAILED_DATA].before_create = NULL;

        // 通用弹窗界面
        s_preset_handler[STATU_POPUP_COMMON].name = UI_NAME_POPUP_COMMON;
        s_preset_handler[STATU_POPUP_COMMON].after_create = ui_after_create_popup_common;
        s_preset_handler[STATU_POPUP_COMMON].before_create = NULL;
        times ++;
    }
    else
    {
        return;
    }
}

int ioc_get_preset_handler_count(void)
{
    return sizeof(s_preset_handler) / sizeof(s_preset_handler[0]);
}

// 如果用户没有主动选择语言，那么一直由ui.json决定
bool ioc_ui_json_set_language(int language)
{
    if ( s_ui_language.check_language == false )
    {
        if (language < 0 || language >= LANG_NUM) return false;

        if (g_ui_json.language != language)
        {
            LV_LOG_WARN("language %d->%d", g_ui_json.language, language);
            g_ui_json.language = language;
            lv_i18n_set_locale(c_LanguageName[language]);
            ui_language_update_selected(c_LanguageName[language]);
            return true;
        }

        return false;
    }
    return true;
}
