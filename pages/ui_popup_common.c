#include "../pages_mngr.h"

static lv_obj_t * s_ui_popup_mask = NULL;

void ui_popup_show_mask(bool show)
{
    if (s_ui_popup_mask == NULL)
    {
        s_ui_popup_mask = lv_obj_create(lv_layer_top());
        lv_obj_set_size(s_ui_popup_mask,800,600);
        lv_obj_set_style_bg_color(s_ui_popup_mask,lv_color_make(0,0,0),0);
        lv_obj_set_style_bg_opa(s_ui_popup_mask,100,0);
        lv_obj_set_style_radius(s_ui_popup_mask,0,0);
    }
    if (show)
    {
        lv_obj_clear_flag(s_ui_popup_mask, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(s_ui_popup_mask, LV_OBJ_FLAG_HIDDEN);
    }
}

#define UI_NAME_COMMON_PROMPT_TEXT                     ".prompt.text"
#define UI_NAME_COMMON_CLOSE_BTN                       ".close_button"
#define UI_NAME_COMMON_CLOSE_TEXT                      ".close_button.text"
#define UI_NAME_COMMON_CLOSE_SECONDS                   ".close_button.seconds"
#define UI_NAME_COMMON_CIRCLE_IMG                      ".circle_img"

#define COMMOM_CLOSE_TIMER_COUNT 60
typedef struct {

    lv_obj_t * obj;
    lv_obj_t * prompt_text;
    lv_obj_t * close_btn;
    lv_obj_t * close_text;
    lv_obj_t * close_seconds;
    lv_obj_t * circle_img;

    lv_timer_t * close_timer;
    uint16_t     close_time_counter;
}popup_common_t;


popup_common_t popup_common;

void ui_popup_show_common(char *str, bool needi18n, circle_e circle_img)
{
    ui_popup_show_mask(true);   // 显示遮罩层
    if ( popup_common.obj == NULL ) {
        popup_common.obj = ioc_layout_mngr_load_layout("popup_common", lv_layer_top());
    }
    if ( popup_common.obj != NULL ) {
        lv_obj_clear_flag(popup_common.obj, LV_OBJ_FLAG_HIDDEN);
    }

    popup_set_circle_img(circle_img);

    if(needi18n)
    {
        ioc_label_set_text(popup_common.prompt_text, str);
    }
    else
    {
        ioc_label_set_text(popup_common.prompt_text, str);
    }

     // "Downloading" 和 “Installing"特殊处理，不显示倒计时
     if (strcmp(str, "Downloading") == 0 || strcmp(str, "Installing") == 0)
     {
         lv_obj_add_state(popup_common.close_btn, LV_STATE_DISABLED);
         char buf[16] = {0};
         sprintf(buf, "%d%%", g_ui_json.trigger.percent);
         LV_LOG_WARN("buf[%s]  percent[%d]", buf, g_ui_json.trigger.percent);
         ioc_label_set_text(popup_common.close_text, buf);
         lv_timer_resume(popup_common.close_timer);
         return;
     }

    // 如果按钮是禁用状态，则恢复
    if (lv_obj_has_state(popup_common.close_btn, LV_STATE_DISABLED))
    {
        lv_obj_clear_state(popup_common.close_btn, LV_STATE_DISABLED);
    }

    ioc_label_set_text(popup_common.close_text, "Close");
    lv_timer_resume(popup_common.close_timer);

    return;
}

static void common_close_timer_cb(lv_timer_t * t) {

    if (popup_common.close_time_counter == 0) {
        lv_obj_add_flag(popup_common.obj, LV_OBJ_FLAG_HIDDEN);
        ui_popup_show_mask(false);
        lv_timer_pause(popup_common.close_timer);
        popup_common.close_time_counter = 0;
        return;
    }

    popup_common.close_time_counter--;


    char buf[8] = {0};
    // "Downloading" 和 “Installing"特殊处理，按钮文本显示百分比
    if((strcmp(g_ui_json.trigger.trigger, "Downloading") != 0) && (strcmp(g_ui_json.trigger.trigger, "Installing") != 0))
    {
        char buf[8];
        snprintf(buf, sizeof(buf), "(%ds)", popup_common.close_time_counter);
        ioc_label_set_text(popup_common.close_seconds, buf);
    }
    else
    {
        ioc_label_set_text(popup_common.close_seconds, buf);
    }
}

typedef struct {
    const char *suffix;
    lv_obj_t **obj;
} ui_detail_obj_map_entry_t;

static ui_detail_obj_map_entry_t obj_map[] = {
    { UI_NAME_COMMON_PROMPT_TEXT,              &popup_common.prompt_text },

    { UI_NAME_COMMON_CLOSE_BTN,                &popup_common.close_btn },
    { UI_NAME_COMMON_CLOSE_TEXT,               &popup_common.close_text },
    { UI_NAME_COMMON_CLOSE_SECONDS,            &popup_common.close_seconds },
    { UI_NAME_COMMON_CIRCLE_IMG,               &popup_common.circle_img }

};

static void popup_common_close_event_cb(lv_event_t *e);

void ui_after_create_popup_common(const char * name, lv_obj_t * obj)
{
    const char * popup_common_name = name + strlen(UI_NAME_POPUP_COMMON);

    for (size_t i = 0; i < sizeof(obj_map) / sizeof(obj_map[0]); ++i) {
        if (strcmp(popup_common_name, obj_map[i].suffix) == 0) {
            *(obj_map[i].obj) = obj;
            if ( strcmp(popup_common_name, UI_NAME_COMMON_CLOSE_BTN) == 0 ) {
                lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
                lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
                lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                lv_obj_set_style_pad_column(obj, 5, 0);
                lv_obj_add_event_cb(obj, popup_common_close_event_cb, LV_EVENT_CLICKED, (void*)obj_map[i].suffix);
            }
            return;
        }
    }
    if ( popup_common.close_timer == NULL )
    {
        popup_common.close_timer = lv_timer_create(common_close_timer_cb, 1000, NULL);
        lv_timer_pause(popup_common.close_timer);
    }
}

static void popup_common_close_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_add_flag(popup_common.obj, LV_OBJ_FLAG_HIDDEN);
        ui_popup_show_mask(false);
    }
}

