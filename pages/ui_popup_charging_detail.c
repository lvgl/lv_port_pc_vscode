#include "../pages_mngr.h"

#define UI_NAME_DETAIL_CLOSE_BTN                       ".close_button"

#define UI_NAME_DETAIL_CLOSE_TEXT                      ".close_button.text"
#define UI_NAME_DETAIL_DETAIL_DATA                     ".detailed_data"

#define UI_NAME_DETAIL_RUN_STATE                       ".card_left.run_state"
#define UI_NAME_DETAIL_SUB_STATE                       ".card_left.sub_state"
#define UI_NAME_DETAIL_ERROR_CODE                      ".card_left.error_code"
#define UI_NAME_DETAIL_CONN_STATUS                     ".card_left.conn_status"
#define UI_NAME_DETAIL_START_SOC                       ".card_left.start_soc"
#define UI_NAME_DETAIL_REQUEST_VOLTAGE                 ".card_left.request_voltage"
#define UI_NAME_DETAIL_REQUEST_CURRENT                 ".card_left.request_current"
#define UI_NAME_DETAIL_OFFER_VOLTAGE                   ".card_left.offer_voltage"
#define UI_NAME_DETAIL_RUN_STATE_VALUE                 ".card_left.run_state_value"
#define UI_NAME_DETAIL_SUB_STATE_VALUE                 ".card_left.sub_state_value"
#define UI_NAME_DETAIL_ERROR_CODE_VALUE                ".card_left.error_code_value"
#define UI_NAME_DETAIL_CONN_STATUS_VALUE               ".card_left.conn_status_value"
#define UI_NAME_DETAIL_START_SOC_VALUE                 ".card_left.start_soc_value"
#define UI_NAME_DETAIL_REQUEST_VOLTAGE_VALUE           ".card_left.request_voltage_value"
#define UI_NAME_DETAIL_REQUEST_CURRENT_VALUE           ".card_left.request_current_value"
#define UI_NAME_DETAIL_OFFER_VOLTAGE_VALUE             ".card_left.offer_voltage_value"

#define UI_NAME_DETAIL_OFFER_CURRENT                   ".card_right.offer_current"
#define UI_NAME_DETAIL_OUTPUT_LIMIT                    ".card_right.output_limit"
#define UI_NAME_DETAIL_COMP_GROUP                      ".card_right.comp_group"
#define UI_NAME_DETAIL_CONN_TEMPERATURE                ".card_right.conn_temperature"
#define UI_NAME_DETAIL_OUTPUT_VOLTAGE                  ".card_right.output_voltage"
#define UI_NAME_DETAIL_OUTPUT_CURRENT                  ".card_right.output_current"
#define UI_NAME_DETAIL_OFFER_CURRENT_VALUE             ".card_right.offer_current_value"
#define UI_NAME_DETAIL_OUTPUT_LIMIT_VALUE              ".card_right.output_limit_value"
#define UI_NAME_DETAIL_COMP_GROUP_VALUE                ".card_right.comp_group_value"
#define UI_NAME_DETAIL_CONN_TEMPERATURE_VALUE          ".card_right.conn_temperature_value"
#define UI_NAME_DETAIL_OUTPUT_VOLTAGE_VALUE            ".card_right.output_voltage_value"
#define UI_NAME_DETAIL_OUTPUT_CURRENT_VALUE            ".card_right.output_current_value"


typedef enum {
    CLOSE_TEXT,
    DETAIL_DATA,

    RUN_STATE,
    SUB_STATE,
    ERROR_CODE,
    CONN_STATUS,
    START_SOC,
    REQUEST_VOLTAGE,
    REQUEST_CURRENT,
    OFFER_VOLTAGE,
    OFFER_CURRENT,
    OUTPUT_LIMIT,
    COMP_GROUP,
    CONN_TEMPERATURE,
    OUTPUT_VOLTAGE,
    OUTPUT_CURRENT,
    RUN_STATE_VALUE,
    SUB_STATE_VALUE,
    ERROR_CODE_VALUE,
    CONN_STATUS_VALUE,
    START_SOC_VALUE,
    REQUEST_VOLTAGE_VALUE,
    REQUEST_CURRENT_VALUE,
    OFFER_VOLTAGE_VALUE,
    OFFER_CURRENT_VALUE,
    OUTPUT_LIMIT_VALUE,
    COMP_GROUP_VALUE,
    CONN_TEMPERATURE_VALUE,
    OUTPUT_VOLTAGE_VALUE,
    OUTPUT_CURRENT_VALUE,

    CARD_NUMBER

}detail_card_e;

