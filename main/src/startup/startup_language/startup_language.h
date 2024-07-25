/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_LANGUAGE_H__
#define __STARTUP_LANGUAGE_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_language_t;


extern void startup_language_start(void);
extern void startup_language_stop(void);

#endif /* __STARTUP_LANGUAGE_H__ */


