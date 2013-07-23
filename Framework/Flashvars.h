/**
 * Exports
 */
extern const char template_main_fname[];
extern const uint8_t template_main_fnsize;
extern const char template_main_controllerrow_fname[];
extern const uint8_t template_main_controllerrow_fnsize;
extern const char template_main_sensorrow_fname[];
extern const uint8_t template_main_sensorrow_fnsize;
extern const char template_main_actuatorrow_fname[];
extern const uint8_t template_main_actuatorrow_fnsize;
extern const char* const template_main[];
extern const uint8_t template_main_elements;
extern const char* const template_main_sensorrow[];
extern const uint8_t template_main_sensorrow_elements;
extern const char* const template_main_controllerrow[];
extern const uint8_t template_main_controllerrow_elements;
extern const char* const template_main_actuatorrow[];
extern const uint8_t template_main_actuatorrow_elements;

extern const char template_config_fname[];
extern const uint16_t template_config_fnsize;
extern const char* const template_config[];
extern const uint16_t template_config_elements;
extern const char* const template_config_inputs[];
extern const uint16_t template_config_inputs_elements;

extern const char template_digitaloutput_fname[];
extern const uint16_t template_digitaloutput_fnsize;
extern const char* const template_digitaloutput[];
extern const uint16_t template_digitaloutput_elements;
extern const char* const template_digitaloutput_inputs[];
extern const uint16_t template_digitaloutput_inputs_elements;

extern const char template_digitalinput_fname[];
extern const uint16_t template_digitalinput_fnsize;
extern const char* const template_digitalinput[];
extern const uint16_t template_digitalinput_elements;
extern const char* const template_digitalinput_inputs[];
extern const uint16_t template_digitalinput_inputs_elements;

extern const char template_ds18s20_fname[];
extern const uint16_t template_ds18s20_fnsize;
extern const char* const template_ds18s20[];
extern const uint16_t template_ds18s20_elements;
extern const char* const template_ds18s20_inputs[];
extern const uint16_t template_ds18s20_inputs_elements;

extern const char template_clocktimercontroller_fname[];
extern const uint16_t template_clocktimercontroller_fnsize;
extern const char* const template_clocktimercontroller[];
extern const uint16_t template_clocktimercontroller_elements;
extern const char template_clocktimercontroller_row_fname[];
extern const uint16_t template_clocktimercontroller_row_fnsize;
extern const char* const template_clocktimercontroller_row[];
extern const uint16_t template_clocktimercontroller_row_elements;
extern const char* const template_clocktimercontroller_inputs[];
extern const uint16_t template_clocktimercontroller_inputs_elements;

extern const char template_levelcontroller_fname[];
extern const uint16_t template_levelcontroller_fnsize;
extern const char* const template_levelcontroller[];
extern const uint16_t template_levelcontroller_elements;
extern const char* const template_levelcontroller_states[];
extern const uint16_t template_levelcontroller_states_elements;
extern const char* const template_levelcontroller_inputs[];
extern const uint16_t template_levelcontroller_inputs_elements;

extern const char template_temperaturecontroller_fname[];
extern const uint16_t template_temperaturecontroller_fnsize;
extern const char* const template_temperaturecontroller[];
extern const uint16_t template_temperaturecontroller_elements;
extern const char* const template_temperaturecontroller_inputs[];
extern const uint16_t template_temperaturecontroller_inputs_elements;

/**
 * Todo: remove the following exports
 */

extern const char pgm_input_ipin[];
extern const char pgm_input_ipwm[];
extern const char pgm_url_select[];

/*
 * Strings
 * -----------------------------------------------------------------------------
 */

#ifdef _FLASHVAR_IMPLEMENTATION_
/**
 * Template Replacement Strings
 */
static const char pgm_hour[] PROGMEM = "##HOUR##";
static const char pgm_minute[] PROGMEM = "##MINUTE##";
static const char pgm_second[] PROGMEM = "##SECOND##";
static const char pgm_dow[] PROGMEM = "##DOW##";
static const char pgm_month[] PROGMEM = "##MONTH##";
static const char pgm_day[] PROGMEM = "##DAY##";
static const char pgm_year[] PROGMEM = "##YEAR##";
static const char pgm_iname[] PROGMEM = "##INAME##";
static const char pgm_link[] PROGMEM = "##LINK##";
static const char pgm_name[] PROGMEM = "##NAME##";
static const char pgm_value[] PROGMEM = "##VALUE##";
static const char pgm_color[] PROGMEM = "##COLOR##";
static const char pgm_controllerrow[] PROGMEM = "##CONTROLLERROW##";
static const char pgm_actuatorrow[] PROGMEM = "##ACTUATORROW##";

