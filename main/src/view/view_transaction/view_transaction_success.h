/*********************
 *      INCLUDES
 *********************/
#ifndef __VIEW_TRANSACTION_SUCCESS_H__
#define __VIEW_TRANSACTION_SUCCESS_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} view_transaction_success_t;

extern void view_transaction_success_start(void);
extern void view_transaction_success_stop(void);

#endif /* __VIEW_TRANSACTION_SUCCESS_H__ */


