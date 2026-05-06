#include "ioc_app.h"

static cJSON *s_styles_json = NULL;

static ioc_style_t **s_styles = NULL;
static int s_styles_cnt = 0;
static int s_style_idx = 0;


static void ioc_style_mngr_update_style(ioc_style_t * ioc_style, cJSON *style_json, bool init)
{
    cJSON * styles = cJSON_GetObjectItem(style_json, "styles");
    if (styles == NULL)
    {
        return;
    }

    for (int i = 0; i < ioc_style->style_count; i++)
    {
        cJSON * style = cJSON_GetArrayItem(styles, i);
        if (style == NULL)
        {
            break;
        }

        if (init)
        {
            ioc_style->styles[i].name = cJSON_GetObjectItem(style, "name")->valuestring;

            lv_style_init(&ioc_style->styles[i].style);
        }

        const char *bg_color_name = cJSON_GetStringValue(cJSON_GetObjectItem(style, "bg_color_name"));
        if (bg_color_name != NULL)
        {
            lv_style_set_bg_color(&ioc_style->styles[i].style, ioc_theme_getcolor(bg_color_name));
            // LV_LOG_WARN("bg_color_name[%s]  [%x]", bg_color_name, ioc_theme_getcolor(bg_color_name));

        }
        cJSON * border_radius = cJSON_GetObjectItem(style, "border_radius");
        if (border_radius != NULL)
        {
            lv_style_set_radius(&ioc_style->styles[i].style, border_radius->valueint);
        }
        cJSON * border_width = cJSON_GetObjectItem(style, "border_width");
        if (border_width != NULL)
        {
            lv_style_set_border_width(&ioc_style->styles[i].style, border_width->valueint);
        }

        lv_style_set_pad_all(&ioc_style->styles[i].style, 0);
        cJSON * padding = cJSON_GetObjectItem(style, "padding");
        if (padding != NULL)
        {
            lv_style_set_pad_all(&ioc_style->styles[i].style, padding->valueint);
        }

        cJSON * width = cJSON_GetObjectItem(style, "width");
        if (width != NULL)
        {
            lv_style_set_width(&ioc_style->styles[i].style, width->valueint);
        }
        cJSON * height = cJSON_GetObjectItem(style, "height");
        if (height != NULL)
        {
            lv_style_set_height(&ioc_style->styles[i].style, height->valueint);
        }
        const char *border_color_name = cJSON_GetStringValue(cJSON_GetObjectItem(style, "border_color_name"));
        if (border_color_name != NULL)
        {
            lv_style_set_border_color(&ioc_style->styles[i].style, ioc_theme_getcolor(border_color_name));
        }
        const char *font_name = cJSON_GetStringValue(cJSON_GetObjectItem(style, "font_name"));
        if (font_name != NULL)
        {
            lv_style_set_text_font(&ioc_style->styles[i].style, ioc_theme_getfont(font_name));
        }
        const char *text_color_name = cJSON_GetStringValue(cJSON_GetObjectItem(style, "text_color_name"));
        if (text_color_name != NULL)
        {
            lv_style_set_text_color(&ioc_style->styles[i].style, ioc_theme_getcolor(text_color_name));
        }
        cJSON * opacity = cJSON_GetObjectItem(style, "opacity");
        if (opacity != NULL)
        {
            lv_style_set_bg_opa(&ioc_style->styles[i].style, opacity->valueint);
        }
    }
}

static ioc_style_t *ioc_style_mngr_load_style(cJSON *style_json)
{
    ioc_style_t * ioc_style = (ioc_style_t *)malloc(sizeof(ioc_style_t));
    if (ioc_style == NULL)
    {
        LV_LOG_ERROR("malloc failed");
        return NULL;
    }

    ioc_style->name = strdup(cJSON_GetObjectItem(style_json, "name")->valuestring);

    ioc_style->style_count = 0;
    cJSON * styles = cJSON_GetObjectItem(style_json, "styles");
    if (styles != NULL)
    {
        ioc_style->style_count = cJSON_GetArraySize(styles);
        ioc_style->styles = (ioc_style_one_t *)calloc(ioc_style->style_count, sizeof(ioc_style_one_t));
        if (ioc_style->styles == NULL)
        {
            LV_LOG_ERROR("malloc failed");
            free(ioc_style);
            return NULL;
        }

        ioc_style_mngr_update_style(ioc_style, style_json, true);
    }

    return ioc_style;
}

