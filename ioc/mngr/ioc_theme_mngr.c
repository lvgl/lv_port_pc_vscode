#include "ioc_app.h"

static cJSON * s_themes_json = NULL;

static ioc_theme_t ** s_themes = NULL;
static int s_themes_count = 0;
static int s_themes_idx = 0;

static void ioc_theme_release(ioc_theme_t *theme)
{
    if (theme != NULL)
    {
        if (theme->colors != NULL)
        {
            free(theme->colors);
        }
        if (theme->fonts != NULL)
        {
            free(theme->fonts);
        }
        if (theme->imgs != NULL)
        {
            free(theme->imgs);
        }
        free(theme);
    }
}

void ioc_theme_mngr_unload(void)
{
    if (s_themes != NULL)
    {
        for (int i = 0; i < s_themes_count; i++)
        {
            ioc_theme_release(s_themes[i]);
        }
        free(s_themes);
        s_themes = NULL;
    }
    s_themes_count = 0;

    if (s_themes_json != NULL)
    {
        cJSON_Delete(s_themes_json);
        s_themes_json = NULL;
    }
}

// 主函数：解析 JSON 数据
ioc_theme_t *ioc_theme_parse(cJSON *theme_json)
{
    ioc_theme_t *theme = (ioc_theme_t *)malloc(sizeof(ioc_theme_t));
    if (!theme)
    {
        LV_LOG_ERROR("malloc failed");
        return NULL;
    }

    // 解析主题名称
    cJSON *name = cJSON_GetObjectItem(theme_json, "name");
    if ((name == NULL) || !cJSON_IsString(name))
    {
        LV_LOG_ERROR("miss name");
        free(theme);
        return NULL;
    }
    theme->name = name->valuestring;

    // 解析颜色
    cJSON *colors = cJSON_GetObjectItem(theme_json, "colors");
    if (colors == NULL || !cJSON_IsArray(colors))
    {
        LV_LOG_ERROR("miss colors");
        free(theme);
        return NULL;
    }
    int color_count = cJSON_GetArraySize(colors);
    theme->colors = (ioc_theme_color_t *)calloc(color_count, sizeof(ioc_theme_color_t));
    theme->color_count = color_count;
    for (int i = 0; i < color_count; i++) {
        cJSON *color_item = cJSON_GetArrayItem(colors, i);
        cJSON *color_name = cJSON_GetObjectItem(color_item, "name");
        cJSON *color_value = cJSON_GetObjectItem(color_item, "value");
        if (color_name != NULL && color_value != NULL && cJSON_IsString(color_name) && cJSON_IsString(color_value))
        {
            theme->colors[i].name = color_name->valuestring;
            theme->colors[i].value = color_value->valuestring;
            LV_LOG_INFO("color: %s", color_name->valuestring);
        }
        else
        {
            LV_LOG_ERROR("miss color name or value");
            free(theme);
            return NULL;
        }
    }

    // 解析字体
    cJSON *fonts = cJSON_GetObjectItem(theme_json, "fonts");
    if (fonts == NULL || !cJSON_IsArray(fonts))
    {
        LV_LOG_ERROR("miss fonts");
        free(theme);
        return NULL;
    }
    int font_count = cJSON_GetArraySize(fonts);
    theme->fonts = (ioc_theme_font_t *)calloc(font_count, sizeof(ioc_theme_font_t));
    theme->font_count = font_count;
    for (int i = 0; i < font_count; i++) {
        cJSON *font_item = cJSON_GetArrayItem(fonts, i);
        cJSON *font_name = cJSON_GetObjectItem(font_item, "name");
        cJSON *font_family = cJSON_GetObjectItem(font_item, "family");
        cJSON *font_size = cJSON_GetObjectItem(font_item, "size");
        cJSON *font_bold = cJSON_GetObjectItem(font_item, "bold");
        if (font_name != NULL && font_family != NULL && font_size != NULL && cJSON_IsString(font_name) && cJSON_IsString(font_family) && cJSON_IsNumber(font_size))
        {
            theme->fonts[i].name = font_name->valuestring;
        #if (LVGL_VERSION_MAJOR == 9)
            #if LV_USE_FREETYPE
            theme->fonts[i].font = lv_freetype_font_create(font_family->valuestring,
                (font_size->valueint > 16) ? LV_FREETYPE_FONT_RENDER_MODE_BITMAP : LV_FREETYPE_FONT_RENDER_MODE_OUTLINE,
                font_size->valueint,
                (font_bold != NULL && cJSON_IsBool(font_bold)) ? LV_FREETYPE_FONT_STYLE_BOLD : LV_FREETYPE_FONT_STYLE_NORMAL);
            #else
            theme->fonts[i].font = LV_FONT_DEFAULT;
            #endif
        #else
            theme->fonts[i].font_info.name = font_family->valuestring;
            theme->fonts[i].font_info.weight = font_size->valueint;
            if (font_bold != NULL && cJSON_IsBool(font_bold))
            {
                theme->fonts[i].font_info.style = FT_FONT_STYLE_BOLD;
            }
            else
            {
                theme->fonts[i].font_info.style = FT_FONT_STYLE_NORMAL;
            }
            lv_ft_font_init(&theme->fonts[i].font_info);
        #endif
            LV_LOG_INFO("font: %s", font_name->valuestring);
        }
        else
        {
            LV_LOG_ERROR("miss font name or family or size");
            free(theme);
            return NULL;
        }

    }

    // 解析图片
    cJSON *imgs = cJSON_GetObjectItem(theme_json, "images");
    if (imgs == NULL || !cJSON_IsArray(imgs))
    {
        LV_LOG_ERROR("miss images");
        free(theme);
        return NULL;
    }
    int img_count = cJSON_GetArraySize(imgs);
    theme->imgs = (ioc_theme_img_t *)calloc(img_count, sizeof(ioc_theme_img_t));
    theme->img_count = img_count;
    for (int i = 0; i < img_count; i++) {
        cJSON *img_item = cJSON_GetArrayItem(imgs, i);
        cJSON *img_name = cJSON_GetObjectItem(img_item, "name");
        cJSON *img_path = cJSON_GetObjectItem(img_item, "path");
        if (img_name != NULL && img_path != NULL && cJSON_IsString(img_name) && cJSON_IsString(img_path))
        {
            theme->imgs[i].name = img_name->valuestring;
            theme->imgs[i].path = img_path->valuestring;
            // printf("img: %s->%s\n", img_name->valuestring, img_path->valuestring);
        }
        else
        {
            LV_LOG_ERROR("miss img name or path");
            free(theme);
            return NULL;
        }
    }

    return theme;
}