static const char pgm_ixivelychannel[] PROGMEM = "##IXIVELYCHANNEL##";
static const char pgm_xivelychannel[] PROGMEM = "##XIVELYCHANNEL##";
static const char pgm_sensor[] PROGMEM = "##SENSORROW##";
static const char pgm_cselect[] PROGMEM = "##CSELECT##";
static const char pgm_coptions[] PROGMEM = "##COPTIONS##";
static const char pgm_lselect[] PROGMEM = "##LSELECT##";
static const char pgm_loptions[] PROGMEM = "##LOPTIONS##";
static const char pgm_sselect[] PROGMEM = "##SSELECT##";
static const char pgm_soptions[] PROGMEM = "##SOPTIONS##";
static const char pgm_mac1[] PROGMEM = "##MAC1##";
static const char pgm_mac2[] PROGMEM = "##MAC2##";
static const char pgm_mac3[] PROGMEM = "##MAC3##";
static const char pgm_mac4[] PROGMEM = "##MAC4##";
static const char pgm_mac5[] PROGMEM = "##MAC5##";
static const char pgm_mac6[] PROGMEM = "##MAC6##";
static const char pgm_dhcp_select[] PROGMEM = "##DHCPSELECTOPTION##";
static const char pgm_ip1[] PROGMEM = "##IP1##";
static const char pgm_ip2[] PROGMEM = "##IP2##";
static const char pgm_ip3[] PROGMEM = "##IP3##";
static const char pgm_ip4[] PROGMEM = "##IP4##";
static const char pgm_nm1[] PROGMEM = "##NETMASK1##";
static const char pgm_nm2[] PROGMEM = "##NETMASK2##";
static const char pgm_nm3[] PROGMEM = "##NETMASK3##";
static const char pgm_nm4[] PROGMEM = "##NETMASK4##";
static const char pgm_gw1[] PROGMEM = "##GATEWAY1##";
static const char pgm_gw2[] PROGMEM = "##GATEWAY2##";
static const char pgm_gw3[] PROGMEM = "##GATEWAY3##";
static const char pgm_gw4[] PROGMEM = "##GATEWAY4##";
static const char pgm_dns1[] PROGMEM = "##DNS1##";
static const char pgm_dns2[] PROGMEM = "##DNS2##";
static const char pgm_dns3[] PROGMEM = "##DNS3##";
static const char pgm_dns4[] PROGMEM = "##DNS4##";
static const char pgm_ntp_select[] PROGMEM = "##NTPSELECTOPTION##";
static const char pgm_ntp1[] PROGMEM = "##NTP1##";
static const char pgm_ntp2[] PROGMEM = "##NTP2##";
static const char pgm_ntp3[] PROGMEM = "##NTP3##";
static const char pgm_ntp4[] PROGMEM = "##NTP4##";
static const char pgm_ntp_period[] PROGMEM = "##NTPPERIOD##";
static const char pgm_timezone[] PROGMEM = "##TIMEZONE##";
static const char pgm_xively_select[] PROGMEM = "##XIVELYSELECTOPTION##";
static const char pgm_xively_api[] PROGMEM = "##XIVELYKEY##";
static const char pgm_xively_feed[] PROGMEM = "##XIVELYFEED##";
static const char pgm_free_ram[] PROGMEM = "##FREERAM##";
static const char pgm_pin[] PROGMEM = "##PIN##";
static const char pgm_ipwm[] PROGMEM = "##IPWM##";
static const char pgm_pwm[] PROGMEM = "##PWM##";
static const char pgm_aname[] PROGMEM = "##ACTUATORNAME##";
static const char pgm_typeoptions[] PROGMEM = "##TYPEOPTIONS##";
static const char pgm_addressselect[] PROGMEM = "##ADDRESSSELECT##";
static const char pgm_url[] PROGMEM = "##URL##";
static const char pgm_clocktimerselect[] PROGMEM = "##CLOCKTIMERSELECT##";
static const char pgm_actuatorselect[] PROGMEM = "##ACTUATORSELECT##";
static const char pgm_clocktimerrow[] PROGMEM = "##CLOCKTIMERROW##";
static const char pgm_checked_mo[] PROGMEM = "##CHECKEDMO##";
static const char pgm_checked_tu[] PROGMEM = "##CHECKEDTU##";
static const char pgm_checked_we[] PROGMEM = "##CHECKEDWE##";
static const char pgm_checked_th[] PROGMEM = "##CHECKEDTH##";
static const char pgm_checked_fr[] PROGMEM = "##CHECKEDFR##";
static const char pgm_checked_sa[] PROGMEM = "##CHECKEDSA##";
static const char pgm_checked_su[] PROGMEM = "##CHECKEDSU##";
static const char pgm_ihon[] PROGMEM = "##I_HON##";
static const char pgm_imon[] PROGMEM = "##I_MON##";
static const char pgm_ihoff[] PROGMEM = "##I_HOFF##";
static const char pgm_imoff[] PROGMEM = "##I_MOFF##";
static const char pgm_hon[] PROGMEM = "##HON##";
static const char pgm_mon[] PROGMEM = "##MON##";
static const char pgm_hoff[] PROGMEM = "##HOFF##";
static const char pgm_moff[] PROGMEM = "##MOFF##";
static const char pgm_state[] PROGMEM = "##STATE##";
static const char pgm_delaylow[] PROGMEM = "##DELAYLOW##";
static const char pgm_delayhigh[] PROGMEM = "##DELAYHIGH##";
static const char pgm_refilltimeout[] PROGMEM = "##REFILLTIMEOUT##";
static const char pgm_ok[] PROGMEM = "OK";
static const char pgm_debounce[] PROGMEM = "DEBOUNCE";
static const char pgm_refill[] PROGMEM = "REFILL";
static const char pgm_overrun[] PROGMEM = "OVERRUN";
static const char pgm_timeout[] PROGMEM = "TIMEOUT";
static const char pgm_temperature[] PROGMEM = "##TEMPERATURE##";
static const char pgm_threshold[] PROGMEM = "##THRESHOLD##";
static const char pgm_pwmmax[] PROGMEM = "##PWMMAX##";
static const char pgm_hysteresis[] PROGMEM = "##HYSTERESIS##";

