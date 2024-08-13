/*********************
 *      INCLUDES
 *********************/

#include "general_homescreen.h"
#include "gui_comm.h"


static gui_comm_img_desc_t general_homescreen_img_table[] =
{
    {"1", 8, 60, &img_bg1_small},
    {"2", 84, 60, &img_bg2_small},
    {"3", 160, 60, &img_bg3_small},
    {"4", 8, 160, &img_bg4_small},
    {"5", 84, 160, &img_bg5_small},
    {"6", 160, 160, &img_bg6_small},
    {"7", 8, 260, &img_bg7_small},
};

extern void general_main_start(void);

static general_homescreen_t* p_general_homescreen = NULL;


static void title_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
	    general_homescreen_stop();
	    general_main_start();
    }
}

static void bg_img_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);

    if (LV_EVENT_SHORT_CLICKED == event)
    {
    	p_general_homescreen->bg_img_src_choosen = e->user_data;
		lv_obj_align_to(p_general_homescreen->img_choosen, e->original_target, LV_ALIGN_TOP_LEFT, 49, 8);
    }
}

static void general_homescreen_bg_cont(lv_obj_t* parent)
{
	gui_comm_draw_title(parent, "Homescreen", title_cb);

    for (uint8_t i = 0; i < sizeof(general_homescreen_img_table) / sizeof(gui_comm_img_desc_t); i++)
    {
        lv_obj_t* img_bg = lv_img_create(parent);
        lv_img_set_src(img_bg, general_homescreen_img_table[i].src);
        lv_obj_set_pos(img_bg, general_homescreen_img_table[i].x, general_homescreen_img_table[i].y);
        lv_obj_add_event_cb(img_bg, bg_img_cb, LV_EVENT_SHORT_CLICKED, (void *)general_homescreen_img_table[i].src);
        lv_obj_add_flag(img_bg, LV_OBJ_FLAG_CLICKABLE);
		printf("%p\n", general_homescreen_img_table[i].src);

		if(p_general_homescreen->bg_img_src_choosen == general_homescreen_img_table[i].src)
		{
	        lv_obj_t* img_choosen = lv_img_create(parent);
	        lv_img_set_src(img_choosen, &img_screen_shoosen);
	        lv_obj_align_to(img_choosen, img_bg, LV_ALIGN_TOP_LEFT, 49, 8);
	        p_general_homescreen->img_choosen = img_choosen;
        }
    }
	lv_obj_move_foreground(p_general_homescreen->img_choosen);
}


void general_homescreen_start(void)
{
    p_general_homescreen = (general_homescreen_t*)lv_malloc(sizeof(general_homescreen_t));
    LV_ASSERT(p_general_homescreen);
    lv_memset(p_general_homescreen, 0, sizeof(general_homescreen_t));
	p_general_homescreen->bg_img_src_choosen = (void *)&img_bg1_small;
	printf("%p\n", p_general_homescreen->bg_img_src_choosen);

    p_general_homescreen->bg_cont = gui_comm_draw_bg();
	lv_obj_add_flag(p_general_homescreen->bg_cont, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC);
    general_homescreen_bg_cont(p_general_homescreen->bg_cont);
}

void general_homescreen_stop(void)
{
    lv_obj_del(p_general_homescreen->bg_cont);
    lv_free(p_general_homescreen);
    p_general_homescreen = NULL;
}


