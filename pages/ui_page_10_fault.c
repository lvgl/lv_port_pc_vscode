#include "../pages_mngr.h"

#define UI_NAME_10_FAULT_EQUIPMENT_FAILURE           ".fault_title"
#define UI_NAME_10_FAULT_ERROR_CODE                  ".fault_code"
#define UI_NAME_10_FAULT_ERROR_PROMPT                ".fault_subtitle"


typedef struct {

    lv_obj_t * page;

    lv_obj_t * equipment_failure;
    lv_obj_t * error_code;
    lv_obj_t * error_prompt;

} ui_page_10_fault_t;

typedef enum
{
	NoError = 0,
	ConnectorLockFailure,
	EVCommunicationError,
	GroundFailure,
	HighTemperature,
	InternalError,
	LocalListConflict,
	OtherError,
	OverCurrentFailure,
	OverVoltage,
	PowerMeterFailure,
	PowerSwitchFailure,
	ReaderFailure,
	ResetFailure,
	UnderVoltage,
	WeakSignal,

	ChargePointErrorCode_Basic_NUM
} ChargePointErrorCodeBasic;
typedef enum
{
    InternalError_Emergency = ChargePointErrorCode_Basic_NUM,
    InternalError_PhaseSwap,

    InternalError_RCD_FAULT,
    InternalError_PE_LEAK,
    InternalError_PME_FAULT,
    InternalError_Power_Temp_Over,

    InternalError_CPStatusE,

    InternalError_OpenDoor,
	InternalError_PEWiring,
    InternalError_RCD_OPENED,       // 3354支持本地检测RCD是否脱扣
    InternalError_PowerDown,        // 功率板掉电
    InternalError_CoolFault,        // 液冷故障

    ChargePointErrorCode_NUM
} ChargePointErrorCode;

const char * c_ChargePointErrorCodeName[ChargePointErrorCode_NUM] =
{
	"NoError",
	"ConnectorLockFailure",
	"EVCommunicationError",
	"GroundFailure",
	"HighTemperature",
	"InternalError",
	"LocalListConflict",
	"OtherError",
	"OverCurrentFailure",
	"OverVoltage",
	"PowerMeterFailure",
	"PowerSwitchFailure",
	"ReaderFailure",
	"ResetFailure",
	"UnderVoltage",
	"WeakSignal",

    "EmergencyStop",
    "PhaseSwap",
    "RCD_6mA",          // RCD 6mA漏电
    "PE_LEAK",          // PE漏电
    "PME_FAULT",
    "PowerTempOver",
    "CP Status E",
    "Open Door",
    "PE Wiring",
    "MCCB Opened",
    "PowerDown",
    "CoolFault",
};

static ui_page_10_fault_t ui_page_10_fault = {NULL};


lv_obj_t * ui_page_create_10_fault(lv_obj_t *parent)
{
    return ioc_layout_mngr_load_layout("fault", parent);
}


void ui_fault_update_equipment_failure(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_10_fault.equipment_failure, "Equipment_failure");
}

void ui_after_create_10_fault_equipment_failure(const char * name, lv_obj_t * obj)
{
    ui_page_10_fault.equipment_failure = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_fault_update_equipment_failure;
}

void ui_fault_update_error_code(struct ioc_obj_user_data_s * user_data)
{
    char str[255];
    const char *err = UI_CONNECTOR_ERRCODE;
    if ( strcmp(err,  c_ChargePointErrorCodeName[ConnectorLockFailure]) == 0 ){
        snprintf(str, sizeof(str), "Lock Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[EVCommunicationError]) == 0 ){
        snprintf(str, sizeof(str), "EV Communication Error");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[GroundFailure]) == 0 ){
        snprintf(str, sizeof(str), "Ground Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[HighTemperature]) == 0 ){
        snprintf(str, sizeof(str), "High Temperature");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError]) == 0 ){
        snprintf(str, sizeof(str), "Internal Error");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[OverCurrentFailure]) == 0 ){
        snprintf(str, sizeof(str), "Over Current");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[OverVoltage]) == 0 ){
        snprintf(str, sizeof(str), "Over Voltage");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[PowerMeterFailure]) == 0 ){
        snprintf(str, sizeof(str), "Meter Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[PowerSwitchFailure]) == 0 ){
        snprintf(str, sizeof(str), "Power Switch Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[ReaderFailure]) == 0 ){
        snprintf(str, sizeof(str), "Reader Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[ResetFailure]) == 0 ){
        snprintf(str, sizeof(str), "Reset Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[UnderVoltage]) == 0 ){
        snprintf(str, sizeof(str), "Under Voltage");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[WeakSignal]) == 0 ){
        snprintf(str, sizeof(str), "Weak Signal");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError_Emergency]) == 0 ){
        snprintf(str, sizeof(str), "Emergency Pushed");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError_PhaseSwap]) == 0 ){
        snprintf(str, sizeof(str), "PhaseSwap Failure");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[OtherError]) == 0 ){
        snprintf(str, sizeof(str), "Other Error");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError_OpenDoor]) == 0 ){
        snprintf(str, sizeof(str), "Door Opened");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError_RCD_OPENED]) == 0 ){
        snprintf(str, sizeof(str), "MCCB Opened");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError_PowerDown]) == 0 ){
        snprintf(str, sizeof(str), "PowerDown");
    }
    else if ( strcmp(err,  c_ChargePointErrorCodeName[InternalError_CoolFault]) == 0 ){
        snprintf(str, sizeof(str), "CoolFault");
    }
    else {
        snprintf(str, sizeof(str), "Unknown Error");
    }

    ioc_label_set_text(ui_page_10_fault.error_code, str);
}

void ui_after_create_10_fault_error_code(const char * name, lv_obj_t * obj)
{
    ui_page_10_fault.error_code = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_fault_update_error_code;
}

void ui_fault_update_error_prompt(struct ioc_obj_user_data_s * user_data)
{
    ioc_label_set_text(ui_page_10_fault.error_prompt, "fault_prompt");
}

void ui_after_create_10_fault_error_prompt(const char * name, lv_obj_t * obj)
{
    ui_page_10_fault.error_prompt = obj;
    ioc_obj_user_data_t * user_data = (ioc_obj_user_data_t *)lv_obj_get_user_data(obj);
    user_data->ui_json_update_cb = ui_fault_update_error_prompt;
}

void ui_after_create_10_fault(const char * name, lv_obj_t * obj)
{
    const char * fault_name = name + strlen(UI_NAME_10_FAULT);
    if (strcmp(fault_name, UI_NAME_10_FAULT_EQUIPMENT_FAILURE) == 0)
    {
        ui_after_create_10_fault_equipment_failure(name, obj);
        return;
    }
    else if (strcmp(fault_name, UI_NAME_10_FAULT_ERROR_CODE) == 0)
    {
        ui_after_create_10_fault_error_code(name, obj);
        return;
    }
    else if (strcmp(fault_name, UI_NAME_10_FAULT_ERROR_PROMPT) == 0)
    {
        ui_after_create_10_fault_error_prompt(name, obj);
        return;
    }
}
