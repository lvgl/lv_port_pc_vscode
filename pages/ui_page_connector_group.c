
#include "../pages_mngr.h"

#define UI_NAME_CONNECTOR_CARD_NO               "conn_card_id"
#define UI_NAME_CONNECTOR_CARD_NO_TEXT          "conn_card_id.NoText"
#define UI_NAME_CONNECTOR_CARD_STATE            "conn_card_state"
#define UI_NAME_CONNECTOR_CARD_TYPE             "conn_card_type"
#define UI_NAME_CONNECTOR_CARD_POWER            "conn_card_power_rated"
#define UI_NAME_CONNECTOR_CARD_TYPE_ICON        "conn_card_type_icon"
#define UI_NAME_CONNECTOR_CARD_PLUG_GUN_ICON    "conn_card_plug_gun_icon"
#define UI_NAME_CONNECTOR_CARD_TIME_LABEL       "conn_card_time_title"
#define UI_NAME_CONNECTOR_CARD_TIME             "conn_card_time"
#define UI_NAME_CONNECTOR_CARD_SOC_BAR          "conn_card_soc_bar"
#define UI_NAME_CONNECTOR_CARD_SOC_LABEL        "conn_card_soc"
#define UI_NAME_CONNECTOR_CARD_POWER_REAL       "conn_card_power_real"
// 60秒无操作后返回home页面
#define CANCEL_ACTIVE_GUN_TIMER_COUNT 60

typedef enum {
    CARD_VISUAL_IDLE = 0,
    CARD_VISUAL_CHARGING,
    CARD_VISUAL_FAULT,
    CARD_VISUAL_RESERVED,
    CARD_VISUAL_UNAVAILABLE,
    CARD_VISUAL_SUSPEND,

    CARD_VISUAL_NUM
} card_visual_state_t;

typedef struct
{
    uint8_t idx;

    ioc_ui_connector_state_t state;                     // 记录state, 跟UI.json对比, 不一致需要更新Style
    lv_obj_t * card;                                    // 卡片本身, 需要按照不同的State来切换Style
    lv_style_t * visual_style[CARD_VISUAL_NUM];         // 预置的卡片选中状态的style
    lv_color_t style_color[CARD_VISUAL_NUM];            // 不同style对应的颜色
    card_visual_state_t current_visual_state;           // 卡片当前选中状态的state
    bool card_actived;                                  // 卡片是否被激活
    lv_timer_t * cancel_active_gun_timer;               // 60秒无操作后取消激活枪的定时器

    lv_obj_t * text_state;          // 当前状态, 需要根据State来改变

    lv_obj_t * no;                  // 枪编号, 一次性, 不需要改变
    lv_obj_t * text_no;             // 枪编号文本, 一次性, 不需要改变
    lv_obj_t * text_type;           // 枪类型, 一次性, 不需要改变
    lv_obj_t * text_power_rated;    // 枪额定功率, 一次性, 不需要改变
    lv_obj_t * type_icon;           // 枪类型图标, 一次性, 不需要改变
    lv_obj_t * plug_gun_icon;       // 枪类型图标, 一次性, 不需要改变

    lv_obj_t * text_time_label;     // 计时时间提示信息, Reserved为剩余时间,  Finished为占位时间
    lv_obj_t * text_time_value;     // 计时时间值, Reserved为剩余时间,  Finished为占位时间

    lv_obj_t * bar_soc;             // 充电中SoC进度条, 仅Charging状态
    lv_obj_t * text_soc;            // 充电中SoC文本, 仅Charging状态
    lv_obj_t * text_power_real;     // 充电中实时功率, 仅Charging状态

} ui_connector_card_t;
lv_timer_t *update_time_value;
static ui_connector_card_t s_connector_card[UI_MAX_CONNECTOR_NUM] = {0};

lv_obj_t * ui_page_create_connector_group(lv_obj_t *parent)
{
    if (UI_NUMOFCONNECTOR == 4)
    {
        return ioc_layout_mngr_load_layout("conn_card_quad", parent);
    }
    else if (UI_NUMOFCONNECTOR == 3)
    {
        return ioc_layout_mngr_load_layout("conn_card_trip", parent);
    }
    else if (UI_NUMOFCONNECTOR == 2)
    {
        return ioc_layout_mngr_load_layout("conn_card_dual", parent);
    }
    else if (UI_NUMOFCONNECTOR == 1)
    {
        ioc_ui_json_set_active_idx(0);      // 单枪不需要home
    }

    return NULL;
}


