#include "../pages_mngr.h"


typedef struct {

    lv_obj_t * page;

} ui_page_07_reserved_t;

static ui_page_07_reserved_t ui_page_07_reserved = {NULL};


static void ui_page_07_reserved_updatejson(struct ioc_obj_user_data_s * user_data)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t * ui_page_create_07_reserved(lv_obj_t *parent)
{
    memset(&ui_page_07_reserved, 0, sizeof(ui_page_07_reserved));

    ui_page_07_reserved.page = ioc_layout_mngr_load_layout("reserved", parent);
    if (ui_page_07_reserved.page != NULL)
    {
        ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(ui_page_07_reserved.page);
        user_data->ui_json_update_cb = ui_page_07_reserved_updatejson;
    }

    return ui_page_07_reserved.page;
}

void ui_page_show_07_reserved(void)
{

}
void ui_page_hide_07_reserved(void)
{

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ui_after_create_07_reserved(const char * name, lv_obj_t * obj)
{

}
