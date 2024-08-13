/*********************
 *      INCLUDES
 *********************/
#ifndef __VIEW_TRANSACTION_MAIN_H__
#define __VIEW_TRANSACTION_MAIN_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} view_transaction_main_t;

extern void view_transaction_main_start(void);
extern void view_transaction_main_stop(void);

#endif /* __VIEW_TRANSACTION_MAIN_H__ */


