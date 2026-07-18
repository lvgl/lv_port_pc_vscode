#ifndef __CONST_NAME_H__
#define __CONST_NAME_H__


typedef enum Charger_CONNType_e
{
	HWCFG_CONN_TYPE_CHADEMO_DC	= 0,
	HWCFG_CONN_TYPE_GBT_DC     	= 1,
	HWCFG_CONN_TYPE_SAE_DC		= 2,
	HWCFG_CONN_TYPE_GBT_AC		= 3,
	HWCFG_CONN_TYPE_IEC_DC		= 4,
	HWCFG_CONN_TYPE_SAE_AC		= 5,
	HWCFG_CONN_TYPE_IEC_AC		= 6,
	HWCFG_CONN_TYPE_GBT_AC_SOCK		= 7,
	HWCFG_CONN_TYPE_SAE_AC_SOCK		= 8,
	HWCFG_CONN_TYPE_IEC_AC_SOCK		= 9,
    HWCFG_CONN_TYPE_SCHUKO          = 10,   // 欧标单相插座, 由于控制逻辑完全相同, 如果使用英式或美式不再另行使用枚举, 借用本枚举
	HWCFG_CONN_TYPE_NACS_DC         = 11,   // 特斯拉直流充电接口，控制逻辑与CCS2相同
    HWCFG_CONN_TYPE_INVALID  	= 0xF
} Charger_CONNType_t;

#define CONN_TYPE_VALID (HWCFG_CONN_TYPE_NACS_DC)

typedef enum
{
    LANG_EN = 0,            // 英语
    LANG_FR,                // 法语
    LANG_DE,                // 德语
    LANG_ES,                // 西班牙语
    LANG_IT,                // 意大利语
    LANG_PT,                // 葡萄牙语
    LANG_RU,                // 俄语
    LANG_CZ,                // 捷克语
    LANG_FI,                // 芬兰语
    LANG_ZH_CN,             // 简体中文
    LANG_ZH_TW,             // 繁体中文(台湾)
    LANG_RO,                // 罗马尼亚语
    LANG_TH,                // 泰国语
    LANG_AR,                // 阿拉伯语
    LANG_TU,                // 土耳其语
    LANG_PL,                // 波兰语
    LANG_NL,                // 荷兰语
    LANG_SQ,                // 阿尔巴尼亚语
    LANG_HE,                // 希伯来语
    LANG_GR,                // 希腊语 (DC暂时未使用)
    LANG_LT,                // 立陶宛语
    LANG_HBC,               // HBC客户定制葡萄牙语
    LANG_BG,                // 保加利亚语

    LANG_NUM,
} Language_t;

/**
 * 将字符串转换为枚举值
 * @param value 要转换的字符串
 * @param enum_values 枚举字符串数组
 * @param enum_num 枚举值的数量
 * @return 如果找到匹配的字符串，返回对应的枚举值；否则返回 -1
 */
int ioc_ui_json_string2enum(const char* value, const char** enum_values, int enum_num);

/**
 * 将枚举值转换为字符串
 * @param enum_value 要转换的枚举值
 * @param enum_values 枚举字符串数组
 * @param enum_num 枚举值的数量
 * @return 如果枚举值有效，返回对应的字符串；否则返回 NULL
 */
const char* ioc_ui_json_enum2string(int enum_value, const char** enum_values, int enum_num);

const char * ioc_ui_json_get_conn_type_name(int type);
const char * ioc_ui_json_get_state_name(int state);

#if (COMPILER_FOR == COMPILE_FOR_DC_AM3354)
const char * ui_conn_type_img(Charger_CONNType_t connType);
#else
const char * ui_conn_type_img(Charger_CONNType_t connType, bool is_gun);
#endif

extern const char *c_LanguageName[];
extern const char * const ioc_ui_connector_state_str[];

#endif