static bool ioc_style_mngr_load_dir(const char *style_path)
{
    DIR *dir = opendir(style_path);
    if (dir == NULL)
    {
        LV_LOG_ERROR("failed %s", style_path);
        return false;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s/%s", style_path, entry->d_name);
        cJSON *style_json = JSON_FromFile(file_path);
        if (style_json == NULL)
        {
            LV_LOG_ERROR("cant't open %s", file_path);
            closedir(dir);
            return false;
        }
        cJSON_Merge(s_styles_json, style_json);
        cJSON_Delete(style_json);
    }
    closedir(dir);
    return true;
}

bool ioc_style_mngr_load(const char *using_style_name)
{
    // 初始化一个空的片段包
    ioc_style_mngr_unload();
    s_styles_json = cJSON_CreateArray();

    if (!ioc_style_mngr_load_dir(STYLE_ROOT_PATH))
    {
        LV_LOG_ERROR("Can't load style root path %s", STYLE_ROOT_PATH);
        return false;
    }
    if (access(STYLE_PATCH_PATH, F_OK) == 0)
    {
        if (!ioc_style_mngr_load_dir(STYLE_PATCH_PATH))
        {
            LV_LOG_ERROR("Can't load style patch path %s", STYLE_PATCH_PATH);
            return false;
        }
    }
    else
    {
        LV_LOG_INFO("Skip style patch path %s", STYLE_PATCH_PATH);
    }

    // char * p = cJSON_Print(s_styles_json);
    // printf("%s\n", p);
    // cJSON_free(p);

    s_style_idx = 0;
    s_styles_cnt = cJSON_GetArraySize(s_styles_json);
    s_styles = (ioc_style_t **)calloc(s_styles_cnt, sizeof(ioc_style_t *));
    for (int i = 0; i < s_styles_cnt; i++)
    {
        s_styles[i] = ioc_style_mngr_load_style(cJSON_GetArrayItem(s_styles_json, i));
        if (s_styles[i] == NULL)
        {
            LV_LOG_ERROR("load style failed");
            ioc_style_mngr_unload();
            return false;
        }

        if (using_style_name != NULL && strcmp(s_styles[i]->name, using_style_name) == 0)
        {
            s_style_idx = i;
            LV_LOG_WARN("Using %s at %d", s_styles[i]->name, s_style_idx);
        }
    }

    return true;
}

static void ioc_style_release(ioc_style_t *style)
{
    if (style != NULL)
    {
        if (style->name != NULL)
        {
            free(style->name);
            style->name = NULL;
        }

        if (style->styles)
        {
            free(style->styles);
            style->styles = NULL;
        }

        free(style);
    }
}

void ioc_style_mngr_unload(void)
{
    if (s_styles != NULL)
    {
        for (int i = 0; i < s_styles_cnt; i++)
        {
            ioc_style_release(s_styles[i]);
        }
    }
    free(s_styles);
    s_styles = NULL;
    s_styles_cnt = 0;

    if (s_styles_json != NULL)
    {
        cJSON_Delete(s_styles_json);
        s_styles_json = NULL;
    }
}

void ioc_style_mngr_theme_changed(void)
{
    ioc_style_mngr_update_style(s_styles[s_style_idx], cJSON_GetArrayItem(s_styles_json, s_style_idx), false);
}

lv_style_t *ioc_style_mngr_get_style(const char *style_name)
{

    if (s_styles == NULL)
    {
        return NULL;
    }
    if (s_style_idx < s_styles_cnt)
    {
        for (int i = 0; i < s_styles[s_style_idx]->style_count; i++)
        {
            if (strcmp(s_styles[s_style_idx]->styles[i].name, style_name) == 0)
            {
                return &s_styles[s_style_idx]->styles[i].style;
            }
        }
    }

    return NULL;
}
