#include "../pages_mngr.h"

#define UI_STARTING1_DATA  g_ui_json.connectors[UI_CONNECTOR_ACTIVE_IDX].state_data.starting


#define UI_NAME_03_STARTING1_QR_CODE                            ".starting1_qr"
#define UI_NAME_03_STARTING1_QR_CODE_LABEL                      ".starting1_title"
#define UI_NAME_03_STARTING1_QR_CODE_PROMPT                     ".starting1_subtitle"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * qr_code;
    lv_obj_t * qr_code_label;
    lv_obj_t * qr_code_prompt;

} ui_page_03_starting1_t;

static ui_page_03_starting1_t ui_page_03_starting1 = {NULL};
static char s_starting1_qr_text[256] = {0};



static void ui_page_03_starting1_updatejson(struct ioc_obj_user_data_s * user_data)
{
    if (UI_CONNECTOR_ACTIVE_IDX < 0 || UI_CONNECTOR_ACTIVE_IDX >= UI_NUMOFCONNECTOR)
    {
        return;
    }

    // 二维码文本有变化时更新二维码
    if(strcmp(s_starting1_qr_text, UI_STARTING1_DATA.connQR) != 0)
    {
        strcpy(s_starting1_qr_text, UI_STARTING1_DATA.connQR);
        lv_qrcode_update(ui_page_03_starting1.qr_code, UI_STARTING1_DATA.connQR, strlen(UI_STARTING1_DATA.connQR));
        LV_LOG_USER("QR Text [%s]", s_starting1_qr_text);
    }
}

lv_obj_t * ui_page_create_03_starting1(lv_obj_t *parent)
{
    memset(&ui_page_03_starting1, 0, sizeof(ui_page_03_starting1));

    ui_page_03_starting1.page = ioc_layout_mngr_load_layout("starting1", parent);
    if (ui_page_03_starting1.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_03_starting1.page);
        user_data->ui_json_update_cb = ui_page_03_starting1_updatejson;
    }

    return ui_page_03_starting1.page;
}

void ui_starting1_update_qr_code_label(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_03_starting1.qr_code_label, "Scan_here_to_start_charging");
}

void ui_after_create_03_starting1_qr_code_label(const char * name, lv_obj_t * obj)
{
    ui_page_03_starting1.qr_code_label = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_starting1_update_qr_code_label;
}

void ui_starting1_update_qr_code_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_03_starting1.qr_code_prompt, "starting_prompt");
}

void ui_after_create_03_starting1_qr_code_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_03_starting1.qr_code_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_starting1_update_qr_code_prompt;
}

void ui_after_create_03_starting1_qr_code(const char * name, lv_obj_t * obj)
{
    ui_page_03_starting1.qr_code = obj;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_03_starting1(const char * name, lv_obj_t * obj)
{
    const char * starting1_name = name + strlen(UI_NAME_03_STARTING1);
    if (strcmp(starting1_name, UI_NAME_03_STARTING1_QR_CODE_LABEL) == 0)
    {
        ui_after_create_03_starting1_qr_code_label(name, obj);
        return;
    }
    else if (strcmp(starting1_name, UI_NAME_03_STARTING1_QR_CODE_PROMPT) == 0)
    {
        ui_after_create_03_starting1_qr_code_prompt(name, obj);
        return;
    }
    else if (strcmp(starting1_name, UI_NAME_03_STARTING1_QR_CODE) == 0)
    {
        ui_after_create_03_starting1_qr_code(name, obj);
        return;
    }
}
