#ifndef __IOC_OBJ_H__
#define __IOC_OBJ_H__

#include "lvgl.h"
#include "cJSON.h"

typedef enum {
    IOC_OBJ_TYPE_FRAGMENT = 0,

    IOC_OBJ_TYPE_FLEX,

    IOC_OBJ_TYPE_LABEL,
    IOC_OBJ_TYPE_IMAGE,
    IOC_OBJ_TYPE_GIF,
    IOC_OBJ_TYPE_BUTTON,
    IOC_OBJ_TYPE_QR,
    IOC_OBJ_TYPE_BAR,
    IOC_OBJ_TYPE_LIST,

    IOC_OBJ_TYPE_NUM,
} ioc_obj_type_t;

typedef struct ioc_obj_user_data_s
{
    char * name;
    ioc_obj_type_t type;

    cJSON * ref_json;
    lv_obj_t *obj;
    struct ioc_obj_user_data_s * parent_user_data;

    union
    {
        void * data_ptr;
        uint32_t data_uint32;
        uint16_t data_uint16;
        uint8_t data_uint8;
        int32_t data_int32;
        int16_t data_int16;
        int8_t data_int8;
        int data_int;
    } extra_data;           // 额外数据, 默认创建的控件为NULL, 用户可以在after_create中设置值
                            // 如果是动态申请出来的, 请注意用户释放. 建议尽量不要动态申请, 以免增加复杂度

    void (*ui_json_update_cb)(struct ioc_obj_user_data_s * user_data);    // ui_json更新回调, 在ui_json更新的时候调用

} ioc_obj_user_data_t;


typedef struct {

    const char * name;

    bool (*before_create)(const char *name);

    void (*after_create)(const char * name, lv_obj_t * obj);
} ioc_obj_handler_t;


/**
 * @note 递归创建对象
 * @param name 对象名称, 如果为null, 则使用json中的name字段
 */
lv_obj_t * ioc_obj_create_obj(const char * name, cJSON * obj_json, lv_obj_t * parent);

/**
 * @note 对象删除的时候默认调用的回调, 需要删除自动创建的user_data
 *       如果用户自己注册了删除回调, 最后需要调用这个回调, 防止内存泄漏
 */
void ioc_obj_deleted_event_cb(lv_event_t * e);

/**
 * @note 获取对象名称, 对象名称为a.b.c格式
 */
const char * ioc_obj_get_name(lv_obj_t * obj);

/**
 * @note 判断name是否是prefix的前缀
 * 前缀一定是"main_quad.connector_quad_group.connector*"格式, 即 prefix+星号结束
 *
 */
bool ioc_obj_name_is_prefix(const char * name, int * prefix_len);

/**
 * @note 判断name是否符合prefix格式
 * 前缀一定是"main_quad.connector_quad_group.connector*"格式, 即 prefix+星号结束
 * 我们系统的复杂度目前只考虑一级前缀, 不考虑多级
 * name可能有两个格式
 * 1. "main_quad.connector_quad_group.connector1"
 * 2. "main_quad.connector_quad_group.connector1.label"
 * 分别对应解析结果为
 * 1. prefix = "main_quad.connector_quad_group.connector", idx = 1, name = NULL
 * 2. prefix = "main_quad.connector_quad_group.connector", idx = 1, name = "label"
 * 注意, name_after_prefix是name去掉prefix后的结果, 直接指向name的内存, 不需要用户释放
 */
bool ioc_obj_name_match_prefix(const char * name        // 待匹配的name
    , const char * prefix , int prefix_len              // 预设前缀
    , int * idx                                         // 匹配到的idx
    , const char ** name_after_prefix, int * name_after_prefix_len // name去掉prefix和idx后的结果
);
#define IOC_OBJ_NAME_MATCH_AFTER_PREFIX(preset_const, name_after_prefix, name_after_prefix_len) \
    ((name_after_prefix_len == strlen(preset_const)) && strncmp(name_after_prefix, preset_const, name_after_prefix_len) == 0)

/**
 * @note 在创建对象之前调用, 可以根据name决定是否创建对象
 */
bool ioc_obj_handler_before_create(const char * name);
/**
 * @note 在创建对象之后调用, 可以根据name进行后续的处理
 */
void ioc_obj_handler_after_create(const char * name, lv_obj_t * obj);

void ioc_obj_set_properties_position(lv_obj_t *obj, cJSON *properties_json);
void ioc_obj_set_properties_size(lv_obj_t *obj, cJSON *properties_json);
void ioc_obj_set_properties_align(lv_obj_t * obj, cJSON *properties_json);
void ioc_obj_set_properties_flex_flow(lv_obj_t *flex, cJSON *properties_json);
void ioc_obj_set_properties_flex_align(lv_obj_t *flex, cJSON *properties_json);
void ioc_obj_set_properties_style(lv_obj_t *obj, cJSON *properties_json);
void ioc_obj_set_properties_image_path(lv_obj_t *img, cJSON *properties_json);
void ioc_obj_set_properties_label_text(lv_obj_t *label, cJSON *properties_json);
bool ioc_label_set_text(lv_obj_t *label, const char * text_id);



#endif
