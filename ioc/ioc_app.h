#ifndef __IOC_APP_H__
#define __IOC_APP_H__

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
// #include <fnmatch.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
// #include <sys/socket.h>
// #include <sys/ioctl.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <net/if.h>
// #include <sys/time.h>
#include <stdint.h>
#include <signal.h>
// #include <sys/reboot.h>


#include "lvgl.h"

#include "../Charger_PreDefine.h"

#define COMPILER_FOR COMPILE_FOR_DC_AM3354

#include "./mngr/cJSON.h"
#include "./mngr/ioc_cjson.h"
#include "./mngr/ioc_string_2_enum.h"
#include "./mngr/ioc_obj.h"
#include "./mngr/ioc_theme_mngr.h"
#include "./mngr/ioc_style_mngr.h"
#include "./mngr/ioc_fragment_mngr.h"
#include "./mngr/ioc_layout_mngr.h"

#include "./uijson/ui_json.h"

// #include "./mqtt/ioc_mqtt.h"

#include "./zlog/zlog.h"
#include "./zlog/Charger_ErrCode.h"

#include "../i18n/lv_i18n.h"

// #include "./core/screen_init.h"

void ioc_app_main(void);
void ioc_app_step(void);
void ioc_app_ui_json_updated_widgets(lv_obj_t* parent);
void s_preset_handler_init(void);
int Charger_zlog_init();

// 自组网
// void autoGroup_init(void);
// void autoGroup_step(void);

extern ioc_obj_handler_t s_preset_handler[];
extern lv_obj_t * ui_page_main_create(lv_obj_t *parent);
extern void language_page_init(void);
extern void ui_language_update_selected(const char *str);
extern int  ioc_get_preset_handler_count(void);
extern void ui_popup_hide_bill();
extern void ui_trigger_panel_hide();
extern void ui_trigger_panel_show(uint8_t color_idx);
extern void ui_popup_show_bill();
extern void ui_popup_common_handle(const char *str);
extern bool ioc_ui_json_set_language(int language);

#endif