/**
 * Template POST Strings
 */
static const char pgm_input_mac1[] PROGMEM = "mac1";
static const char pgm_input_mac2[] PROGMEM = "mac2";
static const char pgm_input_mac3[] PROGMEM = "mac3";
static const char pgm_input_mac4[] PROGMEM = "mac4";
static const char pgm_input_mac5[] PROGMEM = "mac5";
static const char pgm_input_mac6[] PROGMEM = "mac6";
static const char pgm_input_dhcp[] PROGMEM = "dhcp";
static const char pgm_input_ip1[] PROGMEM = "ip1";
static const char pgm_input_ip2[] PROGMEM = "ip2";
static const char pgm_input_ip3[] PROGMEM = "ip3";
static const char pgm_input_ip4[] PROGMEM = "ip4";
static const char pgm_input_netmask1[] PROGMEM = "netmask1";
static const char pgm_input_netmask2[] PROGMEM = "netmask2";
static const char pgm_input_netmask3[] PROGMEM = "netmask3";
static const char pgm_input_netmask4[] PROGMEM = "netmask4";
static const char pgm_input_gateway1[] PROGMEM = "gateway1";
static const char pgm_input_gateway2[] PROGMEM = "gateway2";
static const char pgm_input_gateway3[] PROGMEM = "gateway3";
static const char pgm_input_gateway4[] PROGMEM = "gateway4";
static const char pgm_input_dns1[] PROGMEM = "dns1";
static const char pgm_input_dns2[] PROGMEM = "dns2";
static const char pgm_input_dns3[] PROGMEM = "dns3";
static const char pgm_input_dns4[] PROGMEM = "dns4";
static const char pgm_input_ntp[] PROGMEM = "ntp";
static const char pgm_input_ntp1[] PROGMEM = "ntp1";
static const char pgm_input_ntp2[] PROGMEM = "ntp2";
static const char pgm_input_ntp3[] PROGMEM = "ntp3";
static const char pgm_input_ntp4[] PROGMEM = "ntp4";
static const char pgm_input_ntp_period[] PROGMEM = "ntpperiod";
static const char pgm_input_timezone[] PROGMEM = "timezone";
static const char pgm_input_hour[] PROGMEM = "hour";
static const char pgm_input_minute[] PROGMEM = "minute";
static const char pgm_input_second[] PROGMEM = "second";
static const char pgm_input_month[] PROGMEM = "month";
static const char pgm_input_day[] PROGMEM = "day";
static const char pgm_input_year[] PROGMEM = "year";
static const char pgm_input_xively[] PROGMEM = "xively";
static const char pgm_input_xively_api_key[] PROGMEM = "xivelykey";
static const char pgm_input_xively_feed[] PROGMEM = "xivelyfeed";
static const char pgm_input_type[] PROGMEM = "type";
static const char pgm_input_address[] PROGMEM = "address";
static const char pgm_input_timer[] PROGMEM = "timer";
static const char pgm_input_actuator[] PROGMEM = "actuator";
static const char pgm_input_dow[] PROGMEM = "dow";
static const char pgm_input_delaylow[] PROGMEM = "delayLow";
static const char pgm_input_delayhigh[] PROGMEM = "delayHigh";
static const char pgm_input_refilltimeout[] PROGMEM = "refillTimeout";
static const char pgm_input_sensor[] PROGMEM = "sensor";
static const char pgm_input_threshold[] PROGMEM = "Threshold";
static const char pgm_inputpwmmax[] PROGMEM = "PWMMax";
static const char pgm_input_hysteresis[] PROGMEM = "Hysteresis";

