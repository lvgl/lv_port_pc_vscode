
#include "ioc_app.h"

void ioc_obj_set_properties_position(lv_obj_t *obj, cJSON *properties_json)
{
    cJSON *x_json = cJSON_GetObjectItem(properties_json, "x");
    if (x_json != NULL)
    {
        lv_obj_set_x(obj, x_json->valueint);
        LV_LOG_INFO("%s:x:%d", ioc_obj_get_name(obj), x_json->valueint);
    }

    cJSON *y_json = cJSON_GetObjectItem(properties_json, "y");
    if (y_json != NULL)
    {
        lv_obj_set_y(obj, y_json->valueint);
        LV_LOG_INFO("%s:y:%d", ioc_obj_get_name(obj), y_json->valueint);
    }

    return;
}

void ioc_obj_set_properties_size(lv_obj_t *obj, cJSON *properties_json)
{
    cJSON *width_json = cJSON_GetObjectItem(properties_json, "width");
    if (width_json != NULL)
    {
        lv_obj_set_width(obj, width_json->valueint);
        LV_LOG_INFO("%s:w:%d", ioc_obj_get_name(obj), width_json->valueint);
    }

    cJSON *height_json = cJSON_GetObjectItem(properties_json, "height");
    if (height_json != NULL)
    {
        lv_obj_set_height(obj, height_json->valueint);
        LV_LOG_INFO("%s:h:%d", ioc_obj_get_name(obj), height_json->valueint);
    }

    return;
}

void ioc_obj_set_properties_align(lv_obj_t * obj, cJSON *properties_json)
{
    const char *align_name = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "align"));
    if (align_name != NULL)
    {
        lv_align_t align = ioc_string_2_align(align_name);

        int32_t x_ofs = 0;
        cJSON * x_ofs_json = cJSON_GetObjectItem(properties_json, "x_ofs");
        if (x_ofs_json != NULL) x_ofs = x_ofs_json->valueint;

        int32_t y_ofs = 0;
        cJSON * y_ofs_json = cJSON_GetObjectItem(properties_json, "y_ofs");
        if (y_ofs_json != NULL) y_ofs = y_ofs_json->valueint;

        cJSON * to_obj = cJSON_GetObjectItem(properties_json, "to_obj");
        if ( to_obj != NULL ){
            lv_obj_t* parent = lv_obj_get_parent(obj);
            #if (LVGL_VERSION_MAJOR == 9)
                int cnt = lv_obj_get_child_count(parent);
            #else
                int cnt = lv_obj_get_child_cnt(parent);
            #endif
            lv_obj_t* child_obj;
            ioc_obj_user_data_t * user_data;
            for ( int i = 0; i < cnt; i++ ){
                child_obj = lv_obj_get_child(parent, i);
                user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(child_obj);
                if ( strcmp(to_obj->valuestring, user_data->name) == 0 ){
                    lv_obj_align_to(obj, child_obj, align, x_ofs, y_ofs);
                    break;
                }
            }
        }
        else{
            lv_obj_align(obj, align, x_ofs, y_ofs);
        }
    }

    return;
}

void ioc_obj_set_properties_text_align(lv_obj_t * obj, cJSON *properties_json)
{
    const char *text_align = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "text_align"));
    if ( text_align != NULL )
    {
        char*str =  cJSON_Print(properties_json);
        lv_text_align_t align = ioc_string_2_text_align(text_align);
        lv_obj_set_style_text_align(obj, align, 0);
        cJSON_free(str);
    }
}

