#ifndef __UI_POPUP_COMMON_H__
#define __UI_POPUP_COMMON_H__

#include "lvgl.h"

#define UI_NAME_POPUP_COMMON         "popup_common"

typedef enum
{
    POP_PROMPTS,
    POP_WARNRING,
    POP_ERROR,

}circle_e;

void ui_popup_show_mask(bool show);

// 显示ui_json传递过来的homeOcpp发起的弹出信息, 例如授权错误等
void ui_popup_show_common(char *str, bool needi18n, circle_e circle_img);

void ui_after_create_popup_common(const char * name, lv_obj_t * obj);

void popup_set_circle_img(int idx);

void ui_popup_common_handle(const char *str);
#endif
