
#include "ioc_app.h"

static cJSON * s_layouts_json = NULL;

static bool ioc_layout_mngr_load_dir(const char * layout_path)
{
    DIR *dir = opendir(layout_path);
    if (dir == NULL)
    {
        LV_LOG_ERROR("failed %s", layout_path);
        return false;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s/%s", layout_path, entry->d_name);
        cJSON *layout_json = JSON_FromFile(file_path);
        if (layout_json == NULL)
        {
            LV_LOG_ERROR("cant't open %s", file_path);
            closedir(dir);
            return false;
        }
        cJSON_Merge(s_layouts_json, layout_json);
        cJSON_Delete(layout_json);
    }
    closedir(dir);
    return true;
}

bool ioc_layout_mngr_load(void)
{
    // 初始化一个空的片段包
    ioc_layout_mngr_unload();
    s_layouts_json = cJSON_CreateArray();

    if (!ioc_layout_mngr_load_dir(LAYOUT_ROOT_PATH))
    {
        LV_LOG_ERROR("Can't load layout root path %s", FRAGMENT_ROOT_PATH);
        return false;
    }
    if (access(LAYOUT_PATCH_PATH, F_OK) == 0)
    {
        if (!ioc_layout_mngr_load_dir(LAYOUT_PATCH_PATH))
        {
            LV_LOG_ERROR("Can't load layout patch path %s", FRAGMENT_PATCH_PATH);
            return false;
        }
    }
    else
    {
        LV_LOG_INFO("Skip layout patch path %s", FRAGMENT_PATCH_PATH);
    }

    // char * p = cJSON_Print(s_layouts_json);
    // printf("%s\n", p);
    // cJSON_free(p);

    return true;
}

void ioc_layout_mngr_unload(void)
{
    if (s_layouts_json != NULL)
    {
        cJSON_Delete(s_layouts_json);
        s_layouts_json = NULL;
    }
}

lv_obj_t * ioc_layout_mngr_load_layout(const char *layout_name, lv_obj_t * screen)
{
    if (s_layouts_json == NULL)
    {
        return NULL;
    }

    cJSON *layout_json;
    cJSON_ArrayForEach(layout_json, s_layouts_json)
    {
        const char *current_name = cJSON_GetStringValue(cJSON_GetObjectItem(layout_json, "name"));
        if (current_name != NULL && strcmp(current_name, layout_name) == 0)
        {
            // LV_LOG_ERROR("load layout %s", layout_name);
            return ioc_obj_create_obj(NULL, layout_json, screen);
        }
    }

    LV_LOG_ERROR("can't find %s", layout_name);
    return NULL;
}