static bool ioc_theme_mngr_load_dir(const char * theme_path)
{
    DIR *dir = opendir(theme_path);
    if (dir == NULL)
    {
        LV_LOG_ERROR("failed %s", theme_path);
        return false;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s/%s", theme_path, entry->d_name);
        cJSON *theme_json = JSON_FromFile(file_path);
        if (theme_json == NULL)
        {
            LV_LOG_ERROR("cant't open %s", file_path);
            closedir(dir);
            return false;
        }
        cJSON_Merge(s_themes_json, theme_json);
        cJSON_Delete(theme_json);
    }
    closedir(dir);
    return true;
}

bool ioc_theme_mngr_load(const char * using_theme_name)
{
    // 初始化一个空的主题包
    ioc_theme_mngr_unload();
    s_themes_json = cJSON_CreateArray();

    if (!ioc_theme_mngr_load_dir(THEME_ROOT_PATH))
    {
        LV_LOG_ERROR("Can't load theme root path %s", THEME_ROOT_PATH);
        return false;
    }
    if (access(THEME_PATCH_PATH, F_OK) == 0)
    {
        if (!ioc_theme_mngr_load_dir(THEME_PATCH_PATH))
        {
            LV_LOG_ERROR("Can't load theme patch path %s", THEME_PATCH_PATH);
            return false;
        }
    }
    else
    {
        LV_LOG_INFO("Skip theme patch path %s", THEME_PATCH_PATH);
    }


    // char * p = cJSON_Print(s_themes_json);
    // printf("%s\n", p);
    // cJSON_free(p);

    s_themes_idx = 0;
    s_themes_count = cJSON_GetArraySize(s_themes_json);
    s_themes = (ioc_theme_t **)calloc(s_themes_count, sizeof(ioc_theme_t*));
    for (int i = 0; i < s_themes_count; i++)
    {
        s_themes[i] = ioc_theme_parse(cJSON_GetArrayItem(s_themes_json, i));
        if (s_themes[i] == NULL)
        {
            LV_LOG_ERROR("can't parse theme");
            ioc_theme_mngr_unload();
            return false;
        }

        if (using_theme_name != NULL && strcmp(s_themes[i]->name, using_theme_name) == 0)
        {
            s_themes_idx = i;
            LV_LOG_WARN("Using %s at %d", s_themes[i]->name, s_themes_idx);
        }
    }

    return true;
}

