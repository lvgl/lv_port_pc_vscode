#include "../pages_mngr.h"

#define UI_NAME_PREAUTH_TOTAL_AMOUNT                ".pre_auth_val"
#define UI_NAME_PREAUTH_ENTER_AMOUNT                ".pre_auth_title"
#define UI_NAME_PREAUTH_CLOSE_TEXT                  ".pre_auth_close_button.btn_label"
#define UI_NAME_PREAUTH_CONFIM_TEXT                 ".pre_auth_confirm_button.btn_label"
#define UI_NAME_PREAUTH_CURRRENCY                   ".pre_amounut_currency"


#define UI_NAME_PREAUTH_CLOSE_BTN                   ".pre_auth_close_button"
#define UI_NAME_PREAUTH_CONFIM_BTN                  ".pre_auth_confirm_button"
#define UI_NAME_PREAUTH_NUM0                        ".num0"
#define UI_NAME_PREAUTH_NUM1                        ".num1"
#define UI_NAME_PREAUTH_NUM2                        ".num2"
#define UI_NAME_PREAUTH_NUM3                        ".num3"
#define UI_NAME_PREAUTH_NUM4                        ".num4"
#define UI_NAME_PREAUTH_NUM5                        ".num5"
#define UI_NAME_PREAUTH_NUM6                        ".num6"
#define UI_NAME_PREAUTH_NUM7                        ".num7"
#define UI_NAME_PREAUTH_NUM8                        ".num8"
#define UI_NAME_PREAUTH_NUM9                        ".num9"
#define UI_NAME_PREAUTH_NUM10                       ".num10"
#define UI_NAME_PREAUTH_NUM50                       ".num50"
#define UI_NAME_PREAUTH_NUM100                      ".num100"
#define UI_NAME_PREAUTH_CHAR_DROP                   ".char_drop"
#define UI_NAME_PREAUTH_CHAR_DELETE                 ".char_delete"

#define UI_NAME_PREAUTH_NUM0_TEXT                  ".num0.text"
#define UI_NAME_PREAUTH_NUM1_TEXT                  ".num1.text"
#define UI_NAME_PREAUTH_NUM2_TEXT                  ".num2.text"
#define UI_NAME_PREAUTH_NUM3_TEXT                  ".num3.text"
#define UI_NAME_PREAUTH_NUM4_TEXT                  ".num4.text"
#define UI_NAME_PREAUTH_NUM5_TEXT                  ".num5.text"
#define UI_NAME_PREAUTH_NUM6_TEXT                  ".num6.text"
#define UI_NAME_PREAUTH_NUM7_TEXT                  ".num7.text"
#define UI_NAME_PREAUTH_NUM8_TEXT                  ".num8.text"
#define UI_NAME_PREAUTH_NUM9_TEXT                  ".num9.text"
#define UI_NAME_PREAUTH_CHAR_DROP_TEXT             ".char_drop.text"

typedef enum {
    NUM0,
    NUM1,
    NUM2,
    NUM3,
    NUM4,
    NUM5,
    NUM6,
    NUM7,
    NUM8,
    NUM9,
    CHAR_DROP,
    CHAR_DELETE,

    NUM10,
    NUM50,
    NUM100,
    CLOSE_BTN,
    CONFIRM_BTN,

    NUMBER
}preauth_key_e;

typedef struct {

    lv_obj_t * obj;
    lv_obj_t * amount;

    lv_obj_t * enter_amount_label;
    lv_obj_t * close_text;
    lv_obj_t * confirm_text;
    lv_obj_t * currency;

    lv_obj_t * key[NUMBER];
    lv_obj_t * key_text[NUMBER];

}pre_authorization_t;

pre_authorization_t pre_authorization;

// 弹出预授权提示框, 用户输入完毕确认后直接发送预授权请求给ui_json, 调用界面不需要处理预授权金额
void ui_popup_show_preauthorization(int idx)
{
    ui_popup_show_mask(true);   // 显示遮罩层

    if ( pre_authorization.obj == NULL ) {
        pre_authorization.obj = ioc_layout_mngr_load_layout("pre_authorization", lv_layer_top());
    }

    if ( pre_authorization.obj != NULL ) {
        lv_obj_clear_flag(pre_authorization.obj, LV_OBJ_FLAG_HIDDEN);
    }

    ioc_label_set_text(pre_authorization.enter_amount_label, "Please_enter_the_amount");
    ioc_label_set_text(pre_authorization.close_text, "Close");
    ioc_label_set_text(pre_authorization.confirm_text, "Confirm");
    ioc_label_set_text(pre_authorization.currency, g_ui_json.currencyCode);
}


