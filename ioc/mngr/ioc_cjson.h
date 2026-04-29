#ifndef __IOC_CJSON_H__
#define __IOC_CJSON_H__

#include "cJSON.h"

/* 合并两个cJson对象, 用于进行补丁包扩展  */
cJSON * cJSON_Merge(cJSON * const dst_array, const cJSON * const src);

#endif
