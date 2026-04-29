#include "../pages_mngr.h"
#include <dirent.h>

#define UI_NAME_LANGUAGE_CLOSE_BTN       ".language_close_btn"
#define UI_NAME_LANGUAGE_CONFIRM_BTN     ".language_confirm_btn"
#define UI_NAME_LANGUAGE_CLOSE_TEXT      ".language_close_btn.btn_label"
#define UI_NAME_LANGUAGE_CONFIRM_TEXT    ".language_confirm_btn.btn_label"
#define UI_NAME_LANGUAGE_LABEL           ".language_title"

#define LANGUAGE_IMG_PATH "./assets/i18n"
#define MAX_LANGUAGES 18

ui_language_t s_ui_language = {NULL, NULL, NULL};

int compare_lang_label(const void *a, const void *b) {
    const lang_info_t *la = (const lang_info_t *)a;
    const lang_info_t *lb = (const lang_info_t *)b;
    return la->label[0] - lb->label[0];
}

// 用户手动设置语言
bool set_current_language_by_locale(const char *locale)
{
    if (!locale || strlen(locale) == 0) return false;

    for (int i = 0; i < LANG_NUM; ++i) {
        if (strcmp(c_LanguageName[i], locale) == 0) {
            lv_i18n_set_locale(c_LanguageName[i]);
            lv_refr_now(NULL);
            return true;
        }
    }

    LV_LOG_WARN("Unsupported locale: %s", locale);
    return false;
}
void language_set_style(lv_obj_t *btn)
{
    lv_style_t *btn_style_checked = ioc_style_mngr_get_style("language_checked");
    lv_style_t *btn_style_unchecked = ioc_style_mngr_get_style("language_unchecked");

    // 如果点击的是当前已经选中的按钮，则不处理
    if (btn != s_ui_language.selected_btn) {
        // 移除上一个按钮的选中样式
        if (s_ui_language.selected_btn) {
            lv_obj_add_style(s_ui_language.selected_btn, btn_style_unchecked, 0);
        }
        if ( s_ui_language.selected_btn && s_ui_language.selected_btn != s_ui_language.selected_btn )
        {
            lv_obj_add_style(s_ui_language.selected_btn, btn_style_unchecked, 0);
        }
        // 设置当前按钮样式为选中
        lv_obj_remove_style(btn, btn_style_checked, 0);
        lv_obj_add_style(btn, btn_style_checked, 0);
        s_ui_language.selected_btn = btn;

    }
}

// 重置语言选择状态
void reset_language_selection(void)
{
    lv_style_t *btn_style_checked = ioc_style_mngr_get_style("language_checked");
    lv_style_t *btn_style_unchecked = ioc_style_mngr_get_style("language_unchecked");

    // 重置所有按钮样式为未选中
    for (int i = 0; i < s_ui_language.lang_count; i++) {
        lv_obj_add_style(s_ui_language.lang_buttons[i].btn_obj, btn_style_unchecked, 0);
    }

    // 重新设置当前语言的选中状态
    const char *cur_locale = current_lang_point->locale_name;
    for (int i = 0; i < s_ui_language.lang_count; i++) {
        if (strcmp(s_ui_language.lang_buttons[i].info.locale, cur_locale) == 0) {
            lv_obj_remove_style(s_ui_language.lang_buttons[i].btn_obj, btn_style_checked, 0);
            lv_obj_add_style(s_ui_language.lang_buttons[i].btn_obj, btn_style_checked, 0);
            s_ui_language.selected_locale = s_ui_language.lang_buttons[i].info.locale;
            s_ui_language.selected_btn = s_ui_language.lang_buttons[i].btn_obj;
            break;
        }
    }
}

void ui_language_update_selected(const char *str);
static void languages_btn_event_cb(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_t *btn = lv_event_get_target(e);
        lang_info_t *info = (lang_info_t *)lv_event_get_user_data(e);
        language_set_style(btn);
        if (info) {
            s_ui_language.selected_locale = info->locale;
            s_ui_language.check_language = true;
            LV_LOG_WARN("Language selection: locale=%s, label=%s, filename=%s\n",
                   info->locale, info->label, info->filename);
        }
    }
}


