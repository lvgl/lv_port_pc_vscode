/*********************
 *      INCLUDES
 *********************/
#ifndef __VIEW_TRANSACTION_WAITING_H__
#define __VIEW_TRANSACTION_WAITING_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
	lv_obj_t* img_waiting;
	lv_timer_t *timer;

	uint16_t angle;
} view_transaction_waiting_t;

extern void view_transaction_waiting_start(void);
extern void view_transaction_waiting_stop(void);

#endif /* __VIEW_TRANSACTION_WAITING_H__ */


