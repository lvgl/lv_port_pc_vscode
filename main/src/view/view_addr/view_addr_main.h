/*********************
 *      INCLUDES
 *********************/
#ifndef __VIEW_ADDR_MAIN_H__
#define __VIEW_ADDR_MAIN_H__


#include <lvgl.h>

enum
{
	VIEW_ADDR_MODE_TEXT,
	VIEW_ADDR_MODE_QRCODE,
};
typedef uint8_t view_addr_page_mode_t;

typedef struct
{
	view_addr_page_mode_t page_mode;
	lv_obj_t* bg_cont;
	lv_obj_t* bg_cont_text;
	lv_obj_t* bg_cont_qrcode;

	lv_obj_t* label_qrcode;
} view_addr_main_t;

extern void view_addr_main_start(void);
extern void view_addr_main_stop(void);

#endif /* __VIEW_ADDR_MAIN_H__ */


