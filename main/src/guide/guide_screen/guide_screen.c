/*********************
 *      INCLUDES
 *********************/


#include "guide_screen.h"
#include "guide_comm.h"

static guide_screen_t* p_guide_screen = NULL;

static void guide_screen_scroll_cb(lv_event_t* e)
{
    lv_event_code_t event = lv_event_get_code(e);
    //printf("event: %s\n", lv_event_code_to_name(event));
    if (LV_EVENT_PRESSING == event)
    {
        lv_indev_t* indev = lv_indev_get_act();
        lv_point_t p;

        lv_indev_get_vect(indev, &p);
        p_guide_screen->move_y += p.y;


        if (p_guide_screen->move_y < -100)
        {
            //gui_app_run_subpage("guide", "set_pin", NULL);
            lv_obj_set_pos(p_guide_screen->scroll_img, 116, 263);
            lv_obj_align_to(p_guide_screen->scroll_label, p_guide_screen->scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
            guide_screen_stop();
        	guide_enter_pin_start();
        	//guide_verification_start();
            return;
        }
        lv_obj_set_pos(p_guide_screen->scroll_img, 116, 263 + p_guide_screen->move_y);
        lv_obj_align_to(p_guide_screen->scroll_label, p_guide_screen->scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }    
    else if (LV_EVENT_RELEASED == event)
    {
        p_guide_screen->move_y = 0;
        lv_obj_set_pos(p_guide_screen->scroll_img, 116, 263);
        lv_obj_align_to(p_guide_screen->scroll_label, p_guide_screen->scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    }
}
static void guide_screen_bg_cont(lv_obj_t* parent)
{
    lv_obj_t* bg_img = lv_img_create(parent);
    lv_img_set_src(bg_img, &img_backgroud);
    lv_obj_center(bg_img);

    lv_obj_t* scroll_img = lv_img_create(parent);
    lv_img_set_src(scroll_img, &img_screen_shape);
    lv_obj_set_pos(scroll_img, 116, 263);
    p_guide_screen->scroll_img = scroll_img;

    lv_obj_t* scroll_label = lv_label_create(parent);
    lv_label_set_text(scroll_label, "Swipe up to show apps");
    lv_obj_set_style_text_color(scroll_label, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(scroll_label, &lv_font_montserrat_12, 0);
    lv_obj_align_to(scroll_label, scroll_img, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    p_guide_screen->scroll_label = scroll_label;

    lv_obj_t* scroll_obj = lv_obj_create(parent);
    lv_obj_set_size(scroll_obj, 240, 200);
    lv_obj_set_y(scroll_obj, 120);
    lv_obj_set_style_opa(scroll_obj, LV_OPA_TRANSP, 0);
    lv_obj_add_event_cb(scroll_obj, guide_screen_scroll_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(scroll_obj, LV_OBJ_FLAG_SCROLL_CHAIN);
}
void guide_screen_start(void)
{
    p_guide_screen = (guide_screen_t*)lv_malloc(sizeof(guide_screen_t));
    lv_memset(p_guide_screen, 0, sizeof(guide_screen_t));
    p_guide_screen->bg_cont = guide_draw_bg_cont();
    guide_screen_bg_cont(p_guide_screen->bg_cont);
}

void guide_screen_stop(void)
{
    lv_obj_del(p_guide_screen->bg_cont);
    lv_free(p_guide_screen);
    p_guide_screen = NULL;
}
