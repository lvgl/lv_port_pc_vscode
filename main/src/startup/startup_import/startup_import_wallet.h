/*********************
 *      INCLUDES
 *********************/
#ifndef __STARTUP_IMPORT_WALLET_H__
#define __STARTUP_IMPORT_WALLET_H__


#include <lvgl.h>

typedef struct
{
	lv_obj_t* bg_cont;
} startup_import_wallet_t;


void startup_import_wallet_start(void);
void startup_import_wallet_stop(void);

#endif /* __STARTUP_IMPORT_WALLET_H__ */