void ioc_obj_set_properties_style(lv_obj_t *obj, cJSON *properties_json)
{
    const char *style_name = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "style"));
    if (style_name != NULL)
    {
        lv_style_t * style = ioc_style_mngr_get_style(style_name);
        if (style != NULL)
        {
            lv_obj_add_style(obj, style, LV_STATE_DEFAULT);
        }
    }

    const char *style_checked_name = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "style_checked"));
    if (style_checked_name != NULL)
    {
        lv_style_t * style_checked = ioc_style_mngr_get_style(style_checked_name);
        if (style_checked != NULL)
        {
            lv_obj_add_style(obj, style_checked, LV_STATE_CHECKED);
        }
    }

    const char *style_indic_name = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "style_indic"));
    if (style_indic_name != NULL)
    {
        lv_style_t * style_indic = ioc_style_mngr_get_style(style_indic_name);
        if (style_indic != NULL)
        {
            lv_obj_add_style(obj, style_indic, LV_PART_INDICATOR);
        }
    }

    return;
}

void ioc_obj_set_properties_flex_flow(lv_obj_t *flex, cJSON *properties_json)
{
    const char *flow_name = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "flow"));
    if (flow_name != NULL) lv_obj_set_flex_flow(flex, ioc_string_2_flex_flow(flow_name));

    return;
}

void ioc_obj_set_properties_flex_align(lv_obj_t *flex, cJSON *properties_json)
{
    const char *main_place = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "main_place"));
    const char *cross_place = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "cross_place"));
    const char *track_cross_place = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "track_cross_place"));

    const char *flow_name = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "flow"));
    if (flow_name != NULL)
    {
        lv_obj_set_flex_align(flex, ioc_string_2_flex_align(main_place), ioc_string_2_flex_align(cross_place), ioc_string_2_flex_align(track_cross_place));
    }
}

void ioc_obj_set_properties_image_path(lv_obj_t *img, cJSON *properties_json)
{
    const char *src = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "src"));
    if (src != NULL)
    {
        // char *json_str = cJSON_Print(properties_json);
        // if (json_str != NULL) {
        //     LV_LOG_WARN("%s\n", json_str);
        //     free(json_str);
        // }

        // LV_LOG_WARN("=================================================");
        lv_img_set_src(img, ioc_theme_getimgpath(src));
        return;
    }

    return;
}

void ioc_obj_set_properties_gif_path(lv_obj_t *gif, cJSON *properties_json)
{
    const char *src = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "src"));
    if (src != NULL)
    {
        lv_gif_set_src(gif, ioc_theme_getimgpath(src));
        return;
    }

    return;
}

#if (LVGL_VERSION_MAJOR == 9)
void ioc_obj_set_properties_qr_code_size(lv_obj_t *qr, cJSON *properties_json)
{
    cJSON *size = cJSON_GetObjectItem(properties_json, "size");
    if (size != NULL)
    {
        lv_qrcode_set_size(qr, size->valueint);
        return;
    }

    return;
}
#endif

void ioc_obj_set_properties_qr_code_data(lv_obj_t *qr, cJSON *properties_json)
{
    const char *qr_code_data = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "qr_code_data"));
    if (qr_code_data != NULL)
    {
        lv_qrcode_update(qr, qr_code_data, strlen(qr_code_data));
        return;
    }

    return;
}

bool ioc_label_set_text(lv_obj_t *label, const char * text_id)
{
    const char * text = lv_i18n_get_text(text_id);

    // if(strcmp(text_id, "Close") == 0)
    // {
    //     LV_LOG_WARN("text_id [%s] is NULL", text_id);
    // }

    if(label == NULL)
    {
        LV_LOG_WARN("text_id [%s] is NULL", text_id);
        return false;
    }

    // 如果传入的值一样又会画一遍，浪费CPU，
    // 因为lv_label_set_text函数没有判断，所以在这里判断
    if (strcmp(text, lv_label_get_text(label)) == 0)
    {
        return false;
    }

    lv_label_set_text(label, text);
    return true;
}


void ioc_obj_set_properties_label_text(lv_obj_t *label, cJSON *properties_json)
{
    const char *text = cJSON_GetStringValue(cJSON_GetObjectItem(properties_json, "text"));
    if (text == NULL)
    {
        // 如果text为空则随便传入一个找不到的值，如果没有则返回本身
        ioc_label_set_text(label, "ioc_test");
        return;
    }

    ioc_label_set_text(label, text);
    return;
}