static void card_set_visual_state(ui_connector_card_t * connector_card)
{
    // DEBUG_WARN("card %d state %d active=%d", connector_card->idx, connector_card->current_visual_state, connector_card->card_actived);

    // 先清掉所有“视觉相关”的 state
    for (int i = 0; i < CARD_VISUAL_NUM; i++)
    {
        lv_obj_remove_style(connector_card->card, connector_card->visual_style[i], LV_PART_MAIN);
    }

    if (!connector_card->card_actived)
    {
        // DEBUG_WARN("card %d exit state %04X", connector_card->idx, connector_card->current_visual_state);
        return;
    }

    if (connector_card->current_visual_state < CARD_VISUAL_NUM)
    {
        lv_obj_add_style(connector_card->card, connector_card->visual_style[connector_card->current_visual_state], LV_PART_MAIN);
    }

    // DEBUG_WARN("card %d set state %04X", connector_card->idx, connector_card->current_visual_state);
}

// 点击卡片切换active idx
static void ui_connector_card_btn_event_cb(lv_event_t *e)
{
    // DEBUG_WARN("1111custom_tick_get[%ld]",custom_tick_get());
    lv_obj_t *obj = lv_event_get_target(e);
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    int idx = user_data->extra_data.data_int;

    if (s_connector_card[idx].card_actived)
    {
        s_connector_card[idx].card_actived = false;
        card_set_visual_state(&s_connector_card[idx]);

        lv_timer_pause(s_connector_card[idx].cancel_active_gun_timer);

        ioc_ui_json_set_active_idx(-1);
    }
    else
    {
        for (int i = 0; i < UI_NUMOFCONNECTOR; i++)
        {
            if (i != idx)
            {
                s_connector_card[i].card_actived = false;
                card_set_visual_state(&s_connector_card[i]);

                lv_timer_pause(s_connector_card[i].cancel_active_gun_timer);
            }
        }

        // DEBUG_WARN("2222custom_tick_get[%ld]",custom_tick_get());
        s_connector_card[idx].card_actived = true;
        card_set_visual_state(&s_connector_card[idx]);
        ioc_ui_json_set_active_idx(idx);

        lv_timer_reset(s_connector_card[idx].cancel_active_gun_timer);
        lv_timer_resume(s_connector_card[idx].cancel_active_gun_timer);

        // 向homeOcpp发送激活的枪号
        ioc_send_ConnSelected(idx);
    }
}

static void update_time_value_cb(lv_timer_t * temp)
{

    for ( int idx = 0; idx < CHARGER_GUN_TYPE_QUAD; idx++ )
    {
        if ( s_connector_card[idx].text_state == NULL )
        {
            return;
        }
        if ( strcmp(lv_label_get_text(s_connector_card[idx].text_state), lv_i18n_get_text("Finished")) == 0 )
        {
            time_t current_time = time(NULL);
            time_t diftimer = difftime(current_time, g_ui_json.connectors[idx].state_data.finished.stopTime);

            if (diftimer < 0) {
                diftimer = 0;
            }

            struct tm *tm = gmtime(&diftimer);
            int show_hour = 0;
            int show_min  = 0;
            int show_sec  = 0;

            if (current_time >= g_ui_json.connectors[idx].state_data.finished.stopTime) {
                show_hour = tm->tm_hour;
                show_min  = tm->tm_min;
                show_sec  = tm->tm_sec;
            }
            char buf[64] = {0};
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", show_hour, show_min, show_sec);
            ioc_label_set_text(s_connector_card[idx].text_time_value, buf);
        }
        else if ( strcmp(lv_label_get_text(s_connector_card[idx].text_state), lv_i18n_get_text("Reserved")) == 0 )
        {
            time_t current_time = time(NULL);
            time_t diftimer = difftime(g_ui_json.connectors[idx].state_data.reserved.reverseTime, current_time);

            if (diftimer < 0) {
                diftimer = 0;
            }

            struct tm *tm = gmtime(&diftimer);
            int show_hour = 0;
            int show_min  = 0;
            int show_sec  = 0;

            if (current_time < g_ui_json.connectors[idx].state_data.reserved.reverseTime) {
                show_hour = tm->tm_hour;
                show_min  = tm->tm_min;
                show_sec  = tm->tm_sec;
            }
            char buf[64] = {0};
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", show_hour, show_min, show_sec);
            ioc_label_set_text(s_connector_card[idx].text_time_value, buf);
        }
    }
}