lv_obj_t *create_language_button(lv_obj_t *parent, const lang_info_t *info, lv_coord_t x, lv_coord_t y) {
    lv_style_t *btn_style = ioc_style_mngr_get_style("style-shortcut_button_pre_authorization");

    // 创建按钮
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, 180, 40);
    lv_obj_add_style(btn, btn_style, 0);
    lv_obj_add_event_cb(btn, languages_btn_event_cb, LV_EVENT_CLICKED, (void *)info);

    // 创建内部容器（用于水平排列图标和文字）
    lv_obj_t *container = lv_obj_create(btn);
    lv_obj_remove_style_all(container);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_CLICKABLE);  // 防止拦截按钮点击事件
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(container);

    lv_obj_set_layout(container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(container, 10, 0);

    // 图标
    if (info->icon[0]) {
        lv_obj_t *img = lv_img_create(container);
        lv_img_set_src(img, info->icon);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    }

    // 标签文字
    lv_obj_t *label_obj = lv_label_create(container);
    ioc_label_set_text(label_obj, info->label);
    lv_style_t *label_style = ioc_style_mngr_get_style("style-connector-quad-default-type-text");
    lv_obj_add_style(label_obj, label_style, 0);

    return btn;
}

int parse_language_json(const char *json_path, char *out_label, size_t label_size, char *out_icon, size_t icon_size) {
    FILE *f = fopen(json_path, "r");
    if (!f) return -1;

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) return -1;

    const cJSON *label = cJSON_GetObjectItemCaseSensitive(root, "label");
    const cJSON *icon = cJSON_GetObjectItemCaseSensitive(root, "icon");

    if (cJSON_IsString(label) && label->valuestring)
        strncpy(out_label, label->valuestring, label_size);

    if (cJSON_IsString(icon) && icon->valuestring)
        strncpy(out_icon, icon->valuestring, icon_size);

    cJSON_Delete(root);
    return 0;
}

void load_language_buttons(lv_obj_t *parent) {
    const char *folder = LANGUAGE_IMG_PATH;
    DIR *d = opendir(folder);
    if (!d) {
        perror("opendir");
        return;
    }
    static lang_button_t lang_buttons[MAX_LANGUAGES];  // 存放语言按钮及信息
    struct dirent *entry;
    s_ui_language.lang_count = 0;

    // 读取 JSON 信息到 lang_buttons[i].info
    while ((entry = readdir(d)) != NULL && s_ui_language.lang_count < MAX_LANGUAGES) {
        if (!strstr(entry->d_name, ".json")) continue;

        char path[512];
        snprintf(path, sizeof(path), "%s/%s", folder, entry->d_name);

        lang_button_t *btn = &lang_buttons[s_ui_language.lang_count];

        if (parse_language_json(path,
                                btn->info.label, sizeof(btn->info.label),
                                btn->info.icon, sizeof(btn->info.icon)) == 0) {
            strncpy(btn->info.filename, entry->d_name, sizeof(btn->info.filename));
            // 可选：从 filename 中提取 locale，如 "en.json" -> "en"
            strncpy(btn->info.locale, entry->d_name, sizeof(btn->info.locale));
            char *dot = strrchr(btn->info.locale, '.');
            if (dot) *dot = '\0'; // 去掉 .json

            s_ui_language.lang_count++;
        }
    }
    closedir(d);

    // 按 label 排序
    qsort(lang_buttons, s_ui_language.lang_count, sizeof(lang_button_t), compare_lang_label);

    // 生成按钮并保存 lv_obj_t*
    const lv_coord_t base_x[3] = {40, 240, 440};
    const lv_coord_t base_y = 85;
    const lv_coord_t row_spacing = 60;

    for (int i = 0; i < s_ui_language.lang_count; i++) {
        int row = i / 3;
        int col = i % 3;

        lv_coord_t x = base_x[col];
        lv_coord_t y = base_y + row * row_spacing;
        lang_buttons[i].btn_obj = create_language_button(parent, &lang_buttons[i].info, x, y);
    }

    // 保存到 UI 状态
    s_ui_language.lang_buttons = lang_buttons;
}