void ioc_obj_set_properties_hidden(lv_obj_t *obj, cJSON *properties_json)
{
    cJSON *hidden = cJSON_GetObjectItem(properties_json, "hidden");
    if (hidden != NULL)
    {
        if (cJSON_IsTrue(hidden))
        {
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        LV_LOG_INFO("%s set hiden", ioc_obj_get_name(obj));
    }

    return;
}

static void ioc_obj_make_full_name(char full_name[1024], const char * obj_name, lv_obj_t *parent)
{
    ioc_obj_user_data_t * parent_user_data = lv_obj_get_user_data(parent);

    if (parent_user_data != NULL)
    {
        const char *parent_name = parent_user_data->name;
        snprintf(full_name, 1024, "%s.%s", parent_name, obj_name);
    }
    else
    {
        strncpy(full_name, obj_name, 1024);
    }
}

bool ioc_obj_create_user_data(lv_obj_t * obj, const char * obj_name, ioc_obj_type_t type, cJSON *obj_json, lv_obj_t *parent)
{
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)calloc(1, sizeof(ioc_obj_user_data_t));
    if (user_data == NULL)
    {
        LV_LOG_ERROR("can't malloc user_data");
        return false;
    }

    char full_name[1024];
    ioc_obj_make_full_name(full_name, (obj_name != NULL) ? obj_name : cJSON_GetStringValue(cJSON_GetObjectItem(obj_json, "name")), parent);
    user_data->name = strdup(full_name);
    if (user_data->name == NULL)
    {
        LV_LOG_ERROR("can't malloc name");
        free(user_data);
        return false;
    }

    user_data->type = type;
    user_data->ref_json = obj_json;
    user_data->obj = obj;
    user_data->parent_user_data = lv_obj_get_user_data(parent);

    lv_obj_set_user_data(obj, user_data);
    // LV_LOG_WARN("obj: %s", user_data->name);

    lv_obj_add_event_cb(obj, ioc_obj_deleted_event_cb, LV_EVENT_DELETE, NULL);

    return true;
}