char * ui_connector_get_text_time_value(int idx)
{
    return lv_label_get_text(s_connector_card[idx].text_time_value);
}

// 卡片本身需要根据不同的State来切换Style和显示隐藏部分控件
static void ui_connector_card_update(struct ioc_obj_user_data_s * user_data)
{
    int idx = user_data->extra_data.data_int;
    if (idx > UI_NUMOFCONNECTOR)
    {
        return;
    }

    // 判断是否显示插枪图标
    if ( s_connector_card[idx].plug_gun_icon != NULL )
    {
        if(strcmp(g_ui_json.connectors[idx].connStatus, "Unplug") != 0)
        {
            if ( UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_CHARGING )
            {
                lv_obj_add_flag(s_connector_card[idx].plug_gun_icon, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_clear_flag(s_connector_card[idx].plug_gun_icon, LV_OBJ_FLAG_HIDDEN);
            }
        }
        else
        {
            lv_obj_add_flag(s_connector_card[idx].plug_gun_icon, LV_OBJ_FLAG_HIDDEN);
        }
    }

    // 更新状态
    s_connector_card[idx].state = UI_CONNECTOR_STATE(idx);

    // 切换激活状态
    bool need_update_style = false;
    // DEBUG_WARN("idx %d, card_actived %d", idx, s_connector_card[idx].card_actived);
    if (idx == UI_CONNECTOR_ACTIVE_IDX)
    {
        if (!s_connector_card[idx].card_actived)
        {
            s_connector_card[idx].card_actived = true;
            // DEBUG_WARN("idx %d, card_actived", idx);

            need_update_style = true;

            lv_timer_reset(s_connector_card[idx].cancel_active_gun_timer);
            lv_timer_resume(s_connector_card[idx].cancel_active_gun_timer);
        }
    }
    else
    {
        if (s_connector_card[idx].card_actived)
        {
            s_connector_card[idx].card_actived = false;
            // DEBUG_WARN("idx %d, card_deactived", idx);

            need_update_style = true;

            lv_timer_pause(s_connector_card[idx].cancel_active_gun_timer);
        }
    }

    // 切换Style
    card_visual_state_t visual_state;
    switch (UI_CONNECTOR_STATE(idx))
    {
    case UI_CONNECTOR_STATE_CONNECTING:
    case UI_CONNECTOR_STATE_STARTING1:
    case UI_CONNECTOR_STATE_STARTING2:
    case UI_CONNECTOR_STATE_CHARGING:
    case UI_CONNECTOR_STATE_FINISHED:
    case UI_CONNECTOR_STATE_WAIT_IN_LINE:
    case UI_CONNECTOR_STATE_STOPPING:
    case UI_CONNECTOR_STATE_PROCESSING:
        visual_state = CARD_VISUAL_CHARGING;
        break;

    case UI_CONNECTOR_STATE_RESERVED:
        visual_state = CARD_VISUAL_RESERVED;
        break;

    case UI_CONNECTOR_STATE_SESSION_ERROR:
    case UI_CONNECTOR_STATE_START_ERROR:
    case UI_CONNECTOR_STATE_FAULT:
        visual_state = CARD_VISUAL_FAULT;
        break;

    case UI_CONNECTOR_STATE_UNAVAILABLE:
        visual_state = CARD_VISUAL_UNAVAILABLE;
        break;

    case UI_CONNECTOR_STATE_SUSPEND:
        visual_state = CARD_VISUAL_SUSPEND;
        break;

    case UI_CONNECTOR_STATE_HOME:
    case UI_CONNECTOR_STATE_IDLE:
    default:
        visual_state = CARD_VISUAL_IDLE;
        break;
    }
    if (visual_state != s_connector_card[idx].current_visual_state)
    {
        s_connector_card[idx].current_visual_state = visual_state;
        need_update_style = true;
        lv_obj_set_style_bg_color(s_connector_card[idx].no, s_connector_card[idx].style_color[visual_state] , 0);
        lv_obj_set_style_text_color(s_connector_card[idx].text_state, s_connector_card[idx].style_color[visual_state],0);
    }

    if (need_update_style)
    {
        card_set_visual_state(&s_connector_card[idx]);
    }


    // if (UI_CONNECTOR_STATE(idx) == s_connector_card[idx].state)
    // {
    //     return;
    // }

    // 切换部分控件的显示状态
    if (UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_CHARGING)
    {
        lv_obj_add_flag(s_connector_card[idx].text_type, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_power_rated, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].type_icon, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(s_connector_card[idx].text_time_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_time_value, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(s_connector_card[idx].bar_soc, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_connector_card[idx].text_soc, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_connector_card[idx].text_power_real, LV_OBJ_FLAG_HIDDEN);
    }
    else if ((UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_FINISHED) || (UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_RESERVED))
    {
        lv_obj_add_flag(s_connector_card[idx].text_type, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_power_rated, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_connector_card[idx].type_icon, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(s_connector_card[idx].text_time_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_connector_card[idx].text_time_value, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(s_connector_card[idx].bar_soc, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_soc, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_power_real, LV_OBJ_FLAG_HIDDEN);

        if (UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_FINISHED)
        {
            ioc_label_set_text(s_connector_card[idx].text_time_label, "Idle time");
        }
        else {
            ioc_label_set_text(s_connector_card[idx].text_time_label, "left");
        }
    }
    else
    {
        lv_obj_clear_flag(s_connector_card[idx].text_type, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_connector_card[idx].text_power_rated, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(s_connector_card[idx].type_icon, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(s_connector_card[idx].text_time_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_time_value, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(s_connector_card[idx].bar_soc, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_soc, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(s_connector_card[idx].text_power_real, LV_OBJ_FLAG_HIDDEN);
    }
}

// 当前状态, 需要根据State来改变值
static void ui_connector_card_update_state(struct ioc_obj_user_data_s * user_data)
{
    int idx = user_data->extra_data.data_int;
    if (idx > UI_NUMOFCONNECTOR)
    {
        return;
    }
    ioc_label_set_text(s_connector_card[idx].text_state, UI_CONNECTOR_STATE_NAME(idx));
}

static void ui_connector_card_update_time_value(struct ioc_obj_user_data_s * user_data)
{
    int idx = user_data->extra_data.data_int;
    if (idx > UI_NUMOFCONNECTOR)
    {
        return;
    }

    if (UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_FINISHED)
    {
        ioc_label_set_text(s_connector_card[idx].text_time_value, "04:24");
    }
    else
    {
        ioc_label_set_text(s_connector_card[idx].text_time_value, "12:34:24");
    }

}

static void ui_connector_card_update_soc_bar(struct ioc_obj_user_data_s * user_data)
{
    int idx = user_data->extra_data.data_int;
    lv_bar_set_value(s_connector_card[idx].bar_soc, g_ui_json.connectors[idx].state_data.charging.soc, LV_ANIM_OFF);
    char soc[10];
    snprintf(soc, sizeof(soc), "%d%%", g_ui_json.connectors[idx].state_data.charging.soc);
    ioc_label_set_text(s_connector_card[idx].text_soc, soc);
}

static void ui_connector_card_update_time_label(struct ioc_obj_user_data_s * user_data)
{
    int idx = user_data->extra_data.data_int;
    if (idx > UI_NUMOFCONNECTOR)
    {
        return;
    }
    if (UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_FINISHED)
    {
        ioc_label_set_text(s_connector_card[idx].text_time_label, "Idle_time");
    }
    else if ( UI_CONNECTOR_STATE(idx) == UI_CONNECTOR_STATE_RESERVED ) {
        ioc_label_set_text(s_connector_card[idx].text_time_label, "left");
    }
}

// 定时回调：选中枪显示对应页面后，一定时间无操作后返回home页面
static void active_timer_cb(lv_timer_t * t)
{
    ui_connector_card_t *connector_card = (ui_connector_card_t *)lv_timer_get_user_data(t);
    if (connector_card == NULL)
    {
        return;
    }

    // DEBUG_WARN("active_timer_cb, idx %d card_actived %d", connector_card->idx, connector_card->card_actived);
    lv_timer_pause(connector_card->cancel_active_gun_timer);

    if (connector_card->card_actived)
    {
        connector_card->card_actived = false;
        card_set_visual_state(connector_card);

        UI_CONNECTOR_ACTIVE_IDX = -1;
        LV_LOG_USER("active_idx -> -1");
        ioc_ui_overtime_changed_page();
    }
}

static void ui_connector_card_register_obj(int idx, const char * name_after_prefix, int  name_after_prefix_len, lv_obj_t *obj)
{
    if (idx > UI_NUMOFCONNECTOR)
    {
        return;
    }

    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->extra_data.data_int = idx;   // 先记录枪号, 便于后续使用

    // 需要记录各对象, 用于后续更新
    ui_connector_card_t * connector_card = &s_connector_card[idx];
    connector_card->idx = idx;
    if ( update_time_value == NULL )
    {
        update_time_value = lv_timer_create(update_time_value_cb, 500, NULL);
    }
    if (name_after_prefix == NULL)
    {
        connector_card->card = obj;
        lv_obj_add_event_cb(obj, ui_connector_card_btn_event_cb, LV_EVENT_CLICKED, NULL);   // 按钮用于切换枪
        user_data->ui_json_update_cb = ui_connector_card_update;  // 卡片本身, 需要按照不同的State来切换Style

        lv_style_t * style_find = NULL;

        connector_card->visual_style[CARD_VISUAL_IDLE] = ioc_style_mngr_get_style("style-connector-dual-checked");
        connector_card->style_color[CARD_VISUAL_IDLE] = ioc_theme_getcolor("state_idle_success");
        connector_card->visual_style[CARD_VISUAL_CHARGING] = ioc_style_mngr_get_style("style-connector-dual-checked-charging");
        connector_card->style_color[CARD_VISUAL_CHARGING] =  ioc_theme_getcolor("state_charging");
        connector_card->visual_style[CARD_VISUAL_FAULT] = ioc_style_mngr_get_style("style-connector-dual-checked-state_fault");
        connector_card->style_color[CARD_VISUAL_FAULT] = ioc_theme_getcolor("state_fault");
        connector_card->visual_style[CARD_VISUAL_RESERVED] = ioc_style_mngr_get_style("style-connector-dual-checked-state_reserve");
        connector_card->style_color[CARD_VISUAL_RESERVED] = ioc_theme_getcolor("state_reserve");
        connector_card->visual_style[CARD_VISUAL_UNAVAILABLE] = ioc_style_mngr_get_style("style-connector-dual-checked-state_unavaliable");
        connector_card->style_color[CARD_VISUAL_UNAVAILABLE] = ioc_theme_getcolor("state_unavaliable");
        connector_card->visual_style[CARD_VISUAL_SUSPEND] = ioc_style_mngr_get_style("style-connector-dual-checked-state_warn_suspend");
        connector_card->style_color[CARD_VISUAL_SUSPEND] = ioc_theme_getcolor("state_warn_suspend");
        connector_card->current_visual_state = CARD_VISUAL_IDLE;
        connector_card->card_actived = false;

        connector_card->cancel_active_gun_timer = lv_timer_create(active_timer_cb, CANCEL_ACTIVE_GUN_TIMER_COUNT * 1000, connector_card);
        lv_timer_pause(connector_card->cancel_active_gun_timer);

        connector_card->state = UI_CONNECTOR_STATE_MAX;   // 强制触发第一次更新
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_STATE, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_state = obj;
        user_data->ui_json_update_cb = ui_connector_card_update_state;  // 当前状态, 需要根据State来改变值
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_NO, name_after_prefix, name_after_prefix_len))
    {
        connector_card->no = obj; // 枪编号, 一次性, 不需要改变
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_NO_TEXT, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_no = obj;

        char no[10];
        sprintf(no, "%d", idx + 1);
        ioc_label_set_text(connector_card->text_no, no);   // 枪编号文本, 一次性, 不需要改变
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_TYPE, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_type = obj;
        ioc_label_set_text(connector_card->text_type, UI_CONNECTOR_CONN_TYPE_NAME(idx));   // 枪类型, 一次性, 不需要改变
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_POWER, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_power_rated = obj;

        char power[40];
        snprintf(power, 20, "%.1fkW", (double)UI_CONNECTOR_RATED_POWER(idx) / 10.0);
        ioc_label_set_text(connector_card->text_power_rated, power);       // 枪额定功率, 一次性, 不需要改变
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_TYPE_ICON, name_after_prefix, name_after_prefix_len))
    {
        connector_card->type_icon = obj;
        lv_img_set_src(connector_card->type_icon, ui_conn_type_img(UI_CONNECTOR_CONN_TYPE(idx)));   // 枪类型图标, 一次性, 不需要改变
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_PLUG_GUN_ICON, name_after_prefix, name_after_prefix_len))
    {
        connector_card->plug_gun_icon = obj;
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_TIME_LABEL, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_time_label = obj;
        user_data->ui_json_update_cb = ui_connector_card_update_time_label;  // 当前状态, 需要根据State来改变值
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_TIME, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_time_value = obj;
        // user_data->ui_json_update_cb = ui_connector_card_update_time_value;
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_SOC_BAR, name_after_prefix, name_after_prefix_len))
    {
        connector_card->bar_soc = obj;

        lv_bar_set_range(connector_card->bar_soc, 0, 100);
        // lv_bar_set_value(card->bar_soc, 10, LV_ANIM_OFF);
        user_data->ui_json_update_cb = ui_connector_card_update_soc_bar;
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_SOC_LABEL, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_soc = obj;
        // user_data->ui_json_update_cb = ui_connector_card_update_soc_label;
        return;
    }
    else if (IOC_OBJ_NAME_MATCH_AFTER_PREFIX(UI_NAME_CONNECTOR_CARD_POWER_REAL, name_after_prefix, name_after_prefix_len))
    {
        connector_card->text_power_real = obj;
        // user_data->ui_json_update_cb = ui_connector_card_update_power_real;
        return;
    }

    return;
}

void ui_after_create_connector_card(const char *name, lv_obj_t *obj)
{
    int id;
    const char * name_after_prefix;
    int  name_after_prefix_len;

    if (ioc_obj_name_match_prefix(name
        , UI_NAME_CONNECTOR_CARD_DUAL_PREFIX, strlen(UI_NAME_CONNECTOR_CARD_DUAL_PREFIX) - 1
        , &id, &name_after_prefix, &name_after_prefix_len))
    {
        ui_connector_card_register_obj(id-1, name_after_prefix, name_after_prefix_len, obj);
        return;
    }
    // if (ioc_obj_name_match_prefix(name
    //     , UI_NAME_CONNECTOR_CARD_SINGLE_PREFIX, strlen(UI_NAME_CONNECTOR_CARD_SINGLE_PREFIX) - 1
    //     , &id, &name_after_prefix, &name_after_prefix_len))
    // {
    //     ui_connector_card_register_obj(id-1, name_after_prefix, name_after_prefix_len, obj);
    //     return;
    // }
    if (ioc_obj_name_match_prefix(name
        , UI_NAME_CONNECTOR_CARD_QUAD_PREFIX, strlen(UI_NAME_CONNECTOR_CARD_QUAD_PREFIX) - 1
        , &id, &name_after_prefix, &name_after_prefix_len))
    {
        ui_connector_card_register_obj(id-1, name_after_prefix, name_after_prefix_len, obj);
        return;
    }
    if (ioc_obj_name_match_prefix(name
        , UI_NAME_CONNECTOR_CARD_TRIP_PREFIX, strlen(UI_NAME_CONNECTOR_CARD_TRIP_PREFIX) - 1
        , &id, &name_after_prefix, &name_after_prefix_len))
    {
        ui_connector_card_register_obj(id-1, name_after_prefix, name_after_prefix_len, obj);
        return;
    }
}
