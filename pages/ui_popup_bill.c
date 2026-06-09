#include "../pages_mngr.h"

#define UI_NAME_BILL_QR_CODE_BILL               ".bill_qr_code_bill"
#define UI_NAME_BILL_QR_CODE_RECEIPT            ".bill_qr_code_receipt"
#define UI_NAME_BILL_CONTENT                    ".bill_content.content"

#define UI_NAME_BILL_SCAN_FOR_BILL              ".bill_scan_for_bill_title"
#define UI_NAME_BILL_SCAN_FOR_RECEIPT           ".bill_qr_code_receipt"
#define UI_NAME_BILL_CLOSE_TEXT                 ".bill_close_button.text"
#define UI_NAME_BILL_CLOSE_BTN                  ".bill_close_button"
#define UI_NAME_BILL_CLOSE_SECONDS              ".bill_close_button.seconds"

#define BILL_CLOSE_TIMER_COUNT 60
typedef struct {

    lv_obj_t * obj;

    lv_obj_t * bill_content;
    lv_obj_t * qr_code_bill;
    lv_obj_t * qr_code_receipt;
    lv_obj_t * scan_for_bill;
    lv_obj_t * scan_for_receipt;
    lv_obj_t * close_text;
    lv_obj_t * close_btn;
    lv_obj_t * close_seconds;

    lv_timer_t * close_timer;
    uint16_t     close_time_counter;
}bill_page_t;

bill_page_t bill_page;

static void ui_bill_clear(void)
{
    memset(g_ui_json.trigger.billUrlStr, 0x00, sizeof(g_ui_json.trigger.billUrlStr));
    memset(g_ui_json.trigger.receiptUrlStr, 0x00, sizeof(g_ui_json.trigger.receiptUrlStr));
    memset(g_ui_json.trigger.content, 0x00, sizeof(g_ui_json.trigger.content));
}


static void ui_bill_show_receipt(bool is_show)
{
    if(is_show) {
        lv_obj_clear_flag(bill_page.scan_for_receipt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(bill_page.qr_code_receipt, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(bill_page.scan_for_receipt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(bill_page.qr_code_receipt, LV_OBJ_FLAG_HIDDEN);
    }
}

// 显示某枪的账单弹出信息, 内容由ui_json提供
void ui_popup_show_bill() {

    ui_popup_show_mask(true);   // 显示遮罩层

    if ( bill_page.obj == NULL ) {
        bill_page.obj = ioc_layout_mngr_load_layout("bill", lv_layer_top());
    }

    if ( bill_page.obj != NULL ) {
        lv_obj_clear_flag(bill_page.obj, LV_OBJ_FLAG_HIDDEN);
    }

    ioc_label_set_text(bill_page.scan_for_bill, "Scan_for_bill");
    ioc_label_set_text(bill_page.scan_for_receipt, "Scan_for_receipt");
    ioc_label_set_text(bill_page.close_text, "Close");

    bill_page.close_time_counter = g_ui_json.trigger.seconds;

    lv_qrcode_update(bill_page.qr_code_bill, g_ui_json.trigger.billUrlStr, strlen(g_ui_json.trigger.billUrlStr));

    if(strlen(g_ui_json.trigger.receiptUrlStr) != 0)
    {
        lv_qrcode_update(bill_page.qr_code_receipt, g_ui_json.trigger.receiptUrlStr, strlen(g_ui_json.trigger.receiptUrlStr));
    }

    ioc_label_set_text(bill_page.bill_content, g_ui_json.trigger.content);

    // 打开定时器
    if (bill_page.close_timer != NULL) {
        lv_timer_resume(bill_page.close_timer);
    }
}

void ui_popup_hide_bill()
{
    bill_page.close_time_counter = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_bill_scan_for_bill(const char * name, lv_obj_t * obj)
{
    bill_page.scan_for_bill = obj;
}

void ui_after_create_bill_scan_for_receipt(const char * name, lv_obj_t * obj)
{
    bill_page.scan_for_receipt = obj;
}

void ui_after_create_bill_close_text(const char * name, lv_obj_t * obj)
{
    bill_page.close_text = obj;
}

static void bill_close_timer_cb(lv_timer_t * t) {

    if (bill_page.close_time_counter == 0) {
        lv_obj_add_flag(bill_page.obj, LV_OBJ_FLAG_HIDDEN);
        ui_popup_show_mask(false);
        lv_timer_pause(bill_page.close_timer);
        bill_page.close_time_counter = BILL_CLOSE_TIMER_COUNT;
        ui_bill_clear();
        return;
    }

    bill_page.close_time_counter--;
    char buf[8];
    snprintf(buf, sizeof(buf), "(%ds)", bill_page.close_time_counter);
    ioc_label_set_text(bill_page.close_seconds, buf);
}


static void bill_close_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_add_flag(bill_page.obj, LV_OBJ_FLAG_HIDDEN);
        ui_popup_show_mask(false);
        ui_bill_clear();
    }
}

void ui_after_create_bill_close_btn(const char * name, lv_obj_t * obj)
{
    bill_page.close_btn = obj;
    lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(obj, 5, 0);
    lv_obj_add_event_cb(obj, bill_close_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_bill_close_seconds(const char * name, lv_obj_t * obj)
{
    bill_page.close_seconds = obj;
}

void ui_after_create_bill_qr_code_bill(const char * name, lv_obj_t * obj)
{
    bill_page.qr_code_bill = obj;
}
void ui_after_create_bill_qr_code_receipt(const char * name, lv_obj_t * obj)
{
    bill_page.qr_code_receipt = obj;
}

void ui_after_create_bill_content(const char * name, lv_obj_t * obj)
{
    bill_page.bill_content = obj;
}

///////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_bill(const char * name, lv_obj_t * obj)
{
    const char * bill_name = name + strlen(UI_NAME_BILL);
    if (strcmp(bill_name, UI_NAME_BILL_SCAN_FOR_BILL) == 0)
    {
        ui_after_create_bill_scan_for_bill(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_SCAN_FOR_RECEIPT) == 0)
    {
        ui_after_create_bill_scan_for_receipt(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_CLOSE_TEXT) == 0)
    {
        ui_after_create_bill_close_text(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_CLOSE_BTN) == 0)
    {
        ui_after_create_bill_close_btn(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_CLOSE_SECONDS) == 0)
    {
        ui_after_create_bill_close_seconds(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_QR_CODE_BILL) == 0)
    {
        ui_after_create_bill_qr_code_bill(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_QR_CODE_RECEIPT) == 0)
    {
        ui_after_create_bill_qr_code_receipt(name, obj);
        return;
    }
    else if (strcmp(bill_name, UI_NAME_BILL_CONTENT) == 0)
    {
        ui_after_create_bill_content(name, obj);
        return;
    }

    if ( bill_page.close_timer == NULL )
    {
        bill_page.close_time_counter = BILL_CLOSE_TIMER_COUNT;
        bill_page.close_timer = lv_timer_create(bill_close_timer_cb, 1000, NULL);
        lv_timer_pause(bill_page.close_timer);
    }
}