typedef struct {

    lv_obj_t * obj;
    lv_obj_t * close_btn;
    lv_obj_t * card_label[CARD_NUMBER];
}detailed_data_t;

detailed_data_t detailed_data;

// 显示充电详情弹出信息, 内容由ui_json提供
void ui_popup_show_charging_detail(int idx, int type)
{
    ui_popup_show_mask(true);   // 显示遮罩层
    if ( detailed_data.obj == NULL ) {
        detailed_data.obj = ioc_layout_mngr_load_layout("detailed_data", lv_layer_top());
    }
    if ( detailed_data.obj != NULL ) {
        lv_obj_clear_flag(detailed_data.obj, LV_OBJ_FLAG_HIDDEN);
    }
    ioc_label_set_text(detailed_data.card_label[CLOSE_TEXT],        "Close");
    ioc_label_set_text(detailed_data.card_label[DETAIL_DATA],       "Detailed_data");
    ioc_label_set_text(detailed_data.card_label[RUN_STATE],         "Run_State");
    ioc_label_set_text(detailed_data.card_label[SUB_STATE],         "Sub_State");
    ioc_label_set_text(detailed_data.card_label[ERROR_CODE],        "Error_Code");
    ioc_label_set_text(detailed_data.card_label[CONN_STATUS],       "Conn_Status");
    ioc_label_set_text(detailed_data.card_label[START_SOC],         "Start_SoC");
    ioc_label_set_text(detailed_data.card_label[REQUEST_VOLTAGE],   "Request_Voltage");
    ioc_label_set_text(detailed_data.card_label[REQUEST_CURRENT],   "Request_Current");
    ioc_label_set_text(detailed_data.card_label[OFFER_VOLTAGE],     "Offer_Voltage");
    ioc_label_set_text(detailed_data.card_label[OFFER_CURRENT],     "Offer_Current");
    ioc_label_set_text(detailed_data.card_label[OUTPUT_LIMIT],      "Output_Limit");
    ioc_label_set_text(detailed_data.card_label[COMP_GROUP],        "Comp_Group");
    ioc_label_set_text(detailed_data.card_label[CONN_TEMPERATURE],  "Conn_Temperature");
    ioc_label_set_text(detailed_data.card_label[OUTPUT_VOLTAGE],    "Output_Voltage");
    ioc_label_set_text(detailed_data.card_label[OUTPUT_CURRENT],    "Output_Current");

    // 设置详细数据
    char buf[128] = {0};
    ioc_label_set_text(detailed_data.card_label[RUN_STATE_VALUE],       g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].m1768Info_runState);
    snprintf(buf, sizeof(buf), "%d", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].subState);
    ioc_label_set_text(detailed_data.card_label[SUB_STATE_VALUE],       buf);

    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, sizeof(buf), "%d", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].m1768errCode);
    ioc_label_set_text(detailed_data.card_label[ERROR_CODE_VALUE],      buf);

    memset(buf, 0x00, sizeof(buf));
    //TODO 国际化
    if(strcmp(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].connStatus, "Unplug") == 0)
    {
        ioc_label_set_text(detailed_data.card_label[CONN_STATUS_VALUE], "Unplug");
    }
    else if(strcmp(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].connStatus, "Plug") == 0)
    {
        ioc_label_set_text(detailed_data.card_label[CONN_STATUS_VALUE], "Plugged");
    }
    else if(strcmp(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].connStatus, "Charging") == 0)
    {
        ioc_label_set_text(detailed_data.card_label[CONN_STATUS_VALUE], "Charging");
    }
    else if(strcmp(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].connStatus, "Finished") == 0)
    {
        ioc_label_set_text(detailed_data.card_label[CONN_STATUS_VALUE], "Finished");
    }
    else
    {
        ioc_label_set_text(detailed_data.card_label[CONN_STATUS_VALUE], "Fault");
    }

    // 交流没有以下参数
    if(ioc_conntype_isAC(UI_CONNECTOR_ACTIVE_IDX))
    {
        ioc_label_set_text(detailed_data.card_label[START_SOC_VALUE],         "N/A");
        ioc_label_set_text(detailed_data.card_label[REQUEST_VOLTAGE_VALUE],   "N/A");
        ioc_label_set_text(detailed_data.card_label[REQUEST_CURRENT_VALUE],   "N/A");
        ioc_label_set_text(detailed_data.card_label[CONN_TEMPERATURE_VALUE],  "N/A");
    }
    else
    {
        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].startSoc);
        ioc_label_set_text(detailed_data.card_label[START_SOC_VALUE],         buf);

        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%.1f", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].reqVol);
        strncat(buf, "V", sizeof(buf) - strlen(buf) - 1);
        ioc_label_set_text(detailed_data.card_label[REQUEST_VOLTAGE_VALUE],   buf);

        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%.1f", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].reqCur);
        strncat(buf, "A", sizeof(buf) - strlen(buf) - 1);
        ioc_label_set_text(detailed_data.card_label[REQUEST_CURRENT_VALUE],   buf);

        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].temp);
        strncat(buf, "°", sizeof(buf) - strlen(buf) - 1);
        ioc_label_set_text(detailed_data.card_label[CONN_TEMPERATURE_VALUE],   buf);
    }

    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, sizeof(buf), "%.1f", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].offerVol);
    strncat(buf, "V", sizeof(buf) - strlen(buf) - 1);
    ioc_label_set_text(detailed_data.card_label[OFFER_VOLTAGE_VALUE],   buf);

    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, sizeof(buf), "%.1f", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].offerCur);
    strncat(buf, "A", sizeof(buf) - strlen(buf) - 1);
    ioc_label_set_text(detailed_data.card_label[OFFER_CURRENT_VALUE],   buf);

    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, sizeof(buf), "%.1f", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].outLimit);
    strncat(buf, g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].outLimitUnit, sizeof(buf) - strlen(buf) - 1);
    ioc_label_set_text(detailed_data.card_label[OUTPUT_LIMIT_VALUE],   buf);

    if(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].compGroup == 1)
    {
        ioc_label_set_text(detailed_data.card_label[COMP_GROUP_VALUE],   "A");
    }
    else if(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].compGroup == 2)
    {
        ioc_label_set_text(detailed_data.card_label[COMP_GROUP_VALUE],   "B");
    }
    else if(g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].compGroup == 3)
    {
        ioc_label_set_text(detailed_data.card_label[COMP_GROUP_VALUE],   "A + B");
    }
    else
    {
        ioc_label_set_text(detailed_data.card_label[COMP_GROUP_VALUE],   "N/A");
    }

    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, sizeof(buf), "%.1f", g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.charging.meterData.voltage[0]);
    strncat(buf, "V", sizeof(buf) - strlen(buf) - 1);
    ioc_label_set_text(detailed_data.card_label[OUTPUT_VOLTAGE_VALUE],   buf);


    double Curr[3];
    Curr[0] = g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.charging.meterData.current[0];
    Curr[1] = g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.charging.meterData.current[1];
    Curr[2] = g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.charging.meterData.current[2];

    double maxCurr = (Curr[0] > Curr[1])? Curr[0] : Curr[1];
    maxCurr = (maxCurr > Curr[2])? maxCurr : Curr[2];

    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, sizeof(buf), "%.1f", maxCurr);
    strncat(buf, "V", sizeof(buf) - strlen(buf) - 1);
    ioc_label_set_text(detailed_data.card_label[OUTPUT_CURRENT_VALUE],   buf);

}

