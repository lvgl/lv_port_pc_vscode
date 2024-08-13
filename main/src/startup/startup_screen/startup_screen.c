/*********************
 *      INCLUDES
 *********************/


#include "startup_screen.h"
#include "gui_comm.h"
#include "gui_data_comm.h"


static startup_screen_t* p_startup_screen = NULL;

extern void startup_language_start(app_index_t app_index);


static void startup_screen_scroll_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    if (LV_EVENT_PRESSING == event)
    {
        lv_indev_t* indev = lv_indev_get_act();
        lv_point_t p;

        lv_indev_get_vect(indev, &p);
        p_startup_screen->move_y += p.y;

        if (p_startup_screen->move_y < -100)
        {
            lv_obj_set_pos(p_startup_screen->scroll_img, 116, 263);
            lv_obj_align_to(p_startup_screen->scroll_label, p_startup_screen->scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
            startup_screen_stop();
        	startup_language_start(APP_STARTUP);
            return;
        }
        lv_obj_set_pos(p_startup_screen->scroll_img, 116, 263 + p_startup_screen->move_y);
        lv_obj_align_to(p_startup_screen->scroll_label, p_startup_screen->scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }    
    else if (LV_EVENT_RELEASED == event)
    {
        p_startup_screen->move_y = 0;
        lv_obj_set_pos(p_startup_screen->scroll_img, 116, 263);
        lv_obj_align_to(p_startup_screen->scroll_label, p_startup_screen->scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }
}
static void startup_screen_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* bg_img = lv_img_create(parent);
    lv_img_set_src(bg_img, gui_data_get_bg_src());
    lv_obj_center(bg_img);

    lv_obj_t* img_name = lv_img_create(parent);
    lv_img_set_src(img_name, &img_name);
    lv_obj_center(img_name);

    lv_obj_t* scroll_img = lv_img_create(parent);
    lv_img_set_src(scroll_img, &img_startup_arrow);
    lv_obj_set_pos(scroll_img, 116, 263);
    p_startup_screen->scroll_img = scroll_img;
	

    lv_obj_t* scroll_label = lv_label_create(parent);
    lv_label_set_text(scroll_label, "Swipe up to show apps");
    lv_obj_set_style_text_color(scroll_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(scroll_label, &lv_font_montserrat_12, 0);
    lv_obj_align_to(scroll_label, scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    p_startup_screen->scroll_label = scroll_label;

    lv_obj_t* scroll_obj = lv_obj_create(parent);
    lv_obj_set_size(scroll_obj, 240, 200);
    lv_obj_set_y(scroll_obj, 120);
    lv_obj_set_style_opa(scroll_obj, LV_OPA_TRANSP, 0);
    lv_obj_add_event_cb(scroll_obj, startup_screen_scroll_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(scroll_obj, LV_OBJ_FLAG_SCROLL_CHAIN);
}
void startup_screen_start(void)
{
    p_startup_screen = (startup_screen_t*)lv_malloc(sizeof(startup_screen_t));
    lv_memset(p_startup_screen, 0, sizeof(startup_screen_t));
    p_startup_screen->bg_cont = gui_comm_draw_bg();
    startup_screen_bg_cont(p_startup_screen->bg_cont);
}

void startup_screen_stop(void)
{
    lv_obj_del(p_startup_screen->bg_cont);
    lv_free(p_startup_screen);
    p_startup_screen = NULL;
}