typedef struct {
    const char *suffix;
    lv_obj_t **obj;
} ui_preauth_obj_map_entry_t;

static ui_preauth_obj_map_entry_t obj_map[] = {
    { UI_NAME_PREAUTH_TOTAL_AMOUNT,  &pre_authorization.amount },
    { UI_NAME_PREAUTH_ENTER_AMOUNT,  &pre_authorization.enter_amount_label },
    { UI_NAME_PREAUTH_CLOSE_TEXT,    &pre_authorization.close_text },
    { UI_NAME_PREAUTH_CONFIM_TEXT,   &pre_authorization.confirm_text },
    { UI_NAME_PREAUTH_CURRRENCY,     &pre_authorization.currency },

    { UI_NAME_PREAUTH_CLOSE_BTN,     &pre_authorization.key[CLOSE_BTN] },
    { UI_NAME_PREAUTH_CONFIM_BTN,    &pre_authorization.key[CONFIRM_BTN] },
    { UI_NAME_PREAUTH_NUM0,          &pre_authorization.key[NUM0] },
    { UI_NAME_PREAUTH_NUM1,          &pre_authorization.key[NUM1] },
    { UI_NAME_PREAUTH_NUM2,          &pre_authorization.key[NUM2] },
    { UI_NAME_PREAUTH_NUM3,          &pre_authorization.key[NUM3] },
    { UI_NAME_PREAUTH_NUM4,          &pre_authorization.key[NUM4] },
    { UI_NAME_PREAUTH_NUM5,          &pre_authorization.key[NUM5] },
    { UI_NAME_PREAUTH_NUM6,          &pre_authorization.key[NUM6] },
    { UI_NAME_PREAUTH_NUM7,          &pre_authorization.key[NUM7] },
    { UI_NAME_PREAUTH_NUM8,          &pre_authorization.key[NUM8] },
    { UI_NAME_PREAUTH_NUM9,          &pre_authorization.key[NUM9] },
    { UI_NAME_PREAUTH_NUM10,         &pre_authorization.key[NUM10] },
    { UI_NAME_PREAUTH_NUM50,         &pre_authorization.key[NUM50] },
    { UI_NAME_PREAUTH_NUM100,        &pre_authorization.key[NUM100] },
    { UI_NAME_PREAUTH_CHAR_DROP,     &pre_authorization.key[CHAR_DROP] },
    { UI_NAME_PREAUTH_CHAR_DELETE,   &pre_authorization.key[CHAR_DELETE] },
    { UI_NAME_PREAUTH_NUM0_TEXT,     &pre_authorization.key_text[NUM0] },
    { UI_NAME_PREAUTH_NUM1_TEXT,     &pre_authorization.key_text[NUM1] },
    { UI_NAME_PREAUTH_NUM2_TEXT,     &pre_authorization.key_text[NUM2] },
    { UI_NAME_PREAUTH_NUM3_TEXT,     &pre_authorization.key_text[NUM3] },
    { UI_NAME_PREAUTH_NUM4_TEXT,     &pre_authorization.key_text[NUM4] },
    { UI_NAME_PREAUTH_NUM5_TEXT,     &pre_authorization.key_text[NUM5] },
    { UI_NAME_PREAUTH_NUM6_TEXT,     &pre_authorization.key_text[NUM6] },
    { UI_NAME_PREAUTH_NUM7_TEXT,     &pre_authorization.key_text[NUM7] },
    { UI_NAME_PREAUTH_NUM8_TEXT,     &pre_authorization.key_text[NUM8] },
    { UI_NAME_PREAUTH_NUM9_TEXT,     &pre_authorization.key_text[NUM9] },
    { UI_NAME_PREAUTH_CHAR_DROP_TEXT,   &pre_authorization.key_text[CHAR_DROP] },
};

typedef struct {
    const char *name;
    const char *text;
} keypad_text_map_t;

static const keypad_text_map_t keypad_map[] = {
    { UI_NAME_PREAUTH_NUM0_TEXT, "0" },
    { UI_NAME_PREAUTH_NUM1_TEXT, "1" },
    { UI_NAME_PREAUTH_NUM2_TEXT, "2" },
    { UI_NAME_PREAUTH_NUM3_TEXT, "3" },
    { UI_NAME_PREAUTH_NUM4_TEXT, "4" },
    { UI_NAME_PREAUTH_NUM5_TEXT, "5" },
    { UI_NAME_PREAUTH_NUM6_TEXT, "6" },
    { UI_NAME_PREAUTH_NUM7_TEXT, "7" },
    { UI_NAME_PREAUTH_NUM8_TEXT, "8" },
    { UI_NAME_PREAUTH_NUM9_TEXT, "9" },
    { UI_NAME_PREAUTH_CHAR_DROP_TEXT, "." },
};

