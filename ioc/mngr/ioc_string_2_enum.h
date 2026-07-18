#ifndef __IOC_STRING_2_ENUM_H__
#define __IOC_STRING_2_ENUM_H__

#include "lvgl.h"

lv_align_t ioc_string_2_align(const char *align_name);
lv_flex_flow_t ioc_string_2_flex_flow(const char *flow_name);
lv_flex_align_t ioc_string_2_flex_align(const char *align_name);
lv_text_align_t ioc_string_2_text_align(const char *align_name);

#endif
