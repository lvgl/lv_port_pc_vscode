/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_IMPORT_ABORT_H__
#define __STARTUP_IMPORT_ABORT_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_import_abort_t;


void startup_import_abort_start(void);
void startup_import_abort_stop(void);

#endif /* __STARTUP_IMPORT_ABORT_H__ */