static void preauth_keypad_event_cb(lv_event_t *e);
void ui_after_create_preauthorization(const char * name, lv_obj_t * obj)
{
    size_t prefix_len = strlen(UI_NAME_PRE_AUTHORIZATION);

    if (strncmp(name, UI_NAME_PRE_AUTHORIZATION, prefix_len) != 0)
    {
        LV_LOG_WARN("name[%s] does not start with prefix[%s], ignore.", name, UI_NAME_PRE_AUTHORIZATION);
        return;
    }

    const char * preauth_name = name + strlen(UI_NAME_PRE_AUTHORIZATION);

    LV_LOG_WARN("name[%s], preauth_name[%s]", name, preauth_name);

    for (size_t i = 0; i < sizeof(obj_map) / sizeof(obj_map[0]); ++i) {
        if (strcmp(preauth_name, obj_map[i].suffix) == 0) {
            *(obj_map[i].obj) = obj;

            if (
                strcmp(preauth_name, UI_NAME_PREAUTH_ENTER_AMOUNT) != 0 &&
                strcmp(preauth_name, UI_NAME_PREAUTH_CLOSE_TEXT) != 0 &&
                strcmp(preauth_name, UI_NAME_PREAUTH_CONFIM_TEXT) != 0 &&
                strcmp(preauth_name, UI_NAME_PREAUTH_TOTAL_AMOUNT) != 0 &&
                strcmp(preauth_name, UI_NAME_PREAUTH_CURRRENCY) != 0
            ) {
                lv_obj_add_event_cb(obj, preauth_keypad_event_cb, LV_EVENT_CLICKED, (void*)obj_map[i].suffix);
            }

            // 键盘数组用统一的fragment，不好在json里设置文本，所以在此处用
            for (size_t j = 0; j < sizeof(keypad_map) / sizeof(keypad_map[0]); j++)
            {
                if (strcmp(preauth_name, keypad_map[j].name) == 0)
                {
                    ioc_label_set_text(obj, keypad_map[j].text);
                    break;
                }
            }

            return;
        }
    }
}

static void preauth_keypad_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *amount_label = pre_authorization.amount;
    const char *current_text = lv_label_get_text(amount_label);
    char new_text[14];
    snprintf(new_text, sizeof(new_text), "%s", current_text);

    const char * suffix = (const char *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {

        // 处理不同按钮的点击事件
        if (strcmp(suffix, UI_NAME_PREAUTH_NUM10) == 0) {
            snprintf(new_text, sizeof(new_text), "%s", "10");
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM50) == 0) {
            snprintf(new_text, sizeof(new_text), "%s", "50");
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM100) == 0) {
            snprintf(new_text, sizeof(new_text), "%s", "100");
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM1) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "1");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "1");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM2) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "2");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "2");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM3) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "3");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "3");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM4) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "4");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "4");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM5) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "5");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "5");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM6) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "6");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "6");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM7) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "7");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "7");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM8) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "8");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "8");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM9) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "9");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "9");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_CHAR_DROP) == 0) {
            snprintf(new_text, sizeof(new_text), "%s%s", current_text, ".");
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_NUM0) == 0) {
            if (strcmp(current_text, "0") == 0) {
                snprintf(new_text, sizeof(new_text), "%s", "0");
            }
            else {
                snprintf(new_text, sizeof(new_text), "%s%s", current_text, "0");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_CHAR_DELETE) == 0) {
            if (strlen(new_text) > 1) {
                new_text[strlen(new_text) - 1] = '\0';
            } else {
                new_text[0] = '\0';
                snprintf(new_text, sizeof(new_text), "0");
            }
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_CLOSE_BTN) == 0) {
            lv_obj_add_flag(pre_authorization.obj, LV_OBJ_FLAG_HIDDEN);
            ui_popup_show_mask(false);
        }
        else if (strcmp(suffix, UI_NAME_PREAUTH_CONFIM_BTN) == 0) {
            // TODO UI 发送金额
            lv_obj_add_flag(pre_authorization.obj, LV_OBJ_FLAG_HIDDEN);
            ui_popup_show_mask(false);
            if ( g_ui_json.enSoftKey == true )
            {
                int amount = atoi(lv_label_get_text(pre_authorization.amount)) * 100;
                ioc_send_UseCreditCard(UI_CONNECTOR_ACTIVE_IDX, amount);
            }
        }
        ioc_label_set_text(amount_label, new_text);
    }
}
