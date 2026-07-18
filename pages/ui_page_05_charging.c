#include "../pages_mngr.h"

#define UI_CHARGING_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.charging

#define UI_NAME_05_CHARGING_POWER              ".charging_power.val"
#define UI_NAME_05_CHARGING_ENERGY             ".charging_energy.val"
#define UI_NAME_05_CHARGING_SOC                ".charging_soc.val"
#define UI_NAME_05_CHARGING_DURATION           ".charging_duration.val"
#define UI_NAME_05_CHARGING_CAR_GIF            ".charging_car_icon.charging_car_gif"

#define UI_NAME_05_CHARGING_POWER_LABEL         ".charging_power.title"
#define UI_NAME_05_CHARGING_ENERGY_LABEL        ".charging_energy.title"
#define UI_NAME_05_CHARGING_SOC_LABEL           ".charging_soc.title"
#define UI_NAME_05_CHARGING_DURATION_LABEL      ".charging_duration.title"

#define UI_NAME_05_CHARGING_POWER_UNIT         ".charging_power.unit"
#define UI_NAME_05_CHARGING_ENERGY_UNIT        ".charging_energy.unit"
#define UI_NAME_05_CHARGING_SOC_UNIT           ".charging_soc.unit"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * car_gif;

    lv_obj_t * power;
    lv_obj_t * energy;
    lv_obj_t * soc;
    lv_obj_t * duration;

    lv_obj_t * power_label;
    lv_obj_t * energy_label;
    lv_obj_t * soc_label;
    lv_obj_t * duration_label;

    lv_obj_t * power_unit;
    lv_obj_t * energy_unit;
    lv_obj_t * soc_unit;

} ui_page_05_charging_t;

static ui_page_05_charging_t s_ui_page_05_charging = {NULL};
static lv_timer_t * charging_timer = NULL;

static void ui_page_05_charging_time(lv_timer_t * temp)
{
    LV_UNUSED(temp);

    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    time_t current_time = time(NULL);
    time_t diftimer = difftime(current_time, UI_CHARGING_DATA.startTime);

    if (diftimer < 0) {
        diftimer = 0;
    }

    char buf[64] = {0};
    struct tm * tm = gmtime(&diftimer);
    if(current_time < UI_CHARGING_DATA.startTime) {
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 0,0,0);
        ioc_label_set_text(s_ui_page_05_charging.duration, buf);
    }else{
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
        ioc_label_set_text(s_ui_page_05_charging.duration, buf);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void ui_page_05_charging_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};

    if (s_ui_page_05_charging.power != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%.2f", UI_CHARGING_DATA.meterData.power);
        ioc_label_set_text(s_ui_page_05_charging.power, buf);
    }

    if (s_ui_page_05_charging.energy != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        // 判断是否显示四位小时，默认显示两位小数
        if (UI_CONNECTOR_ISQ4DOT(UI_CONNECTOR_ACTIVE_IDX))
        {
            // 已经在解析时拼接了第四位，在这直接显示
            snprintf(buf, sizeof(buf), "%.4f", UI_CHARGING_DATA.meterData.Q[3]);
        }
        else
        {
            snprintf(buf, sizeof(buf), "%.2f", UI_CHARGING_DATA.meterData.Q[3]);
        }
        ioc_label_set_text(s_ui_page_05_charging.energy, buf);
    }

    if (s_ui_page_05_charging.soc != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", UI_CHARGING_DATA.soc);
        ioc_label_set_text(s_ui_page_05_charging.soc, buf);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_05_charging(lv_obj_t *parent)
{
    memset(&s_ui_page_05_charging, 0, sizeof(s_ui_page_05_charging));

    s_ui_page_05_charging.page = ioc_layout_mngr_load_layout("charging", parent);
    if (s_ui_page_05_charging.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(s_ui_page_05_charging.page);
        user_data->ui_json_update_cb = ui_page_05_charging_updatejson;
    }

    return s_ui_page_05_charging.page;
}

void ui_page_show_05_charging(void)
{
    if (s_ui_page_05_charging.car_gif != NULL)
    {
        lv_gif_restart(s_ui_page_05_charging.car_gif);
    }

    // 为了时间显示的流畅，充电时间由本地计算
    if(charging_timer == NULL)
    {
        charging_timer = lv_timer_create(ui_page_05_charging_time, 500, NULL);
    }
}

void ui_page_hide_05_charging(void)
{
    if (s_ui_page_05_charging.car_gif != NULL)
    {
        lv_gif_pause(s_ui_page_05_charging.car_gif);
    }

    if (charging_timer!= NULL)
    {
        lv_timer_del(charging_timer);
        charging_timer = NULL;
        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 0, 0, 0);
        ioc_label_set_text(s_ui_page_05_charging.duration, buf);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_05_charging_power(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.power = obj;
}

void ui_after_create_05_charging_energy(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.energy = obj;
}

void ui_after_create_05_charging_soc(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.soc = obj;
}

void ui_after_create_05_charging_duration(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.duration = obj;
}

void ui_after_create_05_charging_power_unit(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.power_unit = obj;
    ioc_label_set_text(s_ui_page_05_charging.power_unit, "kW");
}

void ui_after_create_05_charging_energy_unit(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.energy_unit = obj;
    ioc_label_set_text(s_ui_page_05_charging.energy_unit, "kWh");
}

void ui_after_create_05_charging_soc_unit(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.soc_unit = obj;
    ioc_label_set_text(s_ui_page_05_charging.soc_unit, "%");
}

void ui_charging_update_power_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_05_charging.power_label, "Power");
}

