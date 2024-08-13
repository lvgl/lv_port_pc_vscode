/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_IMPORT_READY_H__
#define __STARTUP_IMPORT_READY_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_import_ready_t;


void startup_import_ready_start(void);
void startup_import_ready_stop(void);

#endif /* __STARTUP_IMPORT_READY_H__ */