void popup_set_circle_img(int idx)
{
    static int last_index = -1;
    if ( last_index == idx )
        return;
    switch (idx)
    {
    case POP_PROMPTS :
        lv_img_set_src(popup_common.circle_img, "A:./assets/images/icon_inmation.png");
        break;
    case POP_WARNRING :
        lv_img_set_src(popup_common.circle_img, "A:./assets/images/icon_warning_2.png");
        break;
    case POP_ERROR :
        lv_img_set_src(popup_common.circle_img, "A:./assets/images/icon_error.png");
        break;
    default:
        break;
    }
    last_index = idx;
}


void ui_popup_common_handle(const char *trigger_str)
{
    if(g_ui_json.trigger.seconds != 0)
    {
        popup_common.close_time_counter = g_ui_json.trigger.seconds;
    }

    memset(g_ui_json.trigger.trigger, 0x00, sizeof(g_ui_json.trigger.trigger));
    strncpy(g_ui_json.trigger.trigger, trigger_str, sizeof(g_ui_json.trigger.trigger) - 1);

    circle_e circle_img;
    char* str = NULL;
    if ( strcmp(trigger_str, "Downloading") == 0 )
    {
        str = "Downloading";
        circle_img = POP_PROMPTS;
        popup_common.close_time_counter = 6000;  // 设置一个大的数卡在这个界面
    }
    else if ( strcmp(trigger_str, "Installing") == 0 )
    {
        str = "Installing";
        circle_img = POP_PROMPTS;
        popup_common.close_time_counter = 6000;  // 设置一个大的数卡在这个界面
    }
    else if ( strcmp(trigger_str, "Prompt") == 0 )
    {
        switch (g_ui_json.trigger.msg)
        {
        case CHARGER_SCREEN_MSG_NO_CONN_SELECTED:
            str = "No connector selected";
            break;
        case CHARGER_SCREEN_MSG_FAILED_TO_START_CHARGING:
            str = "Failed to start charging";
            break;
        case CHARGER_SCREEN_MSG_FAULT_FOUND_STOP_CHARGING:
            str = "Fault found, Stop Charging";
            break;
        case CHARGER_SCREEN_MSG_ADDRFID:
            str = "Add RFID succeed";       //TODO 国际化
            break;
        case CHARGER_SCREEN_MSG_CLEARRFID:
            str = "Remove RFID succeed";    //TODO 国际化
            break;
        case CHARGER_SCREEN_MSG_CCS_AUTOCHARGE_BIND:
        {
            int conidx = ui_page_04_starting2_get_autu_conidx();
            switch (conidx)
            {
            case 0:
                str = "Please start charging within 1 minutes for connector 1th";
                break;
            case 1:
                str = "Please start charging within 1 minutes for connector 2th";
                break;
            case 2:
                str = "Please start charging within 1 minutes for connector 3th";
                break;
            case 3:
                str = "Please start charging within 1 minutes for connector 4th";
                break;
            default:
                str = "Please start charging within 1 minutes";
                break;
            }
        }
            break;
        case CHARGER_SCREEN_MSG_NOT_PERMIT:
        default:
            str = "Not Permit";
            break;
        }
        circle_img = POP_WARNRING;
    }
    else if ( strcmp(trigger_str, "CreditCard") == 0 )
    {
        switch (g_ui_json.trigger.msg)
        {
        case CHARGER_SCREEN_CREDIT_POINT:
            str = "Swipe, insert or tap your card on the POS";
            circle_img = POP_PROMPTS;
            break;
        case CHARGER_SCREEN_CREDIT_NO_CONN_SELECTED:
            str = "No connector selected";
            circle_img = POP_ERROR;
            break;
        case CHARGER_SCREEN_CREDIT_PREAUTH_FAILED:
            str = "pre-authorization failed";
            circle_img = POP_ERROR;
            break;
        case CHARGER_SCREEN_CREDIT_PREAUTH_SUCCESS:
            str = "Pre-Authorization Settled";
            circle_img = POP_PROMPTS;
            break;
        case CHARGER_SCREEN_CREDIT_SETTLEMENT_FAILED:
            str = "Payment failed";
            circle_img = POP_ERROR;
            break;
        case CHARGER_SCREEN_CREDIT_SETTLEMENT_SUCCESS:
            str = "Payment Settled";
            circle_img = POP_PROMPTS;
            break;
        case CHARGER_SCREEN_CREDIT_WATTING:
            str = "POS is processing the request, please wait";
            circle_img = POP_PROMPTS;
            break;
        case CHARGER_SCREEN_CREDIT_VPOS_BUSY:
            str = "POS busy, please try again later";
            circle_img = POP_WARNRING;
            break;
        case CHARGER_SCREEN_CREDIT_VPOS_FAULT:
            str = "POS error, please contact the service";
            circle_img = POP_ERROR;
            break;
        default:
            str = "ERROR";
            circle_img = POP_ERROR;
            break;
        }
    }
    else if (strcmp(trigger_str, "Auth") == 0)
    {
        switch (g_ui_json.trigger.msg)
        {
        case CHARGER_SCREEN_AUTH_FAIL_BLOCKED:
            str = "Authorization Failed: Blocked";
            break;
        case CHARGER_SCREEN_AUTH_FAIL_EXPIRED:
            str = "Authorization Failed: Expired";
            break;
        case CHARGER_SCREEN_AUTH_FAIL_INVALID:
            str = "Authorization Failed: Invalid";
            break;
        case CHARGER_SCREEN_AUTH_FAIL_CONCURRENTTX:
            str = "Authorization Failed: Concurrent TX";
            break;
        case CHARGER_SCREEN_AUTH_SUCCESS:
            str = "Authorized";
            break;
        case CHARGER_SCREEN_AUTH_FAIL_OTHER:
        default:
            str = "Authorization Failed: Other Reason";
            break;
        }
        if(g_ui_json.trigger.msg == CHARGER_SCREEN_AUTH_SUCCESS)
        {
            circle_img = POP_PROMPTS;
        }
        else
        {
            circle_img = POP_ERROR;
        }
    }
    else if (strcmp(trigger_str, "Message") == 0)
    {
        // message里的内容是由调用者填写，内容不确定，所以不用国际化
        str = g_ui_json.trigger.content;
        circle_img = POP_PROMPTS;
        ui_popup_show_common(str, false, circle_img);
        return;
    }
    else if (strcmp(trigger_str, "Cancel") == 0)
    {
        popup_common.close_time_counter = 0;
        return;
    }
    else if (strcmp(trigger_str, "MessageCancel") == 0)
    {
        return;
    }
    else
    {
        LV_LOG_WARN("Unknow Trigger[%s]", trigger_str);
        return;
    }

    ui_popup_show_common(str, true, circle_img);

    return;
}
