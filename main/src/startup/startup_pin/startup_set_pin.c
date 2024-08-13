/*********************
 *      INCLUDES
 *********************/

#include "startup_set_pin.h"
#include "gui_comm.h"
#include "gui_data_comm.h"

extern void startup_ready_check_start(void);
extern void startup_import_abort_start(void);
extern void settings_security_start(void);
extern void settings_pin_changed_start(void);




static startup_set_pin_t* p_startup_set_pin = NULL;

static void startup_wrong_pin_bg_cont(lv_obj_t* parent);

static void startup_keypad_ok_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (STARTUP_SET_PIN_MODE_SET == p_startup_set_pin->mode)
        {
            if (6 != lv_strlen(p_startup_set_pin->set_pin))
                return;

            p_startup_set_pin->mode = STARTUP_SET_PIN_MODE_CONFIRM;
            lv_label_set_text(p_startup_set_pin->title_label, "Confirm PIN");
            lv_obj_add_flag(p_startup_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
        }
        else if (STARTUP_SET_PIN_MODE_CONFIRM == p_startup_set_pin->mode)
        {
            if (0 == lv_strcmp(p_startup_set_pin->set_pin, p_startup_set_pin->confirm_pin))
            {
				gui_data_set_pin(p_startup_set_pin->set_pin);
				if(APP_STARTUP_CREATE_WALLET == p_startup_set_pin->app_index)
				{
					startup_ready_check_start();
				}
				else if(APP_STARTUP_IMPORT_WALLET == p_startup_set_pin->app_index)
				{
					startup_import_abort_start();
				}
				else if(APP_SETTINGS_PIN == p_startup_set_pin->app_index)
				{
					settings_pin_changed_start();
				}
				startup_set_pin_stop();
            }
            else
            {
			    if(APP_STARTUP_CREATE_WALLET == p_startup_set_pin->app_index || 
			    	APP_STARTUP_IMPORT_WALLET == p_startup_set_pin->app_index)
			    	lv_label_set_text(p_startup_set_pin->title_label, "Enter New Pin");
			    else if(APP_SETTINGS_PIN == p_startup_set_pin->app_index)
			    	lv_label_set_text(p_startup_set_pin->title_label, "Set New PIN");

                p_startup_set_pin->mode = STARTUP_SET_PIN_MODE_SET;
                lv_memset(p_startup_set_pin->set_pin, 0, sizeof(p_startup_set_pin->set_pin));
                lv_memset(p_startup_set_pin->confirm_pin, 0, sizeof(p_startup_set_pin->confirm_pin));
                lv_obj_add_flag(p_startup_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

static void startup_keypad_cancle_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (STARTUP_SET_PIN_MODE_SET == p_startup_set_pin->mode)
        {
            uint8_t ter_len = lv_strlen(p_startup_set_pin->set_pin);
            if(ter_len)
            {
	            p_startup_set_pin->set_pin[ter_len - 1] = 0;
	            lv_label_set_text(p_startup_set_pin->pin_label, p_startup_set_pin->set_pin);
	            if (ter_len == 1)
	                lv_obj_add_flag(p_startup_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
			}
        }
        else if (STARTUP_SET_PIN_MODE_CONFIRM == p_startup_set_pin->mode)
        {
            uint8_t ter_len = lv_strlen(p_startup_set_pin->confirm_pin);
            if(ter_len)
            {
	            p_startup_set_pin->confirm_pin[ter_len - 1] = 0;
	            lv_label_set_text(p_startup_set_pin->pin_label, p_startup_set_pin->confirm_pin);
	            if (ter_len == 1)
	                lv_obj_add_flag(p_startup_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
			}
        }
    }
}
static void startup_keypad_num_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_clear_flag(p_startup_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (STARTUP_SET_PIN_MODE_SET == p_startup_set_pin->mode)
        {
            if (6 > lv_strlen(p_startup_set_pin->set_pin))
            {
                lv_strcat(p_startup_set_pin->set_pin, (char*)(e->user_data));
                lv_label_set_text(p_startup_set_pin->pin_label, p_startup_set_pin->set_pin);
            }
        }
        else if (STARTUP_SET_PIN_MODE_CONFIRM == p_startup_set_pin->mode)
        {
            if (6 > lv_strlen(p_startup_set_pin->confirm_pin))
            {
                lv_strcat(p_startup_set_pin->confirm_pin, (char*)(e->user_data));
                lv_label_set_text(p_startup_set_pin->pin_label, p_startup_set_pin->confirm_pin);
            }
        }
    }
}

static void startup_set_pin_bg_cont(lv_obj_t* parent)
{
    lv_obj_clean(parent);
    gui_comm_draw_keypad_num(parent, startup_keypad_num_cb, startup_keypad_ok_cb, startup_keypad_cancle_cb);

    lv_obj_t* title_label = lv_label_create(parent);
	lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
	lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(title_label, 200);
    if(APP_STARTUP_CREATE_WALLET == p_startup_set_pin->app_index || 
    	APP_STARTUP_IMPORT_WALLET == p_startup_set_pin->app_index)
    	lv_label_set_text(title_label, "Enter New Pin");
    else if(APP_SETTINGS_PIN == p_startup_set_pin->app_index)
    	lv_label_set_text(title_label, "Set New PIN");
    	
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_update_layout(title_label);
    p_startup_set_pin->title_label = title_label;

    lv_obj_t* pin_label = lv_label_create(parent);
	lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_20, 0);
	lv_obj_set_style_text_color(pin_label, lv_color_hex(0xffffff), 0);
    lv_label_set_long_mode(pin_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(pin_label, 200);
    lv_label_set_text(pin_label, "1");
    lv_obj_set_style_text_align(pin_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(pin_label, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_add_flag(pin_label, LV_OBJ_FLAG_HIDDEN);
    p_startup_set_pin->pin_label = pin_label;
}

void startup_set_pin_start(app_index_t app_index)
{
    p_startup_set_pin = (startup_set_pin_t*)lv_malloc(sizeof(startup_set_pin_t));
    LV_ASSERT(p_startup_set_pin);
    lv_memset(p_startup_set_pin, 0, sizeof(startup_set_pin_t));

	p_startup_set_pin->app_index = app_index;
    p_startup_set_pin->bg_cont = gui_comm_draw_bg();
    startup_set_pin_bg_cont(p_startup_set_pin->bg_cont);
}

void startup_set_pin_stop(void)
{
    lv_obj_del(p_startup_set_pin->bg_cont);
    lv_free(p_startup_set_pin);
    p_startup_set_pin = NULL;
}


