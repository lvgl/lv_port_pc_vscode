#include "ioc_app.h"

#ifdef _WIN32
#include <windows.h>
#endif

static lv_obj_t * s_ui_welcome = NULL;
static const char * s_ui_visible_tree_json_name = "ui_visible_tree.json";
static const char * s_ui_visible_obj_info_json_name = "ui_visible_obj_info.json";

static bool ioc_app_get_executable_dir(char *dir_path, size_t dir_path_size)
{
    if ((dir_path == NULL) || (dir_path_size == 0))
    {
        return false;
    }

#ifdef _WIN32
    DWORD path_len = GetModuleFileNameA(NULL, dir_path, (DWORD)dir_path_size);
    if ((path_len == 0) || (path_len >= dir_path_size))
    {
        LV_LOG_ERROR("Failed to get executable path");
        return false;
    }

    for (int i = (int)path_len - 1; i >= 0; i--)
    {
        if ((dir_path[i] == '\\') || (dir_path[i] == '/'))
        {
            dir_path[i] = '\0';
            return true;
        }
    }
#else
    ssize_t path_len = readlink("/proc/self/exe", dir_path, dir_path_size - 1);
    if ((path_len <= 0) || (path_len >= (ssize_t)dir_path_size))
    {
        LV_LOG_ERROR("Failed to get executable path: %s", strerror(errno));
        return false;
    }

    dir_path[path_len] = '\0';
    for (ssize_t i = path_len - 1; i >= 0; i--)
    {
        if (dir_path[i] == '/')
        {
            dir_path[i] = '\0';
            return true;
        }
    }
#endif

    LV_LOG_ERROR("Failed to parse executable directory");
    return false;
}

static const char *ioc_app_align_to_string(lv_align_t align)
{
    switch (align)
    {
        case LV_ALIGN_DEFAULT: return "LV_ALIGN_DEFAULT";
        case LV_ALIGN_TOP_LEFT: return "LV_ALIGN_TOP_LEFT";
        case LV_ALIGN_TOP_MID: return "LV_ALIGN_TOP_MID";
        case LV_ALIGN_TOP_RIGHT: return "LV_ALIGN_TOP_RIGHT";
        case LV_ALIGN_LEFT_MID: return "LV_ALIGN_LEFT_MID";
        case LV_ALIGN_CENTER: return "LV_ALIGN_CENTER";
        case LV_ALIGN_RIGHT_MID: return "LV_ALIGN_RIGHT_MID";
        case LV_ALIGN_BOTTOM_LEFT: return "LV_ALIGN_BOTTOM_LEFT";
        case LV_ALIGN_BOTTOM_MID: return "LV_ALIGN_BOTTOM_MID";
        case LV_ALIGN_BOTTOM_RIGHT: return "LV_ALIGN_BOTTOM_RIGHT";
        case LV_ALIGN_OUT_TOP_LEFT: return "LV_ALIGN_OUT_TOP_LEFT";
        case LV_ALIGN_OUT_TOP_MID: return "LV_ALIGN_OUT_TOP_MID";
        case LV_ALIGN_OUT_TOP_RIGHT: return "LV_ALIGN_OUT_TOP_RIGHT";
        case LV_ALIGN_OUT_BOTTOM_LEFT: return "LV_ALIGN_OUT_BOTTOM_LEFT";
        case LV_ALIGN_OUT_BOTTOM_MID: return "LV_ALIGN_OUT_BOTTOM_MID";
        case LV_ALIGN_OUT_BOTTOM_RIGHT: return "LV_ALIGN_OUT_BOTTOM_RIGHT";
        case LV_ALIGN_OUT_LEFT_TOP: return "LV_ALIGN_OUT_LEFT_TOP";
        case LV_ALIGN_OUT_LEFT_MID: return "LV_ALIGN_OUT_LEFT_MID";
        case LV_ALIGN_OUT_LEFT_BOTTOM: return "LV_ALIGN_OUT_LEFT_BOTTOM";
        case LV_ALIGN_OUT_RIGHT_TOP: return "LV_ALIGN_OUT_RIGHT_TOP";
        case LV_ALIGN_OUT_RIGHT_MID: return "LV_ALIGN_OUT_RIGHT_MID";
        case LV_ALIGN_OUT_RIGHT_BOTTOM: return "LV_ALIGN_OUT_RIGHT_BOTTOM";
        default: return "LV_ALIGN_UNKNOWN";
    }
}

