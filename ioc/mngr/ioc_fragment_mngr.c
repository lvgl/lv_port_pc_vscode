
#include "ioc_app.h"

static cJSON *s_fragments_json = NULL;

static bool ioc_fragment_mngr_load_dir(const char *fragment_path)
{
    DIR *dir = opendir(fragment_path);
    if (dir == NULL)
    {
        LV_LOG_ERROR("failed %s", fragment_path);
        return false;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s/%s", fragment_path, entry->d_name);
        cJSON *fragment_json = JSON_FromFile(file_path);
        if (fragment_json == NULL)
        {
            LV_LOG_ERROR("cant't open %s", file_path);
            closedir(dir);
            return false;
        }
        cJSON_Merge(s_fragments_json, fragment_json);
        cJSON_Delete(fragment_json);
    }
    closedir(dir);
    return true;
}

bool ioc_fragment_mngr_load(void)
{
    // 初始化一个空的片段包
    ioc_fragment_mngr_unload();
    s_fragments_json = cJSON_CreateArray();

    if (!ioc_fragment_mngr_load_dir(FRAGMENT_ROOT_PATH))
    {
        LV_LOG_ERROR("Can't load fragment root path %s", FRAGMENT_ROOT_PATH);
        return false;
    }
    if (access(FRAGMENT_PATCH_PATH, F_OK) == 0)
    {
        if (!ioc_fragment_mngr_load_dir(FRAGMENT_PATCH_PATH))
        {
            LV_LOG_ERROR("Can't load fragment patch path %s", FRAGMENT_PATCH_PATH);
            return false;
        }
    }
    else
    {
        LV_LOG_INFO("Skip fragment patch path %s", FRAGMENT_PATCH_PATH);
    }

    // char * p = cJSON_Print(s_fragments_json);
    // printf("%s\n", p);
    // cJSON_free(p);

    return true;
}

void ioc_fragment_mngr_unload(void)
{
    if (s_fragments_json != NULL)
    {
        cJSON_Delete(s_fragments_json);
        s_fragments_json = NULL;
    }
}


lv_obj_t * ioc_obj_create_fragment(const char * name, cJSON * fragment_json, lv_obj_t * parent)
{
    const char * fragment_name = cJSON_GetStringValue(cJSON_GetObjectItem(fragment_json, "fragment_name"));

    cJSON * curr_fragment_json;
    cJSON_ArrayForEach(curr_fragment_json, s_fragments_json)
    {
        const char * curr_fragment_name = cJSON_GetStringValue(cJSON_GetObjectItem(curr_fragment_json, "name"));
        if (curr_fragment_name != NULL && strcmp(curr_fragment_name, fragment_name) == 0)
        {
            lv_obj_t * fragment = ioc_obj_create_obj(name, curr_fragment_json, parent);

            cJSON *properties_json = cJSON_GetObjectItem(fragment_json, "properties");
            if (properties_json != NULL)
            {
                ioc_obj_set_properties_position(fragment, properties_json);
                ioc_obj_set_properties_size(fragment, properties_json);
                ioc_obj_set_properties_align(fragment, properties_json);
                ioc_obj_set_properties_style(fragment, properties_json);
            }

            return fragment;
        }
    }

    LV_LOG_ERROR("Can't find fragment %s", fragment_name);
    return NULL;
}
