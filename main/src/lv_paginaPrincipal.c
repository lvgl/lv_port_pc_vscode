#include "lv_paginaPrincipal.h"
#include "lvgl/lvgl.h"

void create_initial_page(void)
{
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_scr_load(scr);

    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "Hello, World!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}