lv_obj_t *ioc_obj_create_basic_obj(const char * name, ioc_obj_type_t type, cJSON *obj_json, lv_obj_t *parent)
{
    lv_obj_t *obj = NULL;
    if (type == IOC_OBJ_TYPE_LABEL)
    {
        obj = lv_label_create(parent);
    }
    else if (type == IOC_OBJ_TYPE_IMAGE)
    {
        obj = lv_img_create(parent);
    }
    else if (type == IOC_OBJ_TYPE_GIF)
    {
        obj = lv_gif_create(parent);
    }
    else if (type == IOC_OBJ_TYPE_BUTTON)
    {
        obj = lv_btn_create(parent);
        lv_obj_set_style_shadow_width(obj, 0, 0);
    }
    else if (type == IOC_OBJ_TYPE_QR)
    {
    #if (LVGL_VERSION_MAJOR == 9)
        obj = lv_qrcode_create(parent);
    #else
        cJSON *properties_json = cJSON_GetObjectItem(obj_json, "properties");
        if (properties_json != NULL)
        {
            cJSON *size_json = cJSON_GetObjectItem(properties_json, "size");
            if (size_json != NULL)
            {
                obj = lv_qrcode_create(parent, size_json->valueint, lv_color_make(0,0,0), lv_color_make(255,255,255));
            }
        }
    #endif
    }
    else if (type == IOC_OBJ_TYPE_BAR)
    {
        obj = lv_bar_create(parent);
    }
    else if (type == IOC_OBJ_TYPE_LIST)
    {
        obj = lv_list_create(parent);
    }
    else
    {
        obj = lv_obj_create(parent);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    }
    if (obj == NULL)
    {
        return NULL;
    }
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);     // 全局性的取消所有控件的Scrollable属性, 我们的布局用不到, 如后续有需求再调整代码

    if (!ioc_obj_create_user_data(obj, (name != NULL) ? name : cJSON_GetStringValue(cJSON_GetObjectItem(obj_json, "name")), type, obj_json, parent))
    {
        lv_obj_del(obj);
        return NULL;
    }

    cJSON *properties_json = cJSON_GetObjectItem(obj_json, "properties");
    if (properties_json != NULL)
    {
        ioc_obj_set_properties_position(obj, properties_json);
        ioc_obj_set_properties_size(obj, properties_json);
        ioc_obj_set_properties_align(obj, properties_json);
        ioc_obj_set_properties_text_align(obj, properties_json);
        ioc_obj_set_properties_hidden(obj, properties_json);
        if (type == IOC_OBJ_TYPE_FLEX)
        {
            ioc_obj_set_properties_flex_flow(obj, properties_json);
            ioc_obj_set_properties_flex_align(obj, properties_json);
        }
        else if (type == IOC_OBJ_TYPE_LABEL)
        {
            ioc_obj_set_properties_label_text(obj, properties_json);
        }
        else if (type == IOC_OBJ_TYPE_IMAGE)
        {
            ioc_obj_set_properties_image_path(obj, properties_json);
        }
        else if (type == IOC_OBJ_TYPE_GIF)
        {
            ioc_obj_set_properties_gif_path(obj, properties_json);
        }
        else if (type == IOC_OBJ_TYPE_QR)
        {
        #if (LVGL_VERSION_MAJOR == 9)
            ioc_obj_set_properties_qr_code_size(obj, properties_json);
        #endif
            ioc_obj_set_properties_qr_code_data(obj, properties_json);
        }
        else if (type == IOC_OBJ_TYPE_BAR)
        {

        }
        else if (type == IOC_OBJ_TYPE_LIST)
        {

        }
        ioc_obj_set_properties_style(obj, properties_json);
    }

    cJSON * children_json = cJSON_GetObjectItem(obj_json, "children");

    // char *json_str = cJSON_Print(children_json);
    // if (json_str != NULL) {
    //     LV_LOG_WARN("%s\n", json_str);
    //     free(json_str);
    // }

    if (children_json != NULL)
    {
        cJSON * child_json;
        cJSON_ArrayForEach(child_json, children_json)
        {
            lv_obj_t *child = ioc_obj_create_obj(cJSON_GetStringValue(cJSON_GetObjectItem(child_json, "name")), child_json, obj);
            if (child != NULL)
            {
                ioc_obj_set_properties_position(child, child_json);
                ioc_obj_set_properties_size(child, child_json);
                ioc_obj_set_properties_hidden(child, child_json);
                ioc_obj_set_properties_align(child, properties_json);
                ioc_obj_set_properties_text_align(child, properties_json);
                ioc_obj_set_properties_style(child, child_json);
            }
        }
    }

    return obj;
}