// 显示语言选择页面
void ui_popup_show_language(void)
{
    ui_popup_show_mask(true);   // 显示遮罩层
    static int execute = 0;
    if (s_ui_language.language == NULL)
    {
        s_ui_language.language = ioc_layout_mngr_load_layout("language", lv_layer_top());
    }

    if (s_ui_language.language != NULL)
    {
        lv_obj_clear_flag(s_ui_language.language, LV_OBJ_FLAG_HIDDEN);
    }
    if ( execute == 0 ) {
        load_language_buttons(s_ui_language.language);
        execute = 1;
    }
    ioc_label_set_text(s_ui_language.close_text, "Close");
    ioc_label_set_text(s_ui_language.confirm_text, "Confirm");
    ioc_label_set_text(s_ui_language.language_label, "Language");
}

static void ui_language_close_btn_event_cb(lv_event_t * e)
{
    s_ui_language.confirm_language = false;

    // 重置语言选择状态
    reset_language_selection();

    lv_obj_add_flag(s_ui_language.language, LV_OBJ_FLAG_HIDDEN);
    ui_popup_show_mask(false);   // 关闭遮罩层
}

static void ui_language_confirm_btn_event_cb(lv_event_t * e)
{
    // 如果用户确实选了语言，则应用它
    if (s_ui_language.check_language && s_ui_language.selected_locale) {
        if (set_current_language_by_locale(s_ui_language.selected_locale)) {
            printf("Language confirmation: Switch to %s\n", s_ui_language.selected_locale);
            ui_language_update_selected(s_ui_language.selected_locale);
        }
    }

    // 关闭语言选择界面
    lv_obj_add_flag(s_ui_language.language, LV_OBJ_FLAG_HIDDEN);
    ui_popup_show_mask(false);
}

void language_page_init(void)
{
    s_ui_language.confirm_language = true;
    ui_popup_show_language();
    lv_obj_add_flag(s_ui_language.language, LV_OBJ_FLAG_HIDDEN);
    ui_popup_show_mask(false);
}

