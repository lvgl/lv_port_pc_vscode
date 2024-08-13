/*********************
 *      INCLUDES
 *********************/

#include "startup_import_word.h"
#include "gui_comm.h"
#include "gui_data_comm.h"

extern void startup_import_ready_start(void);

static startup_import_word_t* p_startup_import_word = NULL;

static void ok_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
		gui_data_set_word(p_startup_import_word->word_index, p_startup_import_word->word);

	    if(p_startup_import_word->word_index == gui_data_get_word_num())
	    {
	    	startup_import_word_stop();
	    	startup_import_ready_start();
	    	return;
	    }
		lv_obj_add_flag(p_startup_import_word->label_word, LV_OBJ_FLAG_HIDDEN);
		lv_memset(p_startup_import_word->word, 0, 32);
		p_startup_import_word->word_index++;    
		lv_label_set_text_fmt(p_startup_import_word->label_title, "Enter Word #%d", p_startup_import_word->word_index);
    }
    if (LV_EVENT_PRESSED == event)
    {
    	gui_comm_keypad_desc_t *desc = e->user_data;
    	lv_img_set_src(e->original_target, desc->src_pressed);
    	lv_img_set_zoom(e->original_target, 300);
    }
    else if (LV_EVENT_RELEASED == event)
    {
    	gui_comm_keypad_desc_t *desc = e->user_data;
    	lv_img_set_src(e->original_target, desc->src_released);
    	lv_img_set_zoom(e->original_target, 256);
    }
}

static void cancle_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        uint8_t ter_len = lv_strlen(p_startup_import_word->word);
        if(ter_len)
        {
	        p_startup_import_word->word[ter_len - 1] = 0;
	        lv_label_set_text(p_startup_import_word->label_word, p_startup_import_word->word);

	        if (ter_len == 1)
	            lv_obj_add_flag(p_startup_import_word->label_word, LV_OBJ_FLAG_HIDDEN);
	    }
    }
    if (LV_EVENT_PRESSED == event)
    {
    	gui_comm_keypad_desc_t *desc = e->user_data;
    	lv_img_set_src(e->original_target, desc->src_pressed);
    	lv_img_set_zoom(e->original_target, 300);
    }
    else if (LV_EVENT_RELEASED == event)
    {
    	gui_comm_keypad_desc_t *desc = e->user_data;
    	lv_img_set_src(e->original_target, desc->src_released);
    	lv_img_set_zoom(e->original_target, 256);
    }
}
static void num_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_PRESSED == event)
    {
    	gui_comm_keypad_desc_t *desc = e->user_data;
    	lv_img_set_src(e->original_target, desc->src_pressed);
    	lv_img_set_zoom(e->original_target, 300);
    }
    else if (LV_EVENT_RELEASED == event)
    {
    	gui_comm_keypad_desc_t *desc = e->user_data;
    	lv_img_set_src(e->original_target, desc->src_released);
    	lv_img_set_zoom(e->original_target, 256);
    }
    else if (LV_EVENT_SHORT_CLICKED == event)
    {	
    	gui_comm_keypad_desc_t *desc = e->user_data;
		lv_strcat(p_startup_import_word->word, desc->str);
		lv_label_set_text(p_startup_import_word->label_word, p_startup_import_word->word);
		printf("%s \n", p_startup_import_word->word);
		lv_obj_clear_flag(p_startup_import_word->label_word, LV_OBJ_FLAG_HIDDEN);
    }
}

static void startup_import_word_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* img_succ = lv_img_create(parent);
	lv_img_set_src(img_succ, &img_guide_back);
    lv_obj_set_pos(img_succ, 20, 8);
    
    lv_obj_t* title_label = lv_label_create(parent);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(title_label, 200);
    lv_obj_set_pos(title_label, 20, 30);
    lv_label_set_text_fmt(title_label, "Enter Word #%d", p_startup_import_word->word_index);
    p_startup_import_word->label_title = title_label;
    
	lv_obj_t *bg_obj = lv_obj_create(parent);
	lv_obj_remove_style_all(bg_obj);
	lv_obj_set_scrollbar_mode(bg_obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_bg_color(bg_obj, lv_color_hex(0x888888), LV_PART_MAIN);
	lv_obj_set_style_bg_opa(bg_obj, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_radius(bg_obj, 6, LV_PART_MAIN);
	lv_obj_set_pos(bg_obj, 20, 66);
	lv_obj_set_size(bg_obj, 200, 100);
	
    lv_obj_t* label_word = lv_label_create(parent);
    lv_obj_set_style_text_color(label_word, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(label_word, &lv_font_montserrat_20, 0);
    lv_label_set_long_mode(label_word, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_word, 200);
    lv_obj_set_pos(label_word, 30, 76);
	lv_obj_add_flag(label_word, LV_OBJ_FLAG_HIDDEN);
    p_startup_import_word->label_word = label_word;
    
    gui_comm_draw_keypad_letter(parent, num_cb, ok_cb, cancle_cb);
}


void startup_import_word_start(void)
{
    p_startup_import_word = (startup_import_word_t*)lv_malloc(sizeof(startup_import_word_t));
    LV_ASSERT(p_startup_import_word);
    lv_memset(p_startup_import_word, 0, sizeof(startup_import_word_t));
	p_startup_import_word->word_index = 1;

    p_startup_import_word->bg_cont = gui_comm_draw_bg();
    startup_import_word_bg_cont(p_startup_import_word->bg_cont);
}

void startup_import_word_stop(void)
{
    lv_obj_del(p_startup_import_word->bg_cont);
    lv_free(p_startup_import_word);
    p_startup_import_word = NULL;
}