extern const char pgm_input_ipin[] PROGMEM = "ipin";
extern const char pgm_input_ipwm[] PROGMEM = "ipwm";
extern const char pgm_url_select[] PROGMEM = "select";
#endif

/**
 * Main Webpage
 * ----------------------------------------------------------------------------
 */
enum TEMPLATE_MAIN
{
    MAIN_HOUR,
    MAIN_MINUTE,
    MAIN_SECOND,
    MAIN_DOW,
    MAIN_MONTH,
    MAIN_DAY,
    MAIN_YEAR,
    MAIN_CONTROLLER,
    MAIN_SENSOR,
    MAIN_ACTUATOR
};

#ifdef _FLASHVAR_IMPLEMENTATION_

extern const char* const template_main[] PROGMEM = {
    pgm_hour,
    pgm_minute,
    pgm_second,
    pgm_dow,
    pgm_month,
    pgm_day,
    pgm_year,
    pgm_controllerrow,
    pgm_sensor,
    pgm_actuatorrow };

extern const uint8_t template_main_elements = sizeof(template_main)
        / sizeof(char*);
extern const char template_main_fname[] PROGMEM = "main.htm";
extern const uint8_t template_main_fnsize = sizeof(template_main_fname);

#endif

/**
 * Main Webpage
 * Controller Row
 * ----------------------------------------------------------------------------
 */

