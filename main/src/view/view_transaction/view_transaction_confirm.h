/*********************
 *      INCLUDES
 *********************/
#ifndef __VIEW_TRANSACTION_CONFIRM_H__
#define __VIEW_TRANSACTION_CONFIRM_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t *bg_cont;
	lv_obj_t *slider;
	lv_timer_t *timer;
} view_transaction_confirm_t;

extern void view_transaction_confirm_start(void);
extern void view_transaction_confirm_stop(void);

#endif /* __VIEW_TRANSACTION_CONFIRM_H__ */


