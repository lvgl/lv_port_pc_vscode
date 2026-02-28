#include "feedback.h"

#include "lvgl/lvgl.h"
#include "feedback_logic.h"

#define FEEDBACK_MESSAGE_SIZE 96

static uint32_t feedback_count;
static lv_obj_t * feedback_status_label;

static void feedback_btn_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    char message[FEEDBACK_MESSAGE_SIZE];
    feedback_count++;
    feedback_format_message(message, sizeof(message), feedback_count);

    if(feedback_status_label != NULL) {
        lv_label_set_text(feedback_status_label, message);
    }
}

void feedback_add_button(lv_obj_t * parent)
{
    if(parent == NULL) {
        return;
    }

    feedback_count = 0;

    feedback_status_label = lv_label_create(parent);
    lv_label_set_text(feedback_status_label, "Tap button to send feedback");
    lv_obj_align(feedback_status_label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * button = lv_button_create(parent);
    lv_obj_set_size(button, 170, 44);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -14);
    lv_obj_add_event_cb(button, feedback_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * button_label = lv_label_create(button);
    lv_label_set_text(button_label, "Give Feedback");
    lv_obj_center(button_label);
}