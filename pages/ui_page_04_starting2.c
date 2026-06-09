#include "../pages_mngr.h"

#define UI_STARTING2_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.starting

#define UI_NAME_04_STARTING2_QR_CODE                            ".starting2_qr"
#define UI_NAME_04_STARTING2_QR_CODE_LABEL                      ".starting2_title"
#define UI_NAME_04_STARTING2_QR_CODE_PROMPT                     ".starting2_subtitle"
#define UI_NAME_04_STARTING2_POS_BTN                            ".starting2_pos_btn"

typedef struct {

    lv_obj_t * page;

    lv_obj_t * qr_code;
    lv_obj_t * qr_code_label;
    lv_obj_t * qr_code_prompt;
    lv_obj_t * pos_btn;
    uint8_t    auth_conidx;

} ui_page_04_starting2_t;

static ui_page_04_starting2_t ui_page_04_starting2 = {NULL};

static char s_starting2_qr_text[256] = {0};



static void ui_page_04_starting2_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    char buf[64] = {0};
    uint8_t startH = 0,  startM = 0, endH = 0, endM = 0;

    // 二维码文本有变化时更新二维码
    if(strcmp(s_starting2_qr_text, UI_STARTING2_DATA.connQR) != 0)
    {
        strcpy(s_starting2_qr_text, UI_STARTING2_DATA.connQR);
        lv_qrcode_update(ui_page_04_starting2.qr_code, UI_STARTING2_DATA.connQR, strlen(UI_STARTING2_DATA.connQR));
        LV_LOG_USER("QR Text [%s]", s_starting2_qr_text);
    }
}

lv_obj_t * ui_page_create_04_starting2(lv_obj_t *parent)
{
    memset(&ui_page_04_starting2, 0, sizeof(ui_page_04_starting2));

    ui_page_04_starting2.page = ioc_layout_mngr_load_layout("starting2", parent);
    if (ui_page_04_starting2.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_04_starting2.page);
        user_data->ui_json_update_cb = ui_page_04_starting2_updatejson;
    }

    return ui_page_04_starting2.page;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_starting2_update_qr_code_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_04_starting2.qr_code_label, "Scan_here_to_start_charging");
}

void ui_after_create_04_starting2_qr_code_label(const char * name, lv_obj_t * obj)
{
    ui_page_04_starting2.qr_code_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_starting2_update_qr_code_label;
}

void ui_starting2_update_qr_code_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_04_starting2.qr_code_prompt, "starting_prompt");
}

void ui_after_create_04_starting2_qr_code_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_04_starting2.qr_code_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_starting2_update_qr_code_prompt;
}

void ui_after_create_04_starting2_qr_code(const char * name, lv_obj_t * obj)
{
    ui_page_04_starting2.qr_code = obj;
}

static void pos_btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        ui_page_04_starting2.auth_conidx = UI_CONNECTOR_ACTIVE_IDX;
        lv_obj_t *btn = lv_event_get_target(e);
        lang_info_t *info = (lang_info_t *)lv_event_get_user_data(e);
        if ( g_ui_json.enSoftKey == false )
        {
            ioc_send_UseCreditCard(UI_CONNECTOR_ACTIVE_IDX,0);
        }
        else
        {
            ui_popup_show_preauthorization(UI_CONNECTOR_ACTIVE_IDX);
        }
    }
}

uint8_t ui_page_04_starting2_get_autu_conidx(void)
{
    return ui_page_04_starting2.auth_conidx;
}

void ui_after_create_04_starting2_pos_btn(const char * name, lv_obj_t * obj)
{
    ui_page_04_starting2.pos_btn = obj;
    lv_obj_add_event_cb(obj, pos_btn_event_cb, LV_EVENT_CLICKED, NULL);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_04_starting2(const char * name, lv_obj_t * obj)
{
    const char * starting2_name = name + strlen(UI_NAME_04_STARTING2);
    if (strcmp(starting2_name, UI_NAME_04_STARTING2_QR_CODE_LABEL) == 0)
    {
        ui_after_create_04_starting2_qr_code_label(name, obj);
        return;
    }
    else if (strcmp(starting2_name, UI_NAME_04_STARTING2_QR_CODE_PROMPT) == 0)
    {
        ui_after_create_04_starting2_qr_code_prompt(name, obj);
        return;
    }
    else if (strcmp(starting2_name, UI_NAME_04_STARTING2_QR_CODE) == 0)
    {
        ui_after_create_04_starting2_qr_code(name, obj);
        return;
    }
    else if (strcmp(starting2_name, UI_NAME_04_STARTING2_POS_BTN) == 0)
    {
        ui_after_create_04_starting2_pos_btn(name, obj);
        return;
    }
}