static const char *ioc_app_text_align_to_string(lv_text_align_t align)
{
    switch (align)
    {
        case LV_TEXT_ALIGN_AUTO: return "LV_TEXT_ALIGN_AUTO";
        case LV_TEXT_ALIGN_LEFT: return "LV_TEXT_ALIGN_LEFT";
        case LV_TEXT_ALIGN_CENTER: return "LV_TEXT_ALIGN_CENTER";
        case LV_TEXT_ALIGN_RIGHT: return "LV_TEXT_ALIGN_RIGHT";
        default: return "LV_TEXT_ALIGN_UNKNOWN";
    }
}

static cJSON *ioc_app_build_visible_obj_tree(lv_obj_t *obj)
{
    if ((obj == NULL) || lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN))
    {
        return NULL;
    }

    cJSON *node = cJSON_CreateObject();
    if (node == NULL)
    {
        return NULL;
    }

    const char *name = "lv_scr_act";
    ioc_obj_user_data_t *user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    if ((user_data != NULL) && (user_data->name != NULL))
    {
        name = user_data->name;
    }

    cJSON_AddStringToObject(node, "name", name);

    cJSON *children = cJSON_CreateArray();
    if (children == NULL)
    {
        cJSON_Delete(node);
        return NULL;
    }
    cJSON_AddItemToObject(node, "children", children);

    int cnt = lv_obj_get_child_cnt(obj);
    for (int i = 0; i < cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(obj, i);
        cJSON *child_node = ioc_app_build_visible_obj_tree(child);
        if (child_node != NULL)
        {
            cJSON_AddItemToArray(children, child_node);
        }
    }

    return node;
}

static void ioc_app_dump_visible_obj_tree_to_json_file(lv_obj_t *root, const char *file_name)
{
    cJSON *tree = ioc_app_build_visible_obj_tree(root);
    if (tree == NULL)
    {
        LV_LOG_ERROR("Failed to build visible object tree");
        return;
    }

    char *json_str = cJSON_Print(tree);
    if (json_str == NULL)
    {
        LV_LOG_ERROR("Failed to serialize visible object tree");
        cJSON_Delete(tree);
        return;
    }

    char file_path[1024] = {0};
    if (!ioc_app_get_executable_dir(file_path, sizeof(file_path)))
    {
        cJSON_free(json_str);
        cJSON_Delete(tree);
        return;
    }

    size_t path_len = strlen(file_path);
    if ((path_len + 1 + strlen(file_name) + 1) >= sizeof(file_path))
    {
        LV_LOG_ERROR("Visible object tree path is too long");
        cJSON_free(json_str);
        cJSON_Delete(tree);
        return;
    }

#ifdef _WIN32
    snprintf(file_path + path_len, sizeof(file_path) - path_len, "\\%s", file_name);
#else
    snprintf(file_path + path_len, sizeof(file_path) - path_len, "/%s", file_name);
#endif

    FILE *fp = fopen(file_path, "w");
    if (fp == NULL)
    {
        LV_LOG_ERROR("Failed to open %s: %s", file_path, strerror(errno));
        cJSON_free(json_str);
        cJSON_Delete(tree);
        return;
    }

    fputs(json_str, fp);
    fclose(fp);

    LV_LOG_WARN("Visible object tree exported to %s", file_path);

    cJSON_free(json_str);
    cJSON_Delete(tree);
}

static cJSON *ioc_app_create_visible_obj_info_item(lv_obj_t *obj)
{
    if ((obj == NULL) || lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN))
    {
        return NULL;
    }

    cJSON *item = cJSON_CreateObject();
    if (item == NULL)
    {
        return NULL;
    }

    const char *name = "lv_scr_act";
    ioc_obj_user_data_t *user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    if (user_data != NULL)
    {
        if (user_data->name != NULL)
        {
            name = user_data->name;
        }
    }

    cJSON_AddStringToObject(item, "name", name);
    cJSON_AddNumberToObject(item, "x", lv_obj_get_x(obj));
    cJSON_AddNumberToObject(item, "y", lv_obj_get_y(obj));
    cJSON_AddNumberToObject(item, "width", lv_obj_get_width(obj));
    cJSON_AddNumberToObject(item, "height", lv_obj_get_height(obj));
    cJSON_AddStringToObject(item, "align", ioc_app_align_to_string(lv_obj_get_style_align(obj, LV_PART_MAIN)));
    cJSON_AddNumberToObject(item, "x_ofs", lv_obj_get_style_x(obj, LV_PART_MAIN));
    cJSON_AddNumberToObject(item, "y_ofs", lv_obj_get_style_y(obj, LV_PART_MAIN));
    cJSON_AddStringToObject(item, "text_align", ioc_app_text_align_to_string(lv_obj_get_style_text_align(obj, LV_PART_MAIN)));

    return item;
}

