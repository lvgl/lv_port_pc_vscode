/*********************
 *      INCLUDES
 *********************/

#include "view_transaction_waiting.h"
#include "gui_comm.h"

extern void view_transaction_success_start(void);



static view_transaction_waiting_t* p_view_transaction_waiting = NULL;

static void timer_cb(lv_timer_t * tmr)
{
	p_view_transaction_waiting->angle = (p_view_transaction_waiting->angle + 100) % 3600;

	lv_img_set_angle(p_view_transaction_waiting->img_waiting, p_view_transaction_waiting->angle);
    if(p_view_transaction_waiting->angle == 3500)
    {
        lv_timer_del(p_view_transaction_waiting->timer);
        view_transaction_waiting_stop();
        view_transaction_success_start();
    }
}

static void view_transaction_waiting_bg_cont(lv_obj_t* parent)
{
	lv_obj_t* img = lv_img_create(parent);
	lv_img_set_src(img, &img_waiting);
    lv_obj_center(img);
	p_view_transaction_waiting->img_waiting = img;
	
	p_view_transaction_waiting->timer = lv_timer_create(timer_cb, 100, NULL);
}


void view_transaction_waiting_start(void)
{
    p_view_transaction_waiting = (view_transaction_waiting_t*)lv_malloc(sizeof(view_transaction_waiting_t));
    LV_ASSERT(p_view_transaction_waiting);
    lv_memset(p_view_transaction_waiting, 0, sizeof(view_transaction_waiting_t));

    p_view_transaction_waiting->bg_cont = gui_comm_draw_bg();
    view_transaction_waiting_bg_cont(p_view_transaction_waiting->bg_cont);
}

 void view_transaction_waiting_stop(void)
{
    lv_obj_del(p_view_transaction_waiting->bg_cont);
    lv_free(p_view_transaction_waiting);
    p_view_transaction_waiting = NULL;
}
