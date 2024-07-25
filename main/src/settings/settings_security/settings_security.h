/*********************
 *      INCLUDES
 *********************/
#ifndef __SETTINGS_SECURITY_H__
#define __SETTINGS_SECURITY_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} settings_security_t;


extern void settings_security_start(void);
extern void settings_security_stop(void);

#endif /* __SETTINGS_SECURITY_H__ */