static void ioc_app_collect_visible_obj_info(lv_obj_t *obj, cJSON *array)
{
    if ((obj == NULL) || (array == NULL) || lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN))
    {
        return;
    }

    cJSON *item = ioc_app_create_visible_obj_info_item(obj);
    if (item != NULL)
    {
        cJSON_AddItemToArray(array, item);
    }

    int cnt = lv_obj_get_child_cnt(obj);
    for (int i = 0; i < cnt; i++)
    {
        ioc_app_collect_visible_obj_info(lv_obj_get_child(obj, i), array);
    }
}

static void ioc_app_dump_visible_obj_info_to_json_file(lv_obj_t *root, const char *file_name)
{
    cJSON *array = cJSON_CreateArray();
    if (array == NULL)
    {
        LV_LOG_ERROR("Failed to create visible object info array");
        return;
    }

    ioc_app_collect_visible_obj_info(root, array);

    char *json_str = cJSON_Print(array);
    if (json_str == NULL)
    {
        LV_LOG_ERROR("Failed to serialize visible object info");
        cJSON_Delete(array);
        return;
    }

    char file_path[1024] = {0};
    if (!ioc_app_get_executable_dir(file_path, sizeof(file_path)))
    {
        cJSON_free(json_str);
        cJSON_Delete(array);
        return;
    }

    size_t path_len = strlen(file_path);
    if ((path_len + 1 + strlen(file_name) + 1) >= sizeof(file_path))
    {
        LV_LOG_ERROR("Visible object info path is too long");
        cJSON_free(json_str);
        cJSON_Delete(array);
        return;
    }

#ifdef _WIN32
    snprintf(file_path + path_len, sizeof(file_path) - path_len, "\\%s", file_name);
#else
    snprintf(file_path + path_len, sizeof(file_path) - path_len, "/%s", file_name);
#endif

    FILE *fp = fopen(file_path, "w");
    if (fp == NULL)
    {
        LV_LOG_ERROR("Failed to open %s: %s", file_path, strerror(errno));
        cJSON_free(json_str);
        cJSON_Delete(array);
        return;
    }

    fputs(json_str, fp);
    fclose(fp);

    LV_LOG_WARN("Visible object info exported to %s", file_path);

    cJSON_free(json_str);
    cJSON_Delete(array);
}

void ioc_app_ui_json_updated_widgets(lv_obj_t* parent)
{
    if ((parent == NULL) || (lv_obj_has_flag(parent, LV_OBJ_FLAG_HIDDEN)))
    {
        return;
    }

    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t*)lv_obj_get_user_data(parent);
    if (user_data != NULL)
    {
        // LV_LOG_WARN("in user_data_name: %s", user_data->name);

        if (user_data->ui_json_update_cb != NULL)
        {
            user_data->ui_json_update_cb(user_data);
        }
        // LV_LOG_WARN("out user_data_name: %s", user_data->name);
    }

    int cnt = lv_obj_get_child_cnt(parent);
    if (cnt > 0)
    {
        // if (cnt > 20) DEBUG_WARN("CNT %d >20", cnt);
        lv_obj_t** childs = (lv_obj_t**)malloc(sizeof(lv_obj_t*)*cnt);
        int i;
        for (i = 0; i < cnt; i++)
        {
            childs[i] = lv_obj_get_child(parent, i);
        }

        for (i = 0; i < cnt; i++)
        {
            // 递归遍历子控件
            ioc_app_ui_json_updated_widgets(childs[i]);
        }
        free(childs);
    }

    return;
}

// welcome 界面
/* ===== 动画配置 ===== */
#define DOT_SIZE        30
#define DOT_GAP         20
#define DOT_RADIUS      8

#define ANIM_TIME       600   // 动画速度放慢（原来 350）
#define ANIM_DELAY_STEP 200   // 依次亮起的间隔也放慢

/* 亮色 & 暗色 */
static lv_color_t color_on  = LV_COLOR_MAKE(0x1A, 0xE9, 0xD9);  // 高亮色
static lv_color_t color_off = LV_COLOR_MAKE(0xD6, 0xF8, 0xF8);  // 浅色

/* 动画回调：改变背景色 */
static void anim_set_color(void * obj, int32_t v)
{
    lv_obj_set_style_bg_color(obj, lv_color_mix(color_on, color_off, v), 0);
}