enum TEMPLATE_MAIN_CONTROLLERROW
{
    CONTROLLERROW_COLOR,
    CONTROLLERROW_INAME,
    CONTROLLERROW_NAME,
    CONTROLLERROW_LINK
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_main_controllerrow[] PROGMEM = {
    pgm_color,
    pgm_iname,
    pgm_name,
    pgm_link };

extern const uint8_t template_main_controllerrow_elements =
        sizeof(template_main_controllerrow) / sizeof(char*);
extern const char template_main_controllerrow_fname[] PROGMEM = "maincrow.htm";
extern const uint8_t template_main_controllerrow_fnsize =
        sizeof(template_main_controllerrow_fname);

#endif

/**
 * Main Webpage
 * Sensor Row
 * ----------------------------------------------------------------------------
 */
enum TEMPLATE_MAIN_SENSORROW
{
    SENSORROW_COLOR,
    SENSORROW_INAME,
    SENSORROW_NAME,
    SENSORROW_VALUE,
    SENSORROW_IXIVELYCHANNEL,
    SENSORROW_XIVELYCHANNEL,
    SENSORROW_LINK
};

#ifdef _FLASHVAR_IMPLEMENTATION_

extern const char* const template_main_sensorrow[] PROGMEM = {
    pgm_color,
    pgm_iname,
    pgm_name,
    pgm_value,
    pgm_ixivelychannel,
    pgm_xivelychannel,
    pgm_link };

extern const uint8_t template_main_sensorrow_elements =
        sizeof(template_main_sensorrow) / sizeof(char*);

extern const char template_main_sensorrow_fname[] PROGMEM = "mainsrow.htm";

extern const uint8_t template_main_sensorrow_fnsize =
        sizeof(template_main_sensorrow_fname);
#endif

/**
 * Main Webpage
 * Actuator Row
 * ----------------------------------------------------------------------------
 */
enum TEMPLATE_MAIN_ACTUATORROW
{
    ACTUATORROW_COLOR,
    ACTUATORROW_IACTUATOR,
    ACTUATORROW_ACTUATORNAME,
    ACTUATORROW_CSELECT,
    ACTUATORROW_COPTIONS,
    ACTUATORROW_LSELECT,
    ACTUATORROW_LOPTIONS,
    ACTUATORROW_SSELECT,
    ACTUATORROW_SOPTIONS,
    ACTUATORROW_LINK
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_main_actuatorrow[] PROGMEM = {
    pgm_color,
    pgm_iname,
    pgm_name,
    pgm_cselect,
    pgm_coptions,
    pgm_lselect,
    pgm_loptions,
    pgm_sselect,
    pgm_soptions,
    pgm_link };

extern const uint8_t template_main_actuatorrow_elements =
        sizeof(template_main_actuatorrow) / sizeof(char*);
extern const char template_main_actuatorrow_fname[] PROGMEM = "mainarow.htm";
extern const uint8_t template_main_actuatorrow_fnsize =
        sizeof(template_main_actuatorrow_fname);

#endif

/*
 * Configuration Webpage
 * ----------------------------------------------------------------------------
 */
enum TEMPLATE_CONFIG
{
    CONFIG_T_MAC1,
    CONFIG_T_MAC2,
    CONFIG_T_MAC3,
    CONFIG_T_MAC4,
    CONFIG_T_MAC5,
    CONFIG_T_MAC6,
    CONFIG_T_DHCPSELECTOPTION,
    CONFIG_T_IP1,
    CONFIG_T_IP2,
    CONFIG_T_IP3,
    CONFIG_T_IP4,
    CONFIG_T_NETMASK1,
    CONFIG_T_NETMASK2,
    CONFIG_T_NETMASK3,
    CONFIG_T_NETMASK4,
    CONFIG_T_GATEWAY1,
    CONFIG_T_GATEWAY2,
    CONFIG_T_GATEWAY3,
    CONFIG_T_GATEWAY4,
    CONFIG_T_DNS1,
    CONFIG_T_DNS2,
    CONFIG_T_DNS3,
    CONFIG_T_DNS4,
    CONFIG_T_NTPSELECTOPTION,
    CONFIG_T_NTP1,
    CONFIG_T_NTP2,
    CONFIG_T_NTP3,
    CONFIG_T_NTP4,
    CONFIG_T_NTPPERIOD,
    CONFIG_T_TIMEZONE,
    CONFIG_T_HOUR,
    CONFIG_T_MINUTE,
    CONFIG_T_SECOND,
    CONFIG_T_MONTH,
    CONFIG_T_DAY,
    CONFIG_T_YEAR,
    CONFIG_T_XIVELY,
    CONFIG_T_XIVELYAPIKEY,
    CONFIG_T_XIVELYFEED,
    CONFIG_T_FREERAM
};

#ifdef _FLASHVAR_IMPLEMENTATION_

extern const char* const template_config[] PROGMEM
= {
    pgm_mac1,
    pgm_mac2,
    pgm_mac3,
    pgm_mac4,
    pgm_mac5,
    pgm_mac6,
    pgm_dhcp_select,
    pgm_ip1,
    pgm_ip2,
    pgm_ip3,
    pgm_ip4,
    pgm_nm1,
    pgm_nm2,
    pgm_nm3,
    pgm_nm4,
    pgm_gw1,
    pgm_gw2,
    pgm_gw3,
    pgm_gw4,
    pgm_dns1,
    pgm_dns2,
    pgm_dns3,
    pgm_dns4,
    pgm_ntp_select,
    pgm_ntp1,
    pgm_ntp2,
    pgm_ntp3,
    pgm_ntp4,
    pgm_ntp_period,
    pgm_timezone,
    pgm_hour,
    pgm_minute,
    pgm_second,
    pgm_month,
    pgm_day,
    pgm_year,
    pgm_xively_select,
    pgm_xively_api,
    pgm_xively_feed,
    pgm_free_ram };

extern const uint16_t template_config_elements = sizeof(template_config)
        / sizeof(char*);
extern const char template_config_fname[] PROGMEM = "conf.htm";
extern const uint16_t template_config_fnsize = sizeof(template_config_fname);

#endif

enum TEMPLATE_CONFIG_INPUTS
{
    CONFIG_I_MAC1,
    CONFIG_I_MAC2,
    CONFIG_I_MAC3,
    CONFIG_I_MAC4,
    CONFIG_I_MAC5,
    CONFIG_I_MAC6,
    CONFIG_I_DHCP,
    CONFIG_I_IP1,
    CONFIG_I_IP2,
    CONFIG_I_IP3,
    CONFIG_I_IP4,
    CONFIG_I_NETMASK1,
    CONFIG_I_NETMASK2,
    CONFIG_I_NETMASK3,
    CONFIG_I_NETMASK4,
    CONFIG_I_GATEWAY1,
    CONFIG_I_GATEWAY2,
    CONFIG_I_GATEWAY3,
    CONFIG_I_GATEWAY4,
    CONFIG_I_DNS1,
    CONFIG_I_DNS2,
    CONFIG_I_DNS3,
    CONFIG_I_DNS4,
    CONFIG_I_NTP,
    CONFIG_I_NTP1,
    CONFIG_I_NTP2,
    CONFIG_I_NTP3,
    CONFIG_I_NTP4,
    CONFIG_I_NTPPERIOD,
    CONFIG_I_TIMEZONE,
    CONFIG_I_HOUR,
    CONFIG_I_MINUTE,
    CONFIG_I_SECOND,
    CONFIG_I_MONTH,
    CONFIG_I_DAY,
    CONFIG_I_YEAR,
    CONFIG_I_XIVELY,
    CONFIG_I_XIVELYAPIKEY,
    CONFIG_I_XIVELYFEED

};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_config_inputs[] PROGMEM = {
    pgm_input_mac1,
    pgm_input_mac2,
    pgm_input_mac3,
    pgm_input_mac4,
    pgm_input_mac5,
    pgm_input_mac6,
    pgm_input_dhcp,
    pgm_input_ip1,
    pgm_input_ip2,
    pgm_input_ip3,
    pgm_input_ip4,
    pgm_input_netmask1,
    pgm_input_netmask2,
    pgm_input_netmask3,
    pgm_input_netmask4,
    pgm_input_gateway1,
    pgm_input_gateway2,
    pgm_input_gateway3,
    pgm_input_gateway4,
    pgm_input_dns1,
    pgm_input_dns2,
    pgm_input_dns3,
    pgm_input_dns4,
    pgm_input_ntp,
    pgm_input_ntp1,
    pgm_input_ntp2,
    pgm_input_ntp3,
    pgm_input_ntp4,
    pgm_input_ntp_period,
    pgm_input_timezone,
    pgm_input_hour,
    pgm_input_minute,
    pgm_input_second,
    pgm_input_month,
    pgm_input_day,
    pgm_input_year,
    pgm_input_xively,
    pgm_input_xively_api_key,
    pgm_input_xively_feed };

extern const uint16_t template_config_inputs_elements =
        sizeof(template_config_inputs) / sizeof(char*);
#endif

/*
 * Digital Output Webpage
 * ----------------------------------------------------------------------------
 */

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char template_digitaloutput_fname[] PROGMEM = "do.htm";
extern const uint16_t template_digitaloutput_fnsize =
        sizeof(template_digitaloutput_fname);
#endif

enum DIGITAL_OUTPUT
{
    DO_IPIN, DO_PIN, DO_IPWM, DO_PWM, DO_NAME, DO_ONVALUE
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitaloutput[] PROGMEM = {
    pgm_iname,
    pgm_pin,
    pgm_ipwm,
    pgm_pwm,
    pgm_aname,
    pgm_typeoptions };

extern const uint16_t template_digitaloutput_elements =
        sizeof(template_digitaloutput) / sizeof(char*);
#endif

enum DIGITAL_OUTPUT_INPUTS
{
    DO_I_TYPE, DO_I_PIN, DO_I_PWM
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitaloutput_inputs[] PROGMEM = {
    pgm_input_type,
    pgm_input_ipin,
    pgm_input_ipwm };

extern const uint16_t template_digitaloutput_inputs_elements =
        sizeof(template_digitaloutput_inputs) / sizeof(char*);
#endif

/*
 * Digital Input Webpage
 * ----------------------------------------------------------------------------
 */

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char template_digitalinput_fname[] PROGMEM = "di.htm";
extern const uint16_t template_digitalinput_fnsize =
        sizeof(template_digitalinput_fname);
#endif

enum TEMPLATE_DIGITALINPUT
{
    DI_INAME, DI_NAME, DI_PIN
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitalinput[] PROGMEM = {
    pgm_iname,
    pgm_name,
    pgm_pin };
extern const uint16_t template_digitalinput_elements =
        sizeof(template_digitalinput) / sizeof(char*);
#endif

enum TEMPLATE_DIGITALINPUT_INPUTS
{
    DI_I_PIN
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitalinput_inputs[] PROGMEM = {
    pgm_input_ipin };
extern const uint16_t template_digitalinput_inputs_elements =
        sizeof(template_digitalinput_inputs) / sizeof(char*);
#endif

/*
 * DS18S20 Webpage
 * ----------------------------------------------------------------------------
 */
#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char template_ds18s20_fname[] PROGMEM = "DS18S20.htm";
extern const uint16_t template_ds18s20_fnsize = sizeof(template_ds18s20_fname);
#endif

enum TEMPLATE_DS18S20
{
    DS18S20_INAME, DS18S20_NAME, DS18S20_PIN, DS18S20_ADDRESSSELECT
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_ds18s20[] PROGMEM = {
    pgm_iname,
    pgm_name,
    pgm_pin,
    pgm_addressselect };
extern const uint16_t template_ds18s20_elements = sizeof(template_ds18s20)
        / sizeof(char*);
#endif

enum TEMPLATE_DS18S20_INPUTS
{
    DS18S20_I_PIN, DS18S20_I_ADDRESS
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_ds18s20_inputs[] PROGMEM = {
    pgm_input_ipin,
    pgm_input_address };
extern const uint16_t template_ds18s20_inputs_elements =
        sizeof(template_ds18s20_inputs) / sizeof(char*);
#endif

/*
 * ClockTimerController Webpage
 * ----------------------------------------------------------------------------
 */

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char template_clocktimercontroller_fname[] PROGMEM = "clock.htm";
extern const uint16_t template_clocktimercontroller_fnsize =
        sizeof(template_clocktimercontroller_fname);
extern const char template_clocktimercontroller_row_fname[] PROGMEM
= "clockrow.htm";
extern const uint16_t template_clocktimercontroller_row_fnsize =
        sizeof(template_clocktimercontroller_row_fname);
#endif

enum TEMPLATE_CLOCKTIMERCONTROLLER
{
    CLOCKTIMERCONTROLLER_URL,
    CLOCKTIMERCONTROLLER_SELECT,
    CLOCKTIMERCONTROLLER_ACTUATORSELECT,
    CLOCKTIMERCONTROLLER_ROW,
    CLOCKTIMERCONTROLLER_CHECKED_MO,
    CLOCKTIMERCONTROLLER_CHECKED_TU,
    CLOCKTIMERCONTROLLER_CHECKED_WE,
    CLOCKTIMERCONTROLLER_CHECKED_TH,
    CLOCKTIMERCONTROLLER_CHECKED_FR,
    CLOCKTIMERCONTROLLER_CHECKED_SA,
    CLOCKTIMERCONTROLLER_CHECKED_SU
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_clocktimercontroller[] PROGMEM = {
    pgm_url,
    pgm_clocktimerselect,
    pgm_actuatorselect,
    pgm_clocktimerrow,
    pgm_checked_mo,
    pgm_checked_tu,
    pgm_checked_we,
    pgm_checked_th,
    pgm_checked_fr,
    pgm_checked_sa,
    pgm_checked_su };
extern const uint16_t template_clocktimercontroller_elements =
        sizeof(template_clocktimercontroller) / sizeof(char*);
#endif

enum TEMPLATE_CLOCKTIMERCONTROLLER_ROW
{
    CLOCKTIMERCONTROLLER_I_COLOR,
    CLOCKTIMERCONTROLLER_I_HON,
    CLOCKTIMERCONTROLLER_I_MON,
    CLOCKTIMERCONTROLLER_I_HOFF,
    CLOCKTIMERCONTROLLER_I_MOFF,
    CLOCKTIMERCONTROLLER_HON,
    CLOCKTIMERCONTROLLER_MON,
    CLOCKTIMERCONTROLLER_HOFF,
    CLOCKTIMERCONTROLLER_MOFF,
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_clocktimercontroller_row[] PROGMEM = {
    pgm_color,
    pgm_ihon,
    pgm_imon,
    pgm_ihoff,
    pgm_imoff,
    pgm_hon,
    pgm_mon,
    pgm_hoff,
    pgm_moff };
extern const uint16_t template_clocktimercontroller_row_elements =
        sizeof(template_clocktimercontroller_row) / sizeof(char*);
#endif

enum TEMPLATE_CLOCKTIMERCONTROLLER_INPUTS
{
    CLOCKTIMERCONTROLLER_I_TIMER,
    CLOCKTIMERCONTROLLER_I_ACTUATOR,
    CLOCKTIMERCONTROLLER_I_DOW,
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_clocktimercontroller_inputs[] PROGMEM = {
    pgm_input_timer,
    pgm_input_actuator,
    pgm_input_dow };
extern const uint16_t template_clocktimercontroller_inputs_elements =
        sizeof(template_clocktimercontroller_inputs) / sizeof(char*);
#endif

/*
 * LevelController Webpage
 * ----------------------------------------------------------------------------
 */

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char template_levelcontroller_fname[] PROGMEM = "level.htm";
extern const uint16_t template_levelcontroller_fnsize =
        sizeof(template_levelcontroller_fname);
#endif

enum TEMPLATE_LEVELCONTROLLER
{
    TEMPLATE_LEVELCONTROLLER_SSELECT,
    TEMPLATE_LEVELCONTROLLER_STATE,
    TEMPLATE_LEVELCONTROLLER_DELAYLOW,
    TEMPLATE_LEVELCONTROLLER_DELAYHIGH,
    TEMPLATE_LEVELCONTROLLER_REFILLTIMEOUT
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_levelcontroller[] PROGMEM = {
    pgm_sselect,
    pgm_state,
    pgm_delaylow,
    pgm_delayhigh,
    pgm_refilltimeout };
extern const uint16_t template_levelcontroller_elements =
        sizeof(template_levelcontroller) / sizeof(char*);
#endif

enum TEMPLATE_LEVELCONTROLLER_STATES
{
    TEMPLATE_LEVELCONTROLLER_STATE_OK,
    TEMPLATE_LEVELCONTROLLER_STATE_DEBOUNCE,
    TEMPLATE_LEVELCONTROLLER_STATE_REFILL,
    TEMPLATE_LEVELCONTROLLER_STATE_OVERRUN,
    TEMPLATE_LEVELCONTROLLER_STATE_REFILL_TIMEOUT
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_levelcontroller_states[] PROGMEM = {
    pgm_ok,
    pgm_debounce,
    pgm_refill,
    pgm_overrun,
    pgm_timeout };

extern const uint16_t template_levelcontroller_states_elements =
        sizeof(template_levelcontroller) / sizeof(char*);
#endif

enum TEMPLATE_LEVELCONTROLLER_INPUTS
{
    LEVELCONTROLLER_I_DELAYLOW,
    LEVELCONTROLLER_I_DELAYHIGH,
    LEVELCONTROLLER_I_REFILLTIMEOUT,
    LEVELCONTROLLER_I_SENSOR
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_levelcontroller_inputs[] PROGMEM = {
    pgm_input_delaylow,
    pgm_input_delayhigh,
    pgm_input_refilltimeout,
    pgm_input_sensor };
extern const uint16_t template_levelcontroller_inputs_elements =
        sizeof(template_levelcontroller) / sizeof(char*);
#endif

/*
 * LevelController Webpage
 * ----------------------------------------------------------------------------
 */

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char template_temperaturecontroller_fname[] PROGMEM = "temp.htm";
extern const uint16_t template_temperaturecontroller_fnsize =
        sizeof(template_temperaturecontroller_fname);
#endif

enum TEMPLATE_TEMPERATURECONTROLLER
{
    TEMPERATURECONTROLLER_SSELECT,
    TEMPERATURECONTROLLER_TEMPERATURE,
    TEMPERATURECONTROLLER_THRESHOLD,
    TEMPERATURECONTROLLER_PWMMAX,
    TEMPERATURECONTROLLER_HYSTERESIS
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_temperaturecontroller[] PROGMEM = {
    pgm_sselect,
    pgm_temperature,
    pgm_threshold,
    pgm_pwmmax,
    pgm_hysteresis };
extern const uint16_t template_temperaturecontroller_elements =
        sizeof(template_temperaturecontroller) / sizeof(char*);
#endif

enum TEMPLATE_TEMPERATURECONTROLLER_INPUTS
{
    TEMPERATURECONTROLLER_I_THRESHOLD,
    TEMPERATURECONTROLLER_I_PWMMAX,
    TEMPERATURECONTROLLER_I_HYSTERESIS,
    TEMPERATURECONTROLLER_I_SENSOR
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_temperaturecontroller_inputs[] PROGMEM = {
    pgm_input_threshold,
    pgm_inputpwmmax,
    pgm_input_hysteresis,
    pgm_input_sensor };
extern const uint16_t template_temperaturecontroller_inputs_elements =
        sizeof(template_temperaturecontroller_inputs) / sizeof(char*);
#endif
