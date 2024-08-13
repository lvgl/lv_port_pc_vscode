/*********************
 *      INCLUDES
 *********************/

#include "startup_verification.h"
#include "gui_comm.h"
#include "gui_data_comm.h"

extern void menu_main_start(void);

static startup_verification_t* p_startup_verification = NULL;

static gui_comm_imgbtn_desc_t startup_imgbtn_num_table[] =
{
    {"word", 20, 81},
    {"word", 130, 81},
    {"word", 20, 111},
    {"word", 130, 111},
    {"word", 20, 164},
    {"word", 130, 164},
    {"word", 20, 194},
    {"word", 130, 194},
};

static void word_cb1(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
		if (0 == lv_strcmp(gui_data_get_word(p_startup_verification->page_id), e->user_data))
	        p_startup_verification->state |= WORD_VERIFACATION_STATE_1;
	    else 
	    	p_startup_verification->state &= ~WORD_VERIFACATION_STATE_1;
	    	
        printf("setting verification:%s  state:%x\n", (char*)e->user_data, p_startup_verification->state);
    }
}
static void word_cb2(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
		if (0 == lv_strcmp(gui_data_get_word(p_startup_verification->page_id + 1), e->user_data))
	        p_startup_verification->state |= WORD_VERIFACATION_STATE_2;
	    else 
	    	p_startup_verification->state &= ~WORD_VERIFACATION_STATE_2;
	    	
        printf("setting verification:%s  state:%x\n", (char*)e->user_data, p_startup_verification->state);
    }
}
static void continue_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	if(p_startup_verification->state != WORD_VERIFACATION_STATE_ALL)
    		return;

    	p_startup_verification->state = 0;
        p_startup_verification->page_id += 2;
        if (gui_data_get_word_num() == p_startup_verification->page_id + 2)
        {
			menu_main_start();
			startup_verification_stop();
            return;
        }
        lv_label_set_text_fmt(p_startup_verification->label_describe1, "Verification Word#%d", p_startup_verification->page_id + 1);
        lv_label_set_text_fmt(p_startup_verification->label_describe2, "Verification Word#%d", p_startup_verification->page_id + 2);
        
		for (uint8_t i = 0; i < sizeof(startup_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
		{
			if(i == 0)
			{
				lv_label_set_text(p_startup_verification->label_word[i], gui_data_get_word(p_startup_verification->page_id));
			}
			else if(i == 4)
			{
				lv_label_set_text(p_startup_verification->label_word[i], gui_data_get_word(p_startup_verification->page_id + 1));
			}
			else
			{
				lv_label_set_text(p_startup_verification->label_word[i], startup_imgbtn_num_table[i].str);
			}
			lv_obj_update_layout(p_startup_verification->label_word[i]);
		}
    }
}
static void startup_verification_bg_cont(lv_obj_t* parent)
{
    gui_comm_draw_title(parent, "Verification Phrase", NULL);

    lv_obj_t* label_describe1 = lv_label_create(parent);
	lv_obj_set_style_text_font(label_describe1, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(label_describe1, lv_color_hex(0xffffff), 0);
    lv_label_set_text_fmt(label_describe1, "Verification Word#%d", p_startup_verification->page_id + 1);
    lv_label_set_long_mode(label_describe1, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe1, 200);
    lv_obj_set_pos(label_describe1, 20, 60);
    p_startup_verification->label_describe1 = label_describe1;

    lv_obj_t* label_describe2 = lv_label_create(parent);
	lv_obj_set_style_text_font(label_describe2, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(label_describe2, lv_color_hex(0xffffff), 0);
    lv_label_set_text_fmt(label_describe2, "Verification Word#%d", p_startup_verification->page_id + 2);
    lv_label_set_long_mode(label_describe2, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label_describe2, 200);
    lv_obj_set_pos(label_describe2, 20, 143);
    p_startup_verification->label_describe2 = label_describe2;

    for (uint8_t i = 0; i < sizeof(startup_imgbtn_num_table) / sizeof(gui_comm_imgbtn_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_imagebutton_create(parent);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_888888_14x26, &img_mid_released_888888_4x26, &img_right_released_888888_14x26);
        lv_imagebutton_set_src(img_btn, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_pressed_bbbbbb_14x26, &img_mid_pressed_bbbbbb_4x26, &img_right_pressed_bbbbbb_14x26);
        lv_obj_set_width(img_btn, 90);
        lv_obj_set_pos(img_btn, startup_imgbtn_num_table[i].x, startup_imgbtn_num_table[i].y);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(img_btn);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);    
		lv_obj_set_style_text_align(label, LV_ALIGN_CENTER, 0);
        lv_obj_add_flag(label, LV_OBJ_FLAG_EVENT_BUBBLE);

        if(i < 4)
		{
			if(i == 0)
			{
				lv_label_set_text(label, gui_data_get_word(0));
				lv_obj_add_event_cb(img_btn, word_cb1, LV_EVENT_SHORT_CLICKED, gui_data_get_word(p_startup_verification->page_id));
			}
			else
			{
				lv_label_set_text(label, startup_imgbtn_num_table[i].str);
		        lv_obj_add_event_cb(img_btn, word_cb1, LV_EVENT_SHORT_CLICKED, (void *)startup_imgbtn_num_table[i].str);
			}
		}
        else
		{
			if(i == 4)
			{
				lv_label_set_text(label, gui_data_get_word(1));
	        	lv_obj_add_event_cb(img_btn, word_cb2, LV_EVENT_SHORT_CLICKED, gui_data_get_word(p_startup_verification->page_id + 1));
			}
			else
			{
				lv_label_set_text(label, startup_imgbtn_num_table[i].str);
		        lv_obj_add_event_cb(img_btn, word_cb2, LV_EVENT_SHORT_CLICKED, (void *)startup_imgbtn_num_table[i].str);
			}
		}
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_update_layout(label);
        p_startup_verification->label_word[i] = label;
    }

    lv_obj_t* img_btn_continue = lv_imagebutton_create(parent);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_RELEASED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_imagebutton_set_src(img_btn_continue, LV_IMAGEBUTTON_STATE_PRESSED, &img_left_released_6c6cf4_14x26, &img_mid_released_6c6cf4_4x26, &img_right_released_6c6cf4_14x26);
    lv_obj_set_width(img_btn_continue, 200);
    lv_obj_set_pos(img_btn_continue, 20, 278);
    lv_obj_add_event_cb(img_btn_continue, continue_cb, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(img_btn_continue, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t* label_continue = lv_label_create(img_btn_continue);
	lv_obj_set_style_text_font(label_continue, &lv_font_montserrat_12, 0);
	lv_obj_set_style_text_color(label_continue, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label_continue, "Contiune");
    lv_obj_align(label_continue, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label_continue, LV_OBJ_FLAG_EVENT_BUBBLE);
}


void startup_verification_start(void)
{
    p_startup_verification = (startup_verification_t*)lv_malloc(sizeof(startup_verification_t));
    LV_ASSERT(p_startup_verification);
    lv_memset(p_startup_verification, 0, sizeof(startup_verification_t));

    p_startup_verification->bg_cont = gui_comm_draw_bg();
    startup_verification_bg_cont(p_startup_verification->bg_cont);
}

void startup_verification_stop(void)
{
    lv_obj_del(p_startup_verification->bg_cont);
    lv_free(p_startup_verification);
    p_startup_verification = NULL;
}


