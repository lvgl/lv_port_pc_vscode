/*********************
 *      INCLUDES
 *********************/

#include "guide_set_pin.h"
#include "guide_comm.h"


static guide_set_pin_t* p_guide_set_pin = NULL;

static void guide_wrong_pin_bg_cont(lv_obj_t* parent);

static void guide_keypad_ok_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (GUIDE_SET_PIN_MODE_SET == p_guide_set_pin->mode)
        {
            if (6 != lv_strlen(p_guide_set_pin->set_pin))
                return;

            p_guide_set_pin->mode = GUIDE_SET_PIN_MODE_CONFIRM;
            lv_label_set_text(p_guide_set_pin->title_label, "Confirm PIN");
            lv_obj_add_flag(p_guide_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
        }
        else if (GUIDE_SET_PIN_MODE_CONFIRM == p_guide_set_pin->mode)
        {
            if (0 == lv_strcmp(p_guide_set_pin->set_pin, p_guide_set_pin->confirm_pin))
            {
                //gui_app_run_subpage("guide", "quick_start", NULL);
                guide_set_pin_stop();
        		guide_quick_start_start();
            }
            else
            {
                lv_label_set_text(p_guide_set_pin->title_label, "Set PIN");
                p_guide_set_pin->mode = GUIDE_SET_PIN_MODE_SET;
                lv_memset(p_guide_set_pin->set_pin, 0, sizeof(p_guide_set_pin->set_pin));
                lv_memset(p_guide_set_pin->confirm_pin, 0, sizeof(p_guide_set_pin->confirm_pin));
                lv_obj_add_flag(p_guide_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

static void guide_keypad_cancle_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (GUIDE_SET_PIN_MODE_SET == p_guide_set_pin->mode)
        {
            uint8_t ter_len = lv_strlen(p_guide_set_pin->set_pin);
            p_guide_set_pin->set_pin[ter_len - 1] = 0;
            lv_label_set_text(p_guide_set_pin->pin_label, p_guide_set_pin->set_pin);

            if (ter_len == 1)
                lv_obj_add_flag(p_guide_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
        }
        else if (GUIDE_SET_PIN_MODE_CONFIRM == p_guide_set_pin->mode)
        {
            uint8_t ter_len = lv_strlen(p_guide_set_pin->confirm_pin);
            p_guide_set_pin->confirm_pin[ter_len - 1] = 0;
            lv_label_set_text(p_guide_set_pin->pin_label, p_guide_set_pin->confirm_pin);

            if (ter_len == 1)
                lv_obj_add_flag(p_guide_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
static void guide_keypad_num_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_clear_flag(p_guide_set_pin->pin_label, LV_OBJ_FLAG_HIDDEN);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        if (GUIDE_SET_PIN_MODE_SET == p_guide_set_pin->mode)
        {
            if (6 > lv_strlen(p_guide_set_pin->set_pin))
            {
                lv_strcat(p_guide_set_pin->set_pin, (char*)(e->user_data));
                lv_label_set_text(p_guide_set_pin->pin_label, p_guide_set_pin->set_pin);
            }
        }
        else if (GUIDE_SET_PIN_MODE_CONFIRM == p_guide_set_pin->mode)
        {
            if (6 > lv_strlen(p_guide_set_pin->confirm_pin))
            {
                lv_strcat(p_guide_set_pin->confirm_pin, (char*)(e->user_data));
                lv_label_set_text(p_guide_set_pin->pin_label, p_guide_set_pin->confirm_pin);
            }
        }
    }
}

static void guide_set_pin_bg_cont(lv_obj_t* parent)
{
    lv_obj_clean(parent);
    guide_draw_keypad(parent, guide_keypad_num_cb, guide_keypad_ok_cb, guide_keypad_cancle_cb);

    lv_obj_t* title_label = lv_label_create(parent);
	lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
	lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(title_label, 200);
    lv_label_set_text(title_label, "Set PIN");
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_update_layout(title_label);
    p_guide_set_pin->title_label = title_label;

    lv_obj_t* pin_label = lv_label_create(parent);
	lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_20, 0);
	lv_obj_set_style_text_color(pin_label, lv_color_hex(0xffffff), 0);
    lv_label_set_long_mode(pin_label, LV_LABEL_LONG_DOT);
    lv_obj_set_width(pin_label, 200);
    lv_label_set_text(pin_label, "1");
    lv_obj_set_style_text_align(pin_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(pin_label, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_add_flag(pin_label, LV_OBJ_FLAG_HIDDEN);
    p_guide_set_pin->pin_label = pin_label;
}

void guide_set_pin_start(void)
{
    p_guide_set_pin = (guide_set_pin_t*)lv_malloc(sizeof(guide_set_pin_t));
    LV_ASSERT(p_guide_set_pin);
    lv_memset(p_guide_set_pin, 0, sizeof(guide_set_pin_t));

    p_guide_set_pin->bg_cont = guide_draw_bg_cont();
    guide_set_pin_bg_cont(p_guide_set_pin->bg_cont);
}

void guide_set_pin_stop(void)
{
    lv_obj_del(p_guide_set_pin->bg_cont);
    lv_free(p_guide_set_pin);
    p_guide_set_pin = NULL;
}

