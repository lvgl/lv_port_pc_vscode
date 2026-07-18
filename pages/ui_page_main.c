#include "../pages_mngr.h"

static lv_obj_t * s_ui_main = NULL;
static lv_obj_t * s_ui_status_bar = NULL;
static lv_obj_t * s_ui_mid_container_top = NULL;
static lv_obj_t * s_ui_connector_group = NULL;

static lv_obj_t * s_ui_state_pages[UI_PAGE_MAX];

typedef lv_obj_t * (*ui_page_create_func)(lv_obj_t *parent);
typedef void (*ui_page_show_func)(void);
typedef void (*ui_page_hide_func)(void);

typedef struct {
    ui_page_create_func create_func;
    ui_page_show_func show_func;
    ui_page_hide_func hide_func;
} ui_page_func_t;

const ui_page_func_t c_ui_page_create_funcs[UI_PAGE_MAX] = {
    {ui_page_create_01_home, NULL, NULL},
    {ui_page_create_02_idle, NULL, NULL},
    {ui_page_create_03_starting1, NULL, NULL},
    {ui_page_create_04_starting2, NULL, NULL},
    {ui_page_create_05_charging, ui_page_show_05_charging, ui_page_hide_05_charging},
    {ui_page_create_06_finished, ui_page_show_06_finished, ui_page_hide_06_finished},
    {ui_page_create_07_reserved, ui_page_show_07_reserved, ui_page_hide_07_reserved},
    {ui_page_create_08_session_error, NULL, NULL},
    {ui_page_create_09_start_error, NULL, NULL},
    {ui_page_create_10_fault, NULL, NULL},
    {ui_page_create_11_unavailable, NULL, NULL},
    {ui_page_create_12_connecting, NULL, NULL},
    {ui_page_create_14_stopping, NULL, NULL},
    {ui_page_create_15_wait_in_line, ui_page_show_15_wait_in_line, ui_page_hide_15_wait_in_line},
    {ui_page_create_16_suspend, NULL, NULL},
    {ui_page_create_17_processing, ui_page_show_17_processing, ui_page_hide_17_processing},
};


static void ui_page_main_show_state_page(ioc_ui_page_e page_idx)
{
    for (int i = 0; i < UI_PAGE_MAX; i++)
    {
        if (i == page_idx)
        {
            if (s_ui_state_pages[i] == NULL)
            {
                s_ui_state_pages[i] = c_ui_page_create_funcs[i].create_func(s_ui_main);
                lv_obj_move_foreground(s_ui_mid_container_top);
            }
            if (s_ui_state_pages[i] != NULL)
            {
                lv_obj_clear_flag(s_ui_state_pages[i], LV_OBJ_FLAG_HIDDEN);
                if (c_ui_page_create_funcs[i].show_func != NULL)
                {
                    c_ui_page_create_funcs[i].show_func();
                }
                ioc_app_ui_json_updated_widgets(s_ui_state_pages[i]);
            }
            if ( s_ui_mid_container_top != NULL ) {
                if ( i == UI_PAGE_HOME ) {
                    lv_obj_add_flag(s_ui_mid_container_top, LV_OBJ_FLAG_HIDDEN);
                }
                else {
                    lv_obj_clear_flag(s_ui_mid_container_top, LV_OBJ_FLAG_HIDDEN);
                }
            }
        }
        else
        {
            if (s_ui_state_pages[i] != NULL)
            {
                lv_obj_add_flag(s_ui_state_pages[i], LV_OBJ_FLAG_HIDDEN);
                if (c_ui_page_create_funcs[i].hide_func != NULL)
                {
                    c_ui_page_create_funcs[i].hide_func();
                }
            }
        }
    }
}

// 根据当前选中的枪的状态, 来切换状态组页面
static void ui_page_main_updatejson(struct ioc_obj_user_data_s * user_data)
{
    // 未选中枪则显示HOME
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        ui_page_main_show_state_page(UI_PAGE_HOME);
        return;
    }

    // 否则按照选中枪的状态来显示
    ioc_ui_connector_state_t ui_state = UI_CONNECTOR_STATE(UI_CONNECTOR_ACTIVE_IDX);
    switch (ui_state)
    {
    case UI_CONNECTOR_STATE_HOME: ui_page_main_show_state_page(UI_PAGE_HOME); break;
    case UI_CONNECTOR_STATE_IDLE: ui_page_main_show_state_page(UI_PAGE_IDLE); break;
    case UI_CONNECTOR_STATE_STARTING1: ui_page_main_show_state_page(UI_PAGE_STARTING1); break;
    case UI_CONNECTOR_STATE_STARTING2: ui_page_main_show_state_page(UI_PAGE_STARTING2); break;
    case UI_CONNECTOR_STATE_CHARGING: ui_page_main_show_state_page(UI_PAGE_CHARTGING); break;
    case UI_CONNECTOR_STATE_FINISHED: ui_page_main_show_state_page(UI_PAGE_FINISHED); break;
    case UI_CONNECTOR_STATE_RESERVED: ui_page_main_show_state_page(UI_PAGE_RESERVED); break;
    case UI_CONNECTOR_STATE_SESSION_ERROR: ui_page_main_show_state_page(UI_PAGE_SESSION_ERROR); break;
    case UI_CONNECTOR_STATE_START_ERROR: ui_page_main_show_state_page(UI_PAGE_START_ERROR); break;
    case UI_CONNECTOR_STATE_FAULT: ui_page_main_show_state_page(UI_PAGE_FAULT); break;
    case UI_CONNECTOR_STATE_UNAVAILABLE: ui_page_main_show_state_page(UI_PAGE_UNAVAILABLE); break;
    case UI_CONNECTOR_STATE_CONNECTING: ui_page_main_show_state_page(UI_PAGE_CONNECTING); break;
    case UI_CONNECTOR_STATE_STOPPING: ui_page_main_show_state_page(UI_PAGE_STOPPING); break;
    case UI_CONNECTOR_STATE_WAIT_IN_LINE: ui_page_main_show_state_page(UI_PAGE_WAIT_IN_LINE); break;
    case UI_CONNECTOR_STATE_SUSPEND: ui_page_main_show_state_page(UI_PAGE_SUSPEND); break;
    case UI_CONNECTOR_STATE_PROCESSING: ui_page_main_show_state_page(UI_PAGE_PROCESSING); break;

    default: ui_page_main_show_state_page(UI_PAGE_HOME); break;
    }

    return;


}

lv_obj_t * ui_page_main_create(lv_obj_t *parent)
{
    if (s_ui_main != NULL)
    {
        lv_obj_del(s_ui_main);
        s_ui_main = NULL;
    }

    s_ui_main = ioc_layout_mngr_load_layout("main", parent);
    if (s_ui_main == NULL)
    {
        return NULL;
    }

    // 测试用
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(s_ui_main);
    user_data->ui_json_update_cb = ui_page_main_updatejson;

    // 创建状态栏
    s_ui_status_bar = ui_page_create_status_bar(s_ui_main);

    // 创建中间顶层容器(包含左上角枪号 枪状态 功率类型 等)
    s_ui_mid_container_top = ui_page_create_mid_container_top(s_ui_main);

    // 创建枪组
    s_ui_connector_group = ui_page_create_connector_group(s_ui_main);

    // 创建状态组, 初始为空 等待第一次UI.json更新
    memset(s_ui_state_pages, 0x00, sizeof(s_ui_state_pages));

    return s_ui_main;
}
