/*********************
 *      INCLUDES
 *********************/

#include "guide_menu.h"
#include "guide_comm.h"

static guide_menu_t* p_guide_menu = NULL;

static guide_img_desc_t guide_menu_img_table[] =
{
    {"General", 21, 30, &img_menu_general},
    {"Guide", 126, 30, &img_menu_guide},
    {"Settings", 21, 153, &img_menu_guide},
    {"Connect App Wallet", 126, 153, &img_menu_connect},
};

static void guide_menu_word_handler(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
        printf("setting menu:%s\n", (char*)e->user_data);
        
		guide_menu_stop();
		if(0 == lv_strcmp((char*)e->user_data, "Connect App Wallet"))
		{
			guide_connect_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "General"))
		{
			guide_general_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Guide"))
		{
			guide_guide_start();
		}
		else if(0 == lv_strcmp((char*)e->user_data, "Settings"))
		{
			guide_settings_start();
		}
//        if(0 == lv_strcmp((char*)e->user_data, "Connect App Wallet"))
//            gui_app_run_subpage("guide", "Connect", NULL);
//        else
//            gui_app_run_subpage("guide", (char*)e->user_data, NULL);
    }
}

static void guide_menu_bg_cont(lv_obj_t* parent)
{
    for (uint8_t i = 0; i < sizeof(guide_menu_img_table) / sizeof(guide_img_desc_t); i++)
    {
        lv_obj_t* img_btn = lv_img_create(parent);
        lv_img_set_src(img_btn, guide_menu_img_table[i].src);
        lv_obj_set_pos(img_btn, guide_menu_img_table[i].x, guide_menu_img_table[i].y);
        lv_obj_add_event_cb(img_btn, guide_menu_word_handler, LV_EVENT_SHORT_CLICKED, (void *)guide_menu_img_table[i].str);
        lv_obj_add_flag(img_btn, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t* label = lv_label_create(parent);
		lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(label, 96);
        lv_label_set_text(label, guide_menu_img_table[i].str);
        lv_obj_align_to(label, img_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 6);
        lv_obj_update_layout(label);
    }
}


void guide_menu_start(void)
{
    p_guide_menu = (guide_menu_t*)lv_malloc(sizeof(guide_menu_t));
    LV_ASSERT(p_guide_menu);
    lv_memset(p_guide_menu, 0, sizeof(guide_menu_t));

    p_guide_menu->bg_cont = guide_draw_bg_cont();
    guide_menu_bg_cont(p_guide_menu->bg_cont);
}

void guide_menu_stop(void)
{
    lv_obj_del(p_guide_menu->bg_cont);
    lv_free(p_guide_menu);
    p_guide_menu = NULL;
}