bool ioc_theme_mngr_switch(const char * using_theme_name)
{
    for (int i = 0; i < s_themes_count; i++)
    {
        if (strcmp(s_themes[i]->name, using_theme_name) == 0)
        {
            if (s_themes_idx != i)
            {
                s_themes_idx = i;
                LV_LOG_WARN("Using %s at %d", s_themes[i]->name, s_themes_idx);
                return true;
            }
            else
            {
                LV_LOG_WARN("Already using %s at %d", s_themes[i]->name, s_themes_idx);
                return false;
            }
        }
    }

    LV_LOG_ERROR("Theme %s not found", using_theme_name);
    return false;
}

lv_color_t ioc_theme_getcolor(const char * name)
{
    if (s_themes_idx < s_themes_count)
    {
        for (int i = 0; i < s_themes[s_themes_idx]->color_count; i++)
        {
            if (strcmp(s_themes[s_themes_idx]->colors[i].name, name) == 0)
            {
                int r, g, b;
                sscanf(s_themes[s_themes_idx]->colors[i].value, "#%2x%2x%2x", &r, &g, &b);
                return lv_color_make(r & 0xFF, g & 0xFF, b & 0xFF);
            }
        }
    }

    return lv_color_black();
}

const lv_font_t * ioc_theme_getfont(const char * name)
{
    if (s_themes_idx < s_themes_count)
    {
        for (int i = 0; i < s_themes[s_themes_idx]->font_count; i++)
        {
            if (strcmp(s_themes[s_themes_idx]->fonts[i].name, name) == 0)
            {
            #if (LVGL_VERSION_MAJOR == 9)
                return s_themes[s_themes_idx]->fonts[i].font;
            #else
                return s_themes[s_themes_idx]->fonts[i].font_info.font;
            #endif
            }
        }
    }
#if (LVGL_VERSION_MAJOR == 9)
    return s_themes[0]->fonts[0].font;
#else
    return s_themes[0]->fonts[0].font_info.font;
#endif
}

const char * ioc_theme_getimgpath(const char * name)
{
    if (s_themes_idx < s_themes_count)
    {
        for (int i = 0; i < s_themes[s_themes_idx]->img_count; i++)
        {
            if (strcmp(s_themes[s_themes_idx]->imgs[i].name, name) == 0)
            {
                return s_themes[s_themes_idx]->imgs[i].path;
                // LV_LOG_WARN("path[%s]", s_themes[s_themes_idx]->imgs[i].path);
            }
        }
    }

    LV_LOG_WARN("name[%s]  path NULL", name);

    return NULL;
}
