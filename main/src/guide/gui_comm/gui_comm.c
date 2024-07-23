/*********************
 *      INCLUDES
 *********************/

#include "gui_comm.h"

bool guide_comm_init(void)
{
    guide_comm_t *guide_comm = (guide_comm_t*)rt_lv_malloc(sizeof(guide_comm_t));
    if (guide_comm == NULL)
    {
        return false;
    }
    lv_memset(guide_comm, 0, sizeof(guide_comm_t));
    return true;
}           

void guide_comm_deinit(void)
{

}



void gui_app_page_run(char *app_name, char *page_name)
{
    if (strcmp(app_name, "setting") == 0 && strcmp(page_name, "guide_comm") == 0)

}

void gui_app_page_register(char* app_name, char* page_name)
{
    gui_app_desc_t
}
