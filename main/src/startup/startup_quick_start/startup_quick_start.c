/*********************
 *      INCLUDES
 *********************/

#include "startup_quick_start.h"
#include "gui_comm.h"

extern void startup_set_pin_start(app_index_t app_index);
extern void startup_language_start(app_index_t app_index);

static startup_quick_start_t* p_startup_quick_start = NULL;

static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        startup_quick_start_stop();
        startup_language_start(APP_STARTUP);
    }
}

static void startup_create_wallet_event_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        startup_quick_start_stop();
        startup_set_pin_start(APP_STARTUP_CREATE_WALLET);
    }
}

static void startup_import_wallet_event_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        startup_quick_start_stop();
        startup_set_pin_start(APP_STARTUP_IMPORT_WALLET);
    }
}

static void startup_quick_start_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Quick Start", title_cb);

    lv_obj_t* describe_label = lv_label_create(parent);
    lv_obj_set_style_text_color(describe_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(describe_label, &lv_font_montserrat_12, 0);
    lv_label_set_long_mode(describe_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(describe_label, 200);
    lv_obj_set_pos(describe_label, 20, 60);
    lv_label_set_text(describe_label, "Create a new wallet, or restore a previously used wallet from recovery phrase backup.");
    lv_obj_add_flag(describe_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_create = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_create, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(img_btn_create, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(img_btn_create, 200);
    lv_obj_set_pos(img_btn_create, 20, 246);
    lv_obj_add_event_cb(img_btn_create, startup_create_wallet_event_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_create, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* create_label = lv_label_create(img_btn_create);
    lv_obj_set_style_text_color(create_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(create_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(create_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(create_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(create_label, 200);
    lv_label_set_text(create_label, "Create Wallet");
    lv_obj_center(create_label);
    lv_obj_add_flag(create_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t* img_btn_import = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_import, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
    lv_imagebutton_set_src(img_btn_import, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
    lv_obj_set_width(img_btn_import, 200);
    lv_obj_set_pos(img_btn_import, 20, 278);
    lv_obj_add_event_cb(img_btn_import, startup_import_wallet_event_handler, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_import, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* import_label = lv_label_create(img_btn_import);
    lv_obj_set_style_text_color(import_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(import_label, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_align(import_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(import_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(import_label, 200);
    lv_label_set_text(import_label, "Import Wallet");
    lv_obj_center(import_label);
    lv_obj_add_flag(import_label, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_EVENT_BUBBLE);
}


void startup_quick_start_start(void)
{
    p_startup_quick_start = (startup_quick_start_t*)lv_malloc(sizeof(startup_quick_start_t));
    LV_ASSERT(p_startup_quick_start);
    lv_memset(p_startup_quick_start, 0, sizeof(startup_quick_start_t));

    p_startup_quick_start->bg_cont = gui_comm_draw_bg();
    startup_quick_start_bg_cont(p_startup_quick_start->bg_cont);
}

void startup_quick_start_stop(void)
{
    lv_obj_del(p_startup_quick_start->bg_cont);
    lv_free(p_startup_quick_start);
    p_startup_quick_start = NULL;
}

