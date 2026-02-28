#include "feedback.h"

#include "lvgl/lvgl.h"

#include <assert.h>
#include <string.h>

static lv_obj_t * find_status_label(lv_obj_t * parent)
{
    uint32_t child_count = lv_obj_get_child_count(parent);
    for(uint32_t i = 0; i < child_count; i++) {
        lv_obj_t * child = lv_obj_get_child(parent, i);
        if(lv_obj_has_class(child, &lv_label_class)) {
            const char * text = lv_label_get_text(child);
            if(text != NULL && strcmp(text, "Tap button to send feedback") == 0) {
                return child;
            }
        }
    }

    return NULL;
}

static lv_obj_t * find_feedback_button(lv_obj_t * parent)
{
    uint32_t child_count = lv_obj_get_child_count(parent);
    for(uint32_t i = 0; i < child_count; i++) {
        lv_obj_t * child = lv_obj_get_child(parent, i);
        if(lv_obj_has_class(child, &lv_button_class)) {
            lv_obj_t * child_label = lv_obj_get_child(child, 0);
            if(child_label != NULL && lv_obj_has_class(child_label, &lv_label_class)) {
                const char * text = lv_label_get_text(child_label);
                if(text != NULL && strcmp(text, "Give Feedback") == 0) {
                    return child;
                }
            }
        }
    }

    return NULL;
}

int main(void)
{
    lv_init();

    lv_display_t * display = lv_display_create(320, 240);
    assert(display != NULL);
    lv_display_set_default(display);

    lv_obj_t * screen = lv_obj_create(NULL);
    assert(screen != NULL);
    feedback_add_button(screen);

    lv_obj_t * status_label = find_status_label(screen);
    assert(status_label != NULL);

    lv_obj_t * feedback_button = find_feedback_button(screen);
    assert(feedback_button != NULL);

    lv_obj_send_event(feedback_button, LV_EVENT_CLICKED, NULL);
    assert(strcmp(lv_label_get_text(status_label), "Thanks for your feedback! (1)") == 0);

    lv_obj_send_event(feedback_button, LV_EVENT_CLICKED, NULL);
    assert(strcmp(lv_label_get_text(status_label), "Thanks for your feedback! (2)") == 0);

    lv_deinit();
    return 0;
}