/* 单个点动画 */
static void create_dot_anim(lv_obj_t * dot, uint32_t delay)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, dot);
    lv_anim_set_exec_cb(&a, anim_set_color);

    lv_anim_set_values(&a, 255, 0);   // 从浅 → 深（亮起）
    lv_anim_set_time(&a, ANIM_TIME);
    lv_anim_set_delay(&a, delay);

    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 200);
    lv_anim_start(&a);
}

/* ===== 创建加载动画组件 ===== */
void ui_create_loading_anim(lv_obj_t * parent)
{
    /* --- 确保父对象也不是可滚动的（非常关键）--- */
    if(parent) {
        lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(parent, LV_DIR_NONE);
    }

    /* 创建透明容器（无外框、无背景） */
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_gap(cont, DOT_GAP, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);

    /* 彻底关闭容器的滚动相关设置 */
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(cont, LV_DIR_NONE);

    /* 居中 */
    lv_obj_center(cont);

    for(int i = 0; i < 4; i++)
    {
        lv_obj_t * dot = lv_obj_create(cont);
        lv_obj_set_size(dot, DOT_SIZE, DOT_SIZE);
        lv_obj_set_style_radius(dot, DOT_RADIUS, 0);
        lv_obj_set_style_bg_color(dot, color_off, 0);
        lv_obj_set_style_border_width(dot, 0, 0);

        /* 也把点的滚动属性都清掉（保险） */
        lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_scrollbar_mode(dot, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(dot, LV_DIR_NONE);

        create_dot_anim(dot, i * ANIM_DELAY_STEP);
    }
}


void ioc_app_main(void)
{
    ioc_ui_json_init();
    lv_i18n_init(lv_i18n_language_pack);
    // 初始化加载语言列表
    // 初始化加载翻译列表
    ioc_theme_mngr_load("main-light");
    ioc_style_mngr_load(NULL);
    ioc_fragment_mngr_load();
    ioc_layout_mngr_load();
    s_preset_handler_init();
    s_ui_welcome = ioc_layout_mngr_load_layout("welcome", lv_scr_act());

    ui_create_loading_anim(s_ui_welcome);

    // // 自组网
    // autoGroup_init();

    // // 测试用
    // ioc_ui_json_set_con_num(1);
    // g_ui_json.dispenserConfig.connIdx[0] = 0;
    // g_ui_json.dispenserConfig.connIdx[1] = 1;
}

// 触摸测试
int g_touch_x = -1;
int g_touch_y = -1;

void draw_black_dot_simple(void)
{
    lv_obj_t * dot = lv_obj_create(lv_layer_top());
    lv_obj_set_size(dot, 10, 10);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, lv_color_black(), 0);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_pos(dot, g_touch_x - 5, g_touch_y - 5);

    lv_obj_move_foreground(dot);
}

void bright_step()
{
    // static int last_brightness = -1;  // 避免重复设亮度

    // time_t now = time(NULL);

    // if(g_ui_json.biright.activeTime == 0)
    // {
    //     g_ui_json.biright.activeTime = now;
    // }

    // int brightness_to_set = g_ui_json.biright.brightIdle;

    // // DEBUG_WARN("brightIdle[%d]",g_ui_json.biright.brightIdle);
    // // DEBUG_WARN("brightActive[%d]",g_ui_json.biright.brightActive);
    // // DEBUG_WARN("brightCharging[%d]",g_ui_json.biright.brightCharging);
    // // DEBUG_WARN("timeoutActive[%d]",g_ui_json.biright.timeoutActive);
    // // DEBUG_WARN("activeTime[%d]",g_ui_json.biright.activeTime);


    // // 判断是否处于激活状态
    // bool isActive = false;
    // if (g_ui_json.biright.activeTime > 0 &&
    //     now - g_ui_json.biright.activeTime < (g_ui_json.biright.timeoutActive * 60))
    // {
    //     isActive = true;
    // }

    // // DEBUG_WARN("isActive[%d]",isActive);

    // // 根据状态决定亮度
    // if (isActive)
    // {
    //     brightness_to_set = g_ui_json.biright.brightActive;
    // }
    // else
    // {
    //     for(int i = 0; i < UI_MAX_CONNECTOR_NUM; i++)
    //     {
    //         // 充电状态显示brightCharging亮度
    //         if(g_ui_json.connectors[i].state == UI_CONNECTOR_STATE_CHARGING)
    //         {
    //             brightness_to_set = g_ui_json.biright.brightCharging;
    //             break;
    //         }
    //     }
    // }

    // // 避免重复设亮度
    // if (brightness_to_set != last_brightness)
    // {
    //     LV_LOG_USER("Setting brightness to %d\n", brightness_to_set);
    //     set_brightness(brightness_to_set);
    //     last_brightness = brightness_to_set;
    // }
}

