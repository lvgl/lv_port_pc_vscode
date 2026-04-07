#ifndef __PAGES_MNGR_H__
#define __PAGES_MNGR_H__

#include "ioc_app.h"

#include "./pages/ui_page_main.h"
#include "./pages/ui_page_connector_group.h"
#include "./pages/ui_page_status_bar.h"
#include "./pages/ui_page_mid_container_top.h"

#include "./pages/ui_page_01_home.h"
#include "./pages/ui_page_02_idle.h"
#include "./pages/ui_page_03_starting1.h"
#include "./pages/ui_page_04_starting2.h"
#include "./pages/ui_page_05_charging.h"
#include "./pages/ui_page_06_finished.h"
#include "./pages/ui_page_07_reserved.h"
#include "./pages/ui_page_08_session_error.h"
#include "./pages/ui_page_09_start_error.h"
#include "./pages/ui_page_10_fault.h"
#include "./pages/ui_page_11_unavailable.h"
#include "./pages/ui_page_12_connecting.h"
#include "./pages/ui_page_14_stopping.h"
#include "./pages/ui_page_15_wait_in_line.h"
#include "./pages/ui_page_16_suspend.h"
#include "./pages/ui_page_17_processing.h"

#include "./pages/ui_popup_help.h"
#include "./pages/ui_popup_language.h"
#include "./pages/ui_popup_charging_detail.h"
#include "./pages/ui_popup_bill.h"
#include "./pages/ui_popup_preauthorization.h"
#include "./pages/ui_popup_common.h"
#include "./pages/ui_price_idle.h"
#include "./pages/ui_price_charging.h"
#include "./pages/ui_price_finished.h"
#include "./pages/ui_price_finished_no_price.h"
#include "./pages/ui_price_reserved.h"
#include "./pages/ui_price_reserved_no_price.h"
#include "./pages/ui_customer_info.h"
#include "./pages/ui_customer_info_charging.h"


#include "./i18n/lv_i18n.h"

#define  UI_NUMOFCONNECTOR_SINGLE  1
#define  UI_CONNECTOR_ACTIVE_IDX_SINGLE  0

#endif
