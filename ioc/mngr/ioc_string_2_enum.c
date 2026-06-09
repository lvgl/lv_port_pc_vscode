#include "ioc_app.h"

lv_align_t ioc_string_2_align(const char *align_name)
{
    if (strcmp(align_name, "LV_ALIGN_TOP_LEFT") == 0)
        return LV_ALIGN_TOP_LEFT;
    if (strcmp(align_name, "LV_ALIGN_TOP_MID") == 0)
        return LV_ALIGN_TOP_MID;
    if (strcmp(align_name, "LV_ALIGN_TOP_RIGHT") == 0)
        return LV_ALIGN_TOP_RIGHT;
    if (strcmp(align_name, "LV_ALIGN_BOTTOM_LEFT") == 0)
        return LV_ALIGN_BOTTOM_LEFT;
    if (strcmp(align_name, "LV_ALIGN_BOTTOM_MID") == 0)
        return LV_ALIGN_BOTTOM_MID;
    if (strcmp(align_name, "LV_ALIGN_BOTTOM_RIGHT") == 0)
        return LV_ALIGN_BOTTOM_RIGHT;
    if (strcmp(align_name, "LV_ALIGN_LEFT_MID") == 0)
        return LV_ALIGN_LEFT_MID;
    if (strcmp(align_name, "LV_ALIGN_RIGHT_MID") == 0)
        return LV_ALIGN_RIGHT_MID;
    if (strcmp(align_name, "LV_ALIGN_CENTER") == 0)
        return LV_ALIGN_CENTER;
    if (strcmp(align_name, "LV_ALIGN_OUT_TOP_LEFT") == 0)
        return LV_ALIGN_OUT_TOP_LEFT;
    if (strcmp(align_name, "LV_ALIGN_OUT_TOP_MID") == 0)
        return LV_ALIGN_OUT_TOP_MID;
    if (strcmp(align_name, "LV_ALIGN_OUT_TOP_RIGHT") == 0)
        return LV_ALIGN_OUT_TOP_RIGHT;
    if (strcmp(align_name, "LV_ALIGN_OUT_BOTTOM_LEFT") == 0)
        return LV_ALIGN_OUT_BOTTOM_LEFT;
    if (strcmp(align_name, "LV_ALIGN_OUT_BOTTOM_MID") == 0)
        return LV_ALIGN_OUT_BOTTOM_MID;
    if (strcmp(align_name, "LV_ALIGN_OUT_BOTTOM_RIGHT") == 0)
        return LV_ALIGN_OUT_BOTTOM_RIGHT;
    if (strcmp(align_name, "LV_ALIGN_OUT_LEFT_TOP") == 0)
        return LV_ALIGN_OUT_LEFT_TOP;
    if (strcmp(align_name, "LV_ALIGN_OUT_LEFT_MID") == 0)
        return LV_ALIGN_OUT_LEFT_MID;
    if (strcmp(align_name, "LV_ALIGN_OUT_LEFT_BOTTOM") == 0)
        return LV_ALIGN_OUT_LEFT_BOTTOM;
    if (strcmp(align_name, "LV_ALIGN_OUT_RIGHT_TOP") == 0)
        return LV_ALIGN_OUT_RIGHT_TOP;
    if (strcmp(align_name, "LV_ALIGN_OUT_RIGHT_MID") == 0)
        return LV_ALIGN_OUT_RIGHT_MID;
    if (strcmp(align_name, "LV_ALIGN_OUT_RIGHT_BOTTOM") == 0)
        return LV_ALIGN_OUT_RIGHT_BOTTOM;

    return LV_ALIGN_DEFAULT;
}


lv_text_align_t ioc_string_2_text_align(const char *align_name)
{
    if (strcmp(align_name, "LV_TEXT_ALIGN_AUTO") == 0)
        return LV_TEXT_ALIGN_AUTO;
    if (strcmp(align_name, "LV_TEXT_ALIGN_LEFT") == 0)
        return LV_TEXT_ALIGN_LEFT;
    if (strcmp(align_name, "LV_TEXT_ALIGN_RIGHT") == 0)
        return LV_TEXT_ALIGN_RIGHT;

    return LV_TEXT_ALIGN_CENTER;
}

lv_flex_flow_t ioc_string_2_flex_flow(const char *flow_name)
{
    if (flow_name != NULL)
    {
        if (strcmp(flow_name, "LV_FLEX_FLOW_ROW") == 0)
        {
            return LV_FLEX_FLOW_ROW;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_COLUMN") == 0)
        {
            return LV_FLEX_FLOW_COLUMN;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_ROW_WRAP") == 0)
        {
            return LV_FLEX_FLOW_ROW_WRAP;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_ROW_REVERSE") == 0)
        {
            return LV_FLEX_FLOW_ROW_REVERSE;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_ROW_WRAP_REVERSE") == 0)
        {
            return LV_FLEX_FLOW_ROW_WRAP_REVERSE;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_COLUMN_WRAP") == 0)
        {
            return LV_FLEX_FLOW_COLUMN_WRAP;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_COLUMN_REVERSE") == 0)
        {
            return LV_FLEX_FLOW_COLUMN_REVERSE;
        }
        else if (strcmp(flow_name, "LV_FLEX_FLOW_COLUMN_WRAP_REVERSE") == 0)
        {
            return LV_FLEX_FLOW_COLUMN_WRAP_REVERSE;
        }
    }

    return LV_FLEX_FLOW_ROW;
}


lv_flex_align_t ioc_string_2_flex_align(const char *align_name)
{
    if (align_name != NULL)
    {
        if (strcmp(align_name, "LV_FLEX_ALIGN_START") == 0)
        {
            return LV_FLEX_ALIGN_START;
        }
        else if (strcmp(align_name, "LV_FLEX_ALIGN_CENTER") == 0)
        {
            return LV_FLEX_ALIGN_CENTER;
        }
        else if (strcmp(align_name, "LV_FLEX_ALIGN_END") == 0)
        {
            return LV_FLEX_ALIGN_END;
        }
        else if (strcmp(align_name, "LV_FLEX_ALIGN_SPACE_AROUND") == 0)
        {
            return LV_FLEX_ALIGN_SPACE_AROUND;
        }
        else if (strcmp(align_name, "LV_FLEX_ALIGN_SPACE_BETWEEN") == 0)
        {
            return LV_FLEX_ALIGN_SPACE_BETWEEN;
        }
        else if (strcmp(align_name, "LV_FLEX_ALIGN_SPACE_EVENLY") == 0)
        {
            return LV_FLEX_ALIGN_SPACE_EVENLY;
        }
    }

    return LV_FLEX_ALIGN_START;
}