static bool language_load = false;
void ioc_app_step(void)
{
    // 自组网
    // autoGroup_step();

    // 设置屏幕亮度
    // bright_step();

    // 先等到第一个UI.json到达, 用于确定初始信息
    if (UI_FIRST_JSON)
    {
        if (ioc_ui_json_step())
        {
            LV_LOG_WARN("FIRST===========================================");

            // 等待UI.json获取homeOcpp设置的当前语言
            s_preset_handler_init();
            if (s_ui_welcome != NULL)
            {
                lv_obj_del(s_ui_welcome);
                s_ui_welcome = NULL;
            }

            ui_page_main_create(lv_scr_act());
            if ( language_load == false )
            {
                language_page_init();
                ui_language_update_selected(current_lang_point->locale_name);
                LV_LOG_WARN("current_lang_point->locale_name:%s",current_lang_point->locale_name);
                language_load = true;
            }

            // 强制刷新第一次内容
            ioc_app_ui_json_updated_widgets(lv_scr_act());
            ioc_ui_json_handle_over();

            ioc_app_dump_visible_obj_tree_to_json_file(lv_scr_act(), s_ui_visible_tree_json_name);
            ioc_app_dump_visible_obj_info_to_json_file(lv_scr_act(), s_ui_visible_obj_info_json_name);

            g_ui_json.isFirstUIjson = false;
        }
        return;
    }

    // if (access("/tmp/theme-main-dark", F_OK) == 0)
    // {
    //     ioc_theme_mngr_switch("main-dark");
    //     ioc_style_mngr_theme_changed();
    //     lv_obj_invalidate(lv_scr_act());
    //     remove("/tmp/theme-main-dark");
    // }
    // if (access("/tmp/theme-main-light", F_OK) == 0)
    // {
    //     ioc_theme_mngr_switch("main-light");
    //     ioc_style_mngr_theme_changed();
    //     lv_obj_invalidate(lv_scr_act());
    //     remove("/tmp/theme-main-light");
    // }

    if (ioc_ui_json_step())
    {
        ioc_app_ui_json_updated_widgets(lv_scr_act());
        ioc_app_ui_json_updated_widgets(lv_layer_top());

        ioc_ui_json_handle_over();
        ioc_app_dump_visible_obj_tree_to_json_file(lv_scr_act(), s_ui_visible_tree_json_name);
        ioc_app_dump_visible_obj_info_to_json_file(lv_scr_act(), s_ui_visible_obj_info_json_name);
    }


    // 触摸测试
    // 如果有触摸，画一个直径为6的圆
    if(g_touch_x > 0 && g_touch_y > 0)
    {
        draw_black_dot_simple();
        g_touch_x = -1;
        g_touch_y = -1;
    }
}


bool ioc_obj_handler_match_preset_name(const char * preset_name, const char *name)
{
    // 先比较基本符合类型
    int preset_name_len = strlen(preset_name);
    if (memcmp(preset_name, name, preset_name_len) == 0)
    {
        return true;
    }

    // 再比较Prefix类型
    int prefix_len;
    if (ioc_obj_name_is_prefix(preset_name, &prefix_len))
    {
        int idx;
        const char * name_after_prefix;
        int  name_after_prefix_len;
        return ioc_obj_name_match_prefix(name, preset_name, prefix_len, &idx, &name_after_prefix, &name_after_prefix_len);
    }

    return false;
}

bool ioc_obj_handler_before_create(const char *name)
{
    int size = ioc_get_preset_handler_count();

    for (int i = 0; i < size; i++)
    {
        if (ioc_obj_handler_match_preset_name(s_preset_handler[i].name, name))
        {
            if (s_preset_handler[i].before_create)
            {
                return s_preset_handler[i].before_create(name);
            }

            return true;
        }
    }

    return true;
}
void ioc_obj_handler_after_create(const char *name, lv_obj_t *obj)
{
    int size = ioc_get_preset_handler_count();

    for (int i = 0; i < size; i++)
    {
        if (ioc_obj_handler_match_preset_name(s_preset_handler[i].name, name))
        {
            if (s_preset_handler[i].after_create)
            {
                s_preset_handler[i].after_create(name, obj);
            }

            return;
        }
    }
}