void ui_after_create_05_charging_power_label(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.power_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_power_label;
}

void ui_charging_update_energy_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_05_charging.energy_label, "Energy");
}

void ui_after_create_05_charging_energy_label(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.energy_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_energy_label;
}

void ui_charging_update_soc_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_05_charging.soc_label, "SoC");
}

void ui_after_create_05_charging_soc_label(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.soc_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_soc_label;
}

void ui_charging_update_duration_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(s_ui_page_05_charging.duration_label, "Duration");
}

void ui_after_create_05_charging_duration_label(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.duration_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_charging_update_duration_label;
}


static void charging_detailed_data_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        ui_popup_show_charging_detail(0,0);
    }
}

void ui_after_create_05_charging_car_gif(const char * name, lv_obj_t * obj)
{
    s_ui_page_05_charging.car_gif = obj;
}


void ui_after_create_05_charging(const char * name, lv_obj_t * obj)
{
    const char * charging_name = name + strlen(UI_NAME_05_CHARGING);
    if (strcmp(charging_name, UI_NAME_05_CHARGING_POWER_LABEL) == 0)
    {
        ui_after_create_05_charging_power_label(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_ENERGY_LABEL) == 0)
    {
        ui_after_create_05_charging_energy_label(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_SOC_LABEL) == 0)
    {
        ui_after_create_05_charging_soc_label(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_DURATION_LABEL) == 0)
    {
        ui_after_create_05_charging_duration_label(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_POWER) == 0)
    {
        ui_after_create_05_charging_power(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_ENERGY) == 0)
    {
        ui_after_create_05_charging_energy(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_SOC) == 0)
    {
        ui_after_create_05_charging_soc(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_DURATION) == 0)
    {
        ui_after_create_05_charging_duration(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_CAR_GIF) == 0)
    {
        ui_after_create_05_charging_car_gif(name, obj);
        return;
    }
        if (strcmp(charging_name, UI_NAME_05_CHARGING_POWER_UNIT) == 0)
    {
        ui_after_create_05_charging_power_unit(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_ENERGY_UNIT) == 0)
    {
        ui_after_create_05_charging_energy_unit(name, obj);
        return;
    }
    else if (strcmp(charging_name, UI_NAME_05_CHARGING_SOC_UNIT) == 0)
    {
        ui_after_create_05_charging_soc_unit(name, obj);
        return;
    }

    return;
}

// static void ui_status_charging_btn_event_cb(lv_event_t *e)
// {
//     lv_obj_t *obj = lv_event_get_target(e);
//     lv_event_code_t code = lv_event_get_code(e);

//     if(code == LV_EVENT_CLICKED) {
//         ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
//         // lv_obj_clear_flag(ui_status_bar.mask, LV_OBJ_FLAG_HIDDEN);
//         // ui_create_status_popup(UI_NAME_DETAILED_DATA);
//         // lv_obj_clear_flag(ui_status_popup.detailed_data_page.obj, LV_OBJ_FLAG_HIDDEN);
//     }
// }


// void ui_state_charging_detailed_data(struct ioc_obj_user_data_s *user_data, const char *name)
// {
//     lv_obj_t *child = NULL;
//     struct ioc_obj_user_data_s *user_data_tmp;

//     // 遍历当前对象的子对象
//     for (int i = 0; i < lv_obj_get_child_cnt(user_data->obj); i++) {
//         child = lv_obj_get_child(user_data->obj, i);
//         user_data_tmp = lv_obj_get_user_data(child);
//         if (strcmp(get_last_field(user_data_tmp->name), name) == 0) {
//             lv_obj_add_event_cb(child, ui_status_charging_btn_event_cb, LV_EVENT_CLICKED, NULL);
//             break;
//         }
//         ui_state_charging_detailed_data(user_data_tmp, name);
//     }
// }
