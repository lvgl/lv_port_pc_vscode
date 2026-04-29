#include "ioc_app.h"

static lv_obj_t * s_ui_welcome = NULL;

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
