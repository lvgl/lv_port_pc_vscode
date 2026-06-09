#include "ioc_app.h"

// 辅助函数：递归合并两个 cJSON 对象
void cJSON_MergeObject(cJSON *dst, const cJSON *src)
{
    cJSON *current = NULL;
    cJSON_ArrayForEach(current, src)
    {
        const char *name = current->string;
        // LV_LOG_WARN("Merging %s", name);

        cJSON *dst_item = cJSON_GetObjectItem(dst, name);
        if (cJSON_IsObject(current))
        {
            if (dst_item)
            {
                if (cJSON_IsObject(dst_item))
                {
                    cJSON_MergeObject(dst_item, current); // 递归合并
                }
                else
                {
                    cJSON_ReplaceItemInObject(dst, name, cJSON_Duplicate(current, true));
                }
            }
            else
            {
                cJSON_AddItemToObject(dst, name, cJSON_Duplicate(current, true));
            }
        }
        else if (cJSON_IsArray(current))
        {
            if (dst_item)
            {
                if (cJSON_IsArray(dst_item))
                {
                    cJSON * sub_item = NULL;
                    cJSON_ArrayForEach(sub_item, current)
                    {
                        cJSON_Merge(dst_item, sub_item);
                    }
                }
                else
                {
                    cJSON_ReplaceItemInObject(dst, name, cJSON_Duplicate(current, true));
                }
            }
            else
            {
                cJSON_AddItemToObject(dst, name, cJSON_Duplicate(current, true));
            }
        }
        else
        {
            if (dst_item)
            {
                if (!cJSON_Compare(dst_item, current, true))
                {
                    LV_LOG_WARN("replace: %s", name);
                    cJSON_ReplaceItemInObject(dst, name, cJSON_Duplicate(current, true)); // 添加新的字段
                }
            }
            else
            {
                LV_LOG_WARN("Add: %s", name);
                cJSON_AddItemToObject(dst, name, cJSON_Duplicate(current, true)); // 添加新字段
            }
        }
    }
}

// 主函数：合并 src 到 dst 数组
cJSON *cJSON_Merge(cJSON *const dst_array, const cJSON *const src)
{
    if (!dst_array || !src)
    {
        return NULL; // 如果任一参数为空，返回 NULL
    }

    const char *src_name = cJSON_GetStringValue(cJSON_GetObjectItem(src, "name"));
    if (!src_name)
    {
        return dst_array; // 如果 src 没有 name 属性，直接返回 dst
    }

    bool deleted = false;
    cJSON * src_deleted = cJSON_GetObjectItem(src, "deleted");
    if (src_deleted != NULL && cJSON_IsTrue(src_deleted))
    {
        LV_LOG_WARN("Deleted: %s", src_name);
        deleted = true;
    }

    int idx = 0;
    cJSON *current = NULL;
    cJSON_ArrayForEach(current, dst_array)
    {
        const char *current_name = cJSON_GetStringValue(cJSON_GetObjectItem(current, "name"));
        // LV_LOG_WARN("current_name: %s", current_name);
        if (current_name && strcmp(current_name, src_name) == 0)
        {
            if (deleted)
            {
                LV_LOG_WARN("Delete: %s", src_name);
                cJSON_DeleteItemFromArray(dst_array, idx);
                return dst_array;
            }
            // 找到相同 name 的对象，递归合并
            cJSON_MergeObject(current, src);
            return dst_array;
        }
        idx++;
    }

    // 如果没有找到相同 name 的对象，将 src 添加到 dst 数组中
    if (!deleted)
    {
            cJSON * idx = cJSON_GetObjectItem(src, "idx");
        if (idx != NULL)
        {
            // LV_LOG_WARN("Add: %s at %d", src_name, idx->valueint);
            cJSON_InsertItemInArray(dst_array, idx->valueint, cJSON_Duplicate(src, true));
        }
        else
        {
            // LV_LOG_WARN("Add: %s", src_name);
            cJSON_AddItemToArray(dst_array, cJSON_Duplicate(src, true));
        }
    }

    return dst_array;
}
