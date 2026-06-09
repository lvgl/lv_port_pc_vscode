#include "../pages_mngr.h"

#define UI_NAME_06_FINISHED_START_TIME                  ".finished_start_time.val"
#define UI_NAME_06_FINISHED_ENERGY                      ".finished_energy.val"
#define UI_NAME_06_FINISHED_START_SOC                   ".finished_soc.val_1"
#define UI_NAME_06_FINISHED_END_SOC                     ".finished_soc.val_2"
#define UI_NAME_06_FINISHED_DURATION                    ".finished_duration.val"
#define UI_NAME_06_FINISHED_START_TIME_LABEL            ".finished_start_time.title"
#define UI_NAME_06_FINISHED_ENERGY_LABEL                ".finished_energy.title"
#define UI_NAME_06_FINISHED_SOC_LABEL                   ".finished_soc.title"
#define UI_NAME_06_FINISHED_DURATION_LABEL              ".finished_duration.title"

#define UI_NAME_06_FINISHED_ENERGY_UNIT                 ".finished_energy.unit"



typedef struct {

    lv_obj_t * page;

    lv_obj_t * start_time;
    lv_obj_t * energy;
    lv_obj_t * start_soc;
    lv_obj_t * end_soc;
    lv_obj_t * duration;

    lv_obj_t * start_time_label;
    lv_obj_t * energy_label;
    lv_obj_t * soc_label;
    lv_obj_t * duration_label;

    lv_obj_t * energy_unit;

} ui_page_06_finished_t;

static ui_page_06_finished_t ui_page_06_finished = {NULL};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ui_page_06_finished_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};

    if (ui_page_06_finished.energy != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        // 判断是否显示四位小时，默认显示两位小数
        if (UI_CONNECTOR_ISQ4DOT(UI_CONNECTOR_ACTIVE_IDX))
        {
            // 已经在解析时拼接了第四位，在这直接显示
            snprintf(buf, sizeof(buf), "%.4f", UI_FINISHED_DATA.actualMeter);
        }
        else
        {
            snprintf(buf, sizeof(buf), "%.2f", UI_FINISHED_DATA.actualMeter);
        }
        ioc_label_set_text(ui_page_06_finished.energy, buf);
    }

    if (ui_page_06_finished.duration != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        sprintf((char*)buf, "%02d:%02d",
            (UI_FINISHED_DATA.duration  / 3600),
            ((UI_FINISHED_DATA.duration   / 60) % 60)
            );
        ioc_label_set_text(ui_page_06_finished.duration, buf);
    }

    if (ui_page_06_finished.start_time != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        time_t n = 0;
        n = UI_FINISHED_DATA.startTime;

        struct tm local;
        struct tm * pTt = localtime_r(&n, &local);

        sprintf((char*)buf, "%02d:%02d", (pTt->tm_hour), (pTt->tm_min));
        ioc_label_set_text(ui_page_06_finished.start_time, buf);
    }

    if (ui_page_06_finished.start_soc != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", UI_FINISHED_DATA.startSoc);
        ioc_label_set_text(ui_page_06_finished.start_soc, buf);
    }

    if (ui_page_06_finished.end_soc != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", UI_FINISHED_DATA.stopSoc);
        ioc_label_set_text(ui_page_06_finished.end_soc, buf);
    }

    if (ui_page_06_finished.end_soc != NULL)
    {
        memset(buf, 0x00, sizeof(buf));
        snprintf(buf, sizeof(buf), "%d", UI_FINISHED_DATA.stopSoc);
        ioc_label_set_text(ui_page_06_finished.end_soc, buf);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_06_finished(lv_obj_t *parent)
{
    memset(&ui_page_06_finished, 0, sizeof(ui_page_06_finished));

    ui_page_06_finished.page = ioc_layout_mngr_load_layout("finished", parent);
    if (ui_page_06_finished.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_06_finished.page);
        user_data->ui_json_update_cb = ui_page_06_finished_updatejson;
    }

    return ui_page_06_finished.page;
}

void ui_page_show_06_finished(void)
{

}

void ui_page_hide_06_finished(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_finished_update_start_time_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_06_finished.start_time_label, "Start_time");
}

void ui_after_create_06_finished_start_time_label(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.start_time_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_start_time_label;
}

void ui_finished_update_energy_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_06_finished.energy_label, "Energy");
}

void ui_after_create_06_finished_energy_label(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.energy_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_energy_label;
}

void ui_finished_update_soc_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_06_finished.soc_label, "SoC");
}

void ui_after_create_06_finished_soc_label(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.soc_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_soc_label;
}

void ui_finished_update_duration_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_06_finished.duration_label, "Duration");
}

void ui_after_create_06_finished_duration_label(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.duration_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_finished_update_duration_label;
}


void ui_after_create_06_finished_start_time(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.start_time = obj;
}
void ui_after_create_06_finished_energy(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.energy = obj;
}
void ui_after_create_06_finished_start_soc(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.start_soc = obj;
}
void ui_after_create_06_finished_end_soc(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.end_soc = obj;
}
void ui_after_create_06_finished_duration(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.duration = obj;
}
void ui_after_create_06_finished_energy_unit(const char * name, lv_obj_t * obj)
{
    ui_page_06_finished.energy_unit= obj;
    ioc_label_set_text(ui_page_06_finished.energy_unit, "kWh");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_06_finished(const char * name, lv_obj_t * obj)
{
    const char * finished_name = name + strlen(UI_NAME_06_FINISHED);
    if (strcmp(finished_name, UI_NAME_06_FINISHED_START_TIME_LABEL) == 0)
    {
        ui_after_create_06_finished_start_time_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_ENERGY_LABEL) == 0)
    {
        ui_after_create_06_finished_energy_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_SOC_LABEL) == 0)
    {
        ui_after_create_06_finished_soc_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_DURATION_LABEL) == 0)
    {
        ui_after_create_06_finished_duration_label(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_START_TIME) == 0)
    {
        ui_after_create_06_finished_start_time(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_ENERGY) == 0)
    {
        ui_after_create_06_finished_energy(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_START_SOC) == 0)
    {
        ui_after_create_06_finished_start_soc(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_END_SOC) == 0)
    {
        ui_after_create_06_finished_end_soc(name, obj);
        return;
    }
    else if (strcmp(finished_name, UI_NAME_06_FINISHED_DURATION) == 0)
    {
        ui_after_create_06_finished_duration(name, obj);
        return;
    }
        else if (strcmp(finished_name, UI_NAME_06_FINISHED_ENERGY_UNIT) == 0)
    {
        ui_after_create_06_finished_energy_unit(name, obj);
        return;
    }
}