lv_obj_t * ioc_obj_create_obj(const char * name, cJSON * obj_json, lv_obj_t * parent)
{
    const char * obj_name = (name != NULL) ? name : cJSON_GetStringValue(cJSON_GetObjectItem(obj_json, "name"));

    char full_name[1024];
    ioc_obj_make_full_name(full_name, obj_name, parent);

    if (!ioc_obj_handler_before_create(full_name))
    {
        LV_LOG_ERROR("Not permit to create obj %s", full_name);
        return NULL;
    }

    const char * type_name = cJSON_GetStringValue(cJSON_GetObjectItem(obj_json, "type"));
    if (type_name == NULL)
    {
        // char *json_str = cJSON_Print(obj_json);
        // if (json_str != NULL) {
        //     LV_LOG_WARN("%s\n", json_str);
        //     free(json_str);
        // }

        LV_LOG_ERROR("crteat[%s]  no type",   full_name);
        return NULL;
    }

    lv_obj_t * obj = NULL;
    bool is_fragment = false;
    if ((strcmp(type_name, "fragment") == 0)
    || (strcmp(type_name, "fragment_checkbox") == 0))
    {
        is_fragment = true;
        obj = ioc_obj_create_fragment(obj_name, obj_json, parent);
    }
    else if (strcmp(type_name, "flex") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_FLEX, obj_json, parent);
    }
    else if (strcmp(type_name, "image") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_IMAGE, obj_json, parent);
    }
    else if (strcmp(type_name, "gif") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_GIF, obj_json, parent);
    }
    else if (strcmp(type_name, "label") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_LABEL, obj_json, parent);
    }
    else if (strcmp(type_name, "button") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_BUTTON, obj_json, parent);
    }
    else if (strcmp(type_name, "qr") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_QR, obj_json, parent);
    }
    else if (strcmp(type_name, "bar") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_BAR, obj_json, parent);
    }
    else if (strcmp(type_name, "list") == 0)
    {
        obj = ioc_obj_create_basic_obj(obj_name, IOC_OBJ_TYPE_LIST, obj_json, parent);
    }
    else
    {
        LV_LOG_ERROR("unknow type %s", type_name);
        return false;
    }
    if (obj == NULL)
    {
        LV_LOG_ERROR("can't create obj %s", full_name);
        return false;
    }
    if (!is_fragment)
    {
        // LV_LOG_WARN("==============>[%s] %s", type_name, full_name);
        ioc_obj_handler_after_create(full_name, obj);
    }

    return obj;
}

void ioc_obj_deleted_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    ioc_obj_user_data_t * user_data = lv_obj_get_user_data(obj);
    if (user_data != NULL)
    {
        free(user_data->name);
        free(user_data);
    }
}

const char * ioc_obj_get_name(lv_obj_t * obj)
{
    ioc_obj_user_data_t * user_data = lv_obj_get_user_data(obj);
    if (user_data != NULL)
    {
        return user_data->name;
    }
    return "UnknownName";
}

bool ioc_obj_name_is_prefix(const char *name, int *prefix_len)
{
    int len = strlen(name);
    if (len < 2)
    {
        return false;
    }

    if (name[len - 1] != '*')
    {
        return false;
    }

    *prefix_len = len - 1;
    return true;
}

bool ioc_obj_name_match_prefix(const char * name        // 待匹配的name
    , const char * prefix , int prefix_len              // 预设前缀
    , int * idx                                         // 匹配到的idx
    , const char ** name_after_prefix, int * name_after_prefix_len // name去掉prefix和idx后的结果
)
{
    // LV_LOG_WARN("==name[%s] prerix[%s] prefix_len[%d]", name, prefix, prefix_len);
    // 检查输入字符串是否以输入前缀开始
    if (strncmp(name, prefix, prefix_len) != 0)
    {
        return false; // 不符合前缀
    }

    // 跳过前缀部分，定位到数字的起始位置
    const char *num_start = name + prefix_len;

    // 找到数字结束的位置（即点号的位置）
    const char *dot_pos = strchr(num_start, '.');
    if (dot_pos == NULL)
    {
        // 如果没有点号，说明后面没有额外的名称
        *name_after_prefix = NULL; // 后续名称为空
        *name_after_prefix_len = 0;
    }
    else
    {
        // 如果有点号，后续名称从点号后开始
        *name_after_prefix = dot_pos + 1; // 跳过点号
        *name_after_prefix_len = strlen(*name_after_prefix); // 计算后续名称的长度
    }

    // 提取数字部分
    char num_str[32] = {0}; // 假设数字不会超过31位
    int num_len = (dot_pos == NULL) ? strlen(num_start) : (dot_pos - num_start);
    if (num_len >= sizeof(num_str))
    {
        return false; // 数字过长
    }
    strncpy(num_str, num_start, num_len);

    // 将数字字符串转换为整数
    char *endptr;
    *idx = strtol(num_str, &endptr, 10);
    if (*endptr != '\0')
    {
        return false; // 数字格式不正确
    }

    return true;
}