void ui_after_create_language_close_btn(const char * name, lv_obj_t * obj)
{
    s_ui_language.close_btn = obj;
    lv_obj_add_event_cb(s_ui_language.close_btn, ui_language_close_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_language_confirm_btn(const char * name, lv_obj_t * obj)
{
    s_ui_language.confirm_btn = obj;
    lv_obj_add_event_cb(s_ui_language.confirm_btn, ui_language_confirm_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

void ui_after_create_language_close_text(const char * name, lv_obj_t * obj)
{
    s_ui_language.close_text = obj;
}

void ui_after_create_language_confirm_text(const char * name, lv_obj_t * obj)
{
    s_ui_language.confirm_text = obj;
}

void ui_after_create_language_label(const char * name, lv_obj_t * obj)
{
    s_ui_language.language_label = obj;
}

void ui_after_create_language(const char * name, lv_obj_t * obj)
{
    const char * language_name = name + strlen(UI_NAME_LANGUAGE);
    if (strcmp(language_name, UI_NAME_LANGUAGE_CLOSE_BTN) == 0)
    {
        ui_after_create_language_close_btn(name, obj);
    }
    else if (strcmp(language_name, UI_NAME_LANGUAGE_CONFIRM_BTN) == 0)
    {
        ui_after_create_language_confirm_btn(name, obj);
    }
    else if (strcmp(language_name, UI_NAME_LANGUAGE_CLOSE_TEXT) == 0)
    {
        ui_after_create_language_close_text(name, obj);
    }
    else if (strcmp(language_name, UI_NAME_LANGUAGE_CONFIRM_TEXT) == 0)
    {
        ui_after_create_language_confirm_text(name, obj);
    }
    else if (strcmp(language_name, UI_NAME_LANGUAGE_LABEL) == 0)
    {
        ui_after_create_language_label(name, obj);
    }
}

// 用于国际化（i18n），将 JSON 文件中定义的语言内容读取进 lv_i18n_lang_t 结构体中，用于 LVGL 界面多语言切换。
void parse_all_lang_jsons(void) {
    const char *dir_path = LANGUAGE_IMG_PATH;
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Failed to open i18n directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // 跳过不是 .json 文件
        if (!strstr(entry->d_name, ".json")) continue;

        // 构建完整文件路径
        char filepath[512];
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);

        // 打开并读取文件内容
        FILE *fp = fopen(filepath, "r");
        if (!fp) {
            perror("File open failed");
            continue;
        }

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        rewind(fp);

        char *json_buf = (char *)malloc(size + 1);
        if (!json_buf) {
            fclose(fp);
            LV_LOG_WARN("Memory allocation failed\n");
            continue;
        }

        fread(json_buf, 1, size, fp);
        json_buf[size] = '\0';
        fclose(fp);

        cJSON *root = cJSON_Parse(json_buf);
        if (!root) {
            LV_LOG_WARN("JSON Parse Error in %s: %s\n", filepath, cJSON_GetErrorPtr());
            free(json_buf);
            continue;
        }

        cJSON *name = cJSON_GetObjectItem(root, "name");

        if (!cJSON_IsString(name)) {
            LV_LOG_WARN("Invalid name field in %s\n", filepath);
            cJSON_Delete(root);
            free(json_buf);
            continue;
        }

        const char *lang_prefix = name->valuestring;
        // LV_LOG_WARN("lang_prefix:%s\n",lang_prefix);
        lv_i18n_lang_t *target_lang = NULL;

        for (int i = 0; lv_i18n_language_pack[i] != NULL; i++) {
            // LV_LOG_WARN("lv_i18n_language_pack[%d]->locale_name:%s\n",i,lv_i18n_language_pack[i]->locale_name);
            if (strncmp(lv_i18n_language_pack[i]->locale_name, lang_prefix, 2) == 0) {
                target_lang = (lv_i18n_lang_t *)lv_i18n_language_pack[i];
                // LV_LOG_WARN("target_lang->locale_name:%s\n",target_lang->locale_name);
                break;
            }
        }

        if (!target_lang) {
            LV_LOG_WARN("No matching language or singulars array is NULL for: %s\n", lang_prefix);
            cJSON_Delete(root);
            free(json_buf);
            continue;
        }

        cJSON *content = cJSON_GetObjectItem(root, "content");
        if (!cJSON_IsArray(content)) {
            LV_LOG_WARN("Invalid content field in %s\n", filepath);
            cJSON_Delete(root);
            free(json_buf);
            continue;
        }

        int count = cJSON_GetArraySize(content);
        if (target_lang->singulars == NULL)
        {
            target_lang->singulars = calloc(count + 1, sizeof(lv_i18n_phrase_t));
        }
        for (int i = 0; i < count; i++) {
            cJSON *item = cJSON_GetArrayItem(content, i);
            if (!cJSON_IsObject(item)) continue;

            cJSON *msg_id = cJSON_GetObjectItem(item, "name");
            cJSON *translation = cJSON_GetObjectItem(item, "value");

            if (cJSON_IsString(msg_id) && cJSON_IsString(translation)) {
                target_lang->singulars[i].msg_id = strdup(msg_id->valuestring);
                target_lang->singulars[i].translation = strdup(translation->valuestring);
            }
        }

        target_lang->singulars[count].msg_id = NULL;
        target_lang->singulars[count].translation = NULL;

        cJSON_Delete(root);
        free(json_buf);
    }

    closedir(dir);
}


void ui_language_update_selected(const char *str)
{
    if ( !s_ui_language.lang_buttons || !str ) return;
    for (int i = 0; i < s_ui_language.lang_count; i++) {
        if ( strcmp(s_ui_language.lang_buttons[i].info.locale, str) == 0 )
        {
            language_set_style(s_ui_language.lang_buttons[i].btn_obj);
            lv_img_set_src(ui_status_bar.lang_icon, s_ui_language.lang_buttons[i].info.icon);
            ioc_label_set_text(ui_status_bar.lang_label, s_ui_language.lang_buttons[i].info.label);
        }
    }
}