typedef struct {
    const char *suffix;
    lv_obj_t **obj;
} ui_detail_obj_map_entry_t;

static ui_detail_obj_map_entry_t obj_map[] = {
    { UI_NAME_DETAIL_CLOSE_BTN,                 &detailed_data.close_btn },

    { UI_NAME_DETAIL_CLOSE_TEXT,                &detailed_data.card_label[CLOSE_TEXT] },
    { UI_NAME_DETAIL_DETAIL_DATA,               &detailed_data.card_label[DETAIL_DATA] },

    { UI_NAME_DETAIL_RUN_STATE,                 &detailed_data.card_label[RUN_STATE] },
    { UI_NAME_DETAIL_SUB_STATE,                 &detailed_data.card_label[SUB_STATE] },
    { UI_NAME_DETAIL_ERROR_CODE,                &detailed_data.card_label[ERROR_CODE] },
    { UI_NAME_DETAIL_CONN_STATUS,               &detailed_data.card_label[CONN_STATUS] },
    { UI_NAME_DETAIL_START_SOC,                 &detailed_data.card_label[START_SOC] },
    { UI_NAME_DETAIL_REQUEST_VOLTAGE,           &detailed_data.card_label[REQUEST_VOLTAGE] },
    { UI_NAME_DETAIL_REQUEST_CURRENT,           &detailed_data.card_label[REQUEST_CURRENT] },
    { UI_NAME_DETAIL_OFFER_VOLTAGE,             &detailed_data.card_label[OFFER_VOLTAGE] },
    { UI_NAME_DETAIL_OFFER_CURRENT,             &detailed_data.card_label[OFFER_CURRENT] },
    { UI_NAME_DETAIL_OUTPUT_LIMIT,              &detailed_data.card_label[OUTPUT_LIMIT] },
    { UI_NAME_DETAIL_COMP_GROUP,                &detailed_data.card_label[COMP_GROUP] },
    { UI_NAME_DETAIL_CONN_TEMPERATURE,          &detailed_data.card_label[CONN_TEMPERATURE] },
    { UI_NAME_DETAIL_OUTPUT_VOLTAGE,            &detailed_data.card_label[OUTPUT_VOLTAGE] },
    { UI_NAME_DETAIL_OUTPUT_CURRENT,            &detailed_data.card_label[OUTPUT_CURRENT] },

    { UI_NAME_DETAIL_RUN_STATE_VALUE,                 &detailed_data.card_label[RUN_STATE_VALUE] },
    { UI_NAME_DETAIL_SUB_STATE_VALUE,                 &detailed_data.card_label[SUB_STATE_VALUE] },
    { UI_NAME_DETAIL_ERROR_CODE_VALUE,                &detailed_data.card_label[ERROR_CODE_VALUE] },
    { UI_NAME_DETAIL_CONN_STATUS_VALUE,               &detailed_data.card_label[CONN_STATUS_VALUE] },
    { UI_NAME_DETAIL_START_SOC_VALUE,                 &detailed_data.card_label[START_SOC_VALUE] },
    { UI_NAME_DETAIL_REQUEST_VOLTAGE_VALUE,           &detailed_data.card_label[REQUEST_VOLTAGE_VALUE] },
    { UI_NAME_DETAIL_REQUEST_CURRENT_VALUE,           &detailed_data.card_label[REQUEST_CURRENT_VALUE] },
    { UI_NAME_DETAIL_OFFER_VOLTAGE_VALUE,             &detailed_data.card_label[OFFER_VOLTAGE_VALUE] },
    { UI_NAME_DETAIL_OFFER_CURRENT_VALUE,             &detailed_data.card_label[OFFER_CURRENT_VALUE] },
    { UI_NAME_DETAIL_OUTPUT_LIMIT_VALUE,              &detailed_data.card_label[OUTPUT_LIMIT_VALUE] },
    { UI_NAME_DETAIL_COMP_GROUP_VALUE,                &detailed_data.card_label[COMP_GROUP_VALUE] },
    { UI_NAME_DETAIL_CONN_TEMPERATURE_VALUE,          &detailed_data.card_label[CONN_TEMPERATURE_VALUE] },
    { UI_NAME_DETAIL_OUTPUT_VOLTAGE_VALUE,            &detailed_data.card_label[OUTPUT_VOLTAGE_VALUE] },
    { UI_NAME_DETAIL_OUTPUT_CURRENT_VALUE,            &detailed_data.card_label[OUTPUT_CURRENT_VALUE] }
};

static void detail_close_event_cb(lv_event_t *e);

void ui_after_create_detaile_data(const char * name, lv_obj_t * obj)
{
    const char * dataile_name = name + strlen(UI_NAME_DETAILED_DATA);

    for (size_t i = 0; i < sizeof(obj_map) / sizeof(obj_map[0]); ++i) {
        if (strcmp(dataile_name, obj_map[i].suffix) == 0) {
            *(obj_map[i].obj) = obj;
            if ( strcmp(dataile_name, UI_NAME_DETAIL_CLOSE_BTN) == 0 ) {
                lv_obj_add_event_cb(obj, detail_close_event_cb, LV_EVENT_CLICKED, (void*)obj_map[i].suffix);
            }
            return;
        }
    }
}

static void detail_close_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_add_flag(detailed_data.obj, LV_OBJ_FLAG_HIDDEN);
        ui_popup_show_mask(false);
    }
}
