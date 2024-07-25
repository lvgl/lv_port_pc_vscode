/*********************
 *      INCLUDES
 *********************/
#ifndef __CONNECT_RESET_H__
#define __CONNECT_RESET_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} connect_reset_t;

extern void connect_reset_start(void);
extern void connect_reset_stop(void);

#endif /* __CONNECT_RESET_H__ */


