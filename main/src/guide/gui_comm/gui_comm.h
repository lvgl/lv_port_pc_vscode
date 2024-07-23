/*********************
 *      INCLUDES
 *********************/
#ifndef __GUI_COMM_H__
#define __GUI_COMM_H__


#include <lvgl.h>


#define GUI_APP_NAME_LEN 16
#define GUI_PAGE_NAME_LEN 16

typedef void(*gui_app_start_callback)(void *data);
typedef void(*gui_app_stop_callback)(void);

typedef struct
{
    char app_name[GUI_APP_NAME_LEN];
    char page_name[GUI_PAGE_NAME_LEN];
    gui_app_start_callback start_cb;
    gui_app_stop_callback stop_cb;
} gui_app_desc_t;

typedef struct
{
    gui_app_desc_t desc;
    struct list list;
} gui_app_node_t;

typedef struct
{
    struct list* app_list;
} gui_app_t;
#endif /* __GUI_COMM_H__ */


