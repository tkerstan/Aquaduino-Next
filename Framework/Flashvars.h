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

extern const char pgm_input_mac1[] PROGMEM;
extern const char pgm_input_mac2[] PROGMEM;
extern const char pgm_input_mac3[] PROGMEM;
extern const char pgm_input_mac4[] PROGMEM;
extern const char pgm_input_mac5[] PROGMEM;
extern const char pgm_input_mac6[] PROGMEM;
extern const char pgm_input_dhcp[] PROGMEM;
extern const char pgm_input_ip1[] PROGMEM;
extern const char pgm_input_ip2[] PROGMEM;
extern const char pgm_input_ip3[] PROGMEM;
extern const char pgm_input_ip4[] PROGMEM;
extern const char pgm_input_netmask1[] PROGMEM;
extern const char pgm_input_netmask2[] PROGMEM;
extern const char pgm_input_netmask3[] PROGMEM;
extern const char pgm_input_netmask4[] PROGMEM;
extern const char pgm_input_gateway1[] PROGMEM;
extern const char pgm_input_gateway2[] PROGMEM;
extern const char pgm_input_gateway3[] PROGMEM;
extern const char pgm_input_gateway4[] PROGMEM;
extern const char pgm_input_dns1[] PROGMEM;
extern const char pgm_input_dns2[] PROGMEM;
extern const char pgm_input_dns3[] PROGMEM;
extern const char pgm_input_dns4[] PROGMEM;
extern const char pgm_input_ntp[] PROGMEM;
extern const char pgm_input_ntp1[] PROGMEM;
extern const char pgm_input_ntp2[] PROGMEM;
extern const char pgm_input_ntp3[] PROGMEM;
extern const char pgm_input_ntp4[] PROGMEM;
extern const char pgm_input_ntp_period[] PROGMEM;
extern const char pgm_input_timezone[] PROGMEM;
extern const char pgm_input_hour[] PROGMEM;
extern const char pgm_input_minute[] PROGMEM;
extern const char pgm_input_second[] PROGMEM;
extern const char pgm_input_month[] PROGMEM;
extern const char pgm_input_day[] PROGMEM;
extern const char pgm_input_year[] PROGMEM;
extern const char pgm_input_xively[] PROGMEM;
extern const char pgm_input_xively_api_key[] PROGMEM;
extern const char pgm_input_xively_feed[] PROGMEM;
extern const char pgm_input_type[] PROGMEM;
extern const char pgm_input_address[] PROGMEM;
extern const char pgm_input_timer[] PROGMEM;
extern const char pgm_input_actuator[] PROGMEM;
extern const char pgm_input_actuator1[] PROGMEM;
extern const char pgm_input_actuator2[] PROGMEM;
extern const char pgm_input_dow[] PROGMEM;
extern const char pgm_input_delayl[] PROGMEM;
extern const char pgm_input_delayh[] PROGMEM;
extern const char pgm_input_timeout[] PROGMEM;
extern const char pgm_input_sensor[] PROGMEM;
extern const char pgm_input_reftemp1[] PROGMEM;
extern const char pgm_input_hyst1[] PROGMEM;
extern const char pgm_input_reftemp2[] PROGMEM;
extern const char pgm_input_hyst2[] PROGMEM;
extern const char pgm_input_pin[] PROGMEM;
extern const char pgm_input_pwm[] PROGMEM;
extern const char pgm_input_on[] PROGMEM;

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
static const char pgm_name[] PROGMEM = "##NAME##";
static const char pgm_value[] PROGMEM = "##VALUE##";
static const char pgm_color[] PROGMEM = "##COLOR##";
static const char pgm_iname[] PROGMEM = "##INAME##";
static const char pgm_link[] PROGMEM = "##LINK##";
static const char pgm_controllerrow[] PROGMEM = "##CONTROLLERROW##";
static const char pgm_actuatorrow[] PROGMEM = "##ACTUATORROW##";
static const char pgm_sensor[] PROGMEM = "##SENSORROW##";
static const char pgm_cselect[] PROGMEM = "##CSELECT##";
static const char pgm_coptions[] PROGMEM = "##COPTIONS##";
static const char pgm_lselect[] PROGMEM = "##LSELECT##";
static const char pgm_loptions[] PROGMEM = "##LOPTIONS##";
static const char pgm_sselect[] PROGMEM = "##SSELECT##";
static const char pgm_soptions[] PROGMEM = "##SOPTIONS##";
static const char pgm_pin_name[] PROGMEM = "##PINNAME##";
static const char pgm_pin_val[] PROGMEM = "##PINVAL##";
static const char pgm_pin_size[] PROGMEM = "##PINSIZE##";
static const char pgm_pin_maxlength[] PROGMEM = "##PINMAXLENGTH##";

/*
 * Main
 */
static const char pgm_hour[] PROGMEM = "##HOUR##";
static const char pgm_minute[] PROGMEM = "##MINUTE##";
static const char pgm_second[] PROGMEM = "##SECOND##";
static const char pgm_dow[] PROGMEM = "##DOW##";
static const char pgm_month[] PROGMEM = "##MONTH##";
static const char pgm_day[] PROGMEM = "##DAY##";
static const char pgm_year[] PROGMEM = "##YEAR##";

/**
 * Main Controller Row
 */
static const char pgm_crowcolor[] PROGMEM = "##CROWCOLOR##";
static const char pgm_cval[] PROGMEM = "##CVAL##";
static const char pgm_cname[] PROGMEM = "##CNAME##";
static const char pgm_csize[] PROGMEM = "##CSIZE##";
static const char pgm_cmaxlength[] PROGMEM = "##CMAXLENGTH##";
static const char pgm_clink[] PROGMEM = "##CLINK##";

/**
 * Main Sensor Row
 */
static const char pgm_srowcolor[] PROGMEM = "##SROWCOLOR##";
static const char pgm_sname[] PROGMEM = "##SNAME##";
static const char pgm_sval[] PROGMEM = "##SVAL##";
static const char pgm_ssize[] PROGMEM = "##SSIZE##";
static const char pgm_smaxlength[] PROGMEM = "##SMAXLENGTH##";
static const char pgm_svalue[] PROGMEM = "##SVALUE##";
static const char pgm_xchannelval[] PROGMEM = "##XCHANNELVAL##";
static const char pgm_xchannelname[] PROGMEM = "##XCHANNELNAME##";
static const char pgm_xchannelsize[] PROGMEM = "##XCHANNELSIZE##";
static const char pgm_xchannelmaxlength[]PROGMEM = "##XCHANNELMAXLENGTH##";
static const char pgm_slink[] PROGMEM = "##SLINK##";

/**
 * Main Actuator Row
 */
static const char pgm_arowcolor[] PROGMEM = "##AROWCOLOR##";
static const char pgm_aname[] PROGMEM = "##ANAME##";
static const char pgm_aval[] PROGMEM = "##AVAL##";
static const char pgm_asize[] PROGMEM = "##ASIZE##";
static const char pgm_amaxlength[] PROGMEM = "##AMAXLENGTH##";
static const char pgm_alink[] PROGMEM = "##ALINK##";

/**
 * Config
 */
static const char pgm_mac1_name[] PROGMEM = "##MAC1NAME##";
static const char pgm_mac2_name[] PROGMEM = "##MAC2NAME##";
static const char pgm_mac3_name[] PROGMEM = "##MAC3NAME##";
static const char pgm_mac4_name[] PROGMEM = "##MAC4NAME##";
static const char pgm_mac5_name[] PROGMEM = "##MAC5NAME##";
static const char pgm_mac6_name[] PROGMEM = "##MAC6NAME##";
static const char pgm_mac1_val[] PROGMEM = "##MAC1VAL##";
static const char pgm_mac2_val[] PROGMEM = "##MAC2VAL##";
static const char pgm_mac3_val[] PROGMEM = "##MAC3VAL##";
static const char pgm_mac4_val[] PROGMEM = "##MAC4VAL##";
static const char pgm_mac5_val[] PROGMEM = "##MAC5VAL##";
static const char pgm_mac6_val[] PROGMEM = "##MAC6VAL##";
static const char pgm_dhcp_select[] PROGMEM = "##DHCPSELECTOPTION##";
static const char pgm_ip1_name[] PROGMEM = "##IP1NAME##";
static const char pgm_ip2_name[] PROGMEM = "##IP2NAME##";
static const char pgm_ip3_name[] PROGMEM = "##IP3NAME##";
static const char pgm_ip4_name[] PROGMEM = "##IP4NAME##";
static const char pgm_ip1_val[] PROGMEM = "##IP1VAL##";
static const char pgm_ip2_val[] PROGMEM = "##IP2VAL##";
static const char pgm_ip3_val[] PROGMEM = "##IP3VAL##";
static const char pgm_ip4_val[] PROGMEM = "##IP4VAL##";
static const char pgm_nm1_name[] PROGMEM = "##NETMASK1NAME##";
static const char pgm_nm2_name[] PROGMEM = "##NETMASK2NAME##";
static const char pgm_nm3_name[] PROGMEM = "##NETMASK3NAME##";
static const char pgm_nm4_name[] PROGMEM = "##NETMASK4NAME##";
static const char pgm_nm1_val[] PROGMEM = "##NETMASK1VAL##";
static const char pgm_nm2_val[] PROGMEM = "##NETMASK2VAL##";
static const char pgm_nm3_val[] PROGMEM = "##NETMASK3VAL##";
static const char pgm_nm4_val[] PROGMEM = "##NETMASK4VAL##";
static const char pgm_gw1_name[] PROGMEM = "##GATEWAY1NAME##";
static const char pgm_gw2_name[] PROGMEM = "##GATEWAY2NAME##";
static const char pgm_gw3_name[] PROGMEM = "##GATEWAY3NAME##";
static const char pgm_gw4_name[] PROGMEM = "##GATEWAY4NAME##";
static const char pgm_gw1_val[] PROGMEM = "##GATEWAY1VAL##";
static const char pgm_gw2_val[] PROGMEM = "##GATEWAY2VAL##";
static const char pgm_gw3_val[] PROGMEM = "##GATEWAY3VAL##";
static const char pgm_gw4_val[] PROGMEM = "##GATEWAY4VAL##";
static const char pgm_dns1_name[] PROGMEM = "##DNS1NAME##";
static const char pgm_dns2_name[] PROGMEM = "##DNS2NAME##";
static const char pgm_dns3_name[] PROGMEM = "##DNS3NAME##";
static const char pgm_dns4_name[] PROGMEM = "##DNS4NAME##";
static const char pgm_dns1_val[] PROGMEM = "##DNS1VAL##";
static const char pgm_dns2_val[] PROGMEM = "##DNS2VAL##";
static const char pgm_dns3_val[] PROGMEM = "##DNS3VAL##";
static const char pgm_dns4_val[] PROGMEM = "##DNS4VAL##";
static const char pgm_ntp_select[] PROGMEM = "##NTPSELECTOPTION##";
static const char pgm_ntp1_name[] PROGMEM = "##NTP1NAME##";
static const char pgm_ntp2_name[] PROGMEM = "##NTP2NAME##";
static const char pgm_ntp3_name[] PROGMEM = "##NTP3NAME##";
static const char pgm_ntp4_name[] PROGMEM = "##NTP4NAME##";
static const char pgm_ntp1_val[] PROGMEM = "##NTP1VAL##";
static const char pgm_ntp2_val[] PROGMEM = "##NTP2VAL##";
static const char pgm_ntp3_val[] PROGMEM = "##NTP3VAL##";
static const char pgm_ntp4_val[] PROGMEM = "##NTP4VAL##";
static const char pgm_ntp_period_val[] PROGMEM = "##NTPPERIODVAL##";
static const char pgm_ntp_period_size[] PROGMEM = "##NTPPERIODSIZE##";
static const char pgm_ntp_period_maxlength[] PROGMEM = "##NTPPERIODMAXLENGTH##";
static const char pgm_timezone[] PROGMEM = "##TIMEZONE##";
static const char pgm_xively_select[] PROGMEM = "##XIVELYSELECTOPTION##";
static const char pgm_xively_select_name[] PROGMEM = "##XIVELYSELECTNAME##";
static const char pgm_xively_api_name[] PROGMEM = "##XAPINAME##";
static const char pgm_xively_api_val[] PROGMEM = "##XAPIVAL##";
static const char pgm_xively_api_size[] PROGMEM = "##XAPISIZE##";
static const char pgm_xively_api_maxlength[] PROGMEM = "##XAPIMAXLENGTH##";
static const char pgm_xively_feed_name[] PROGMEM = "##XFEEDNAME##";
static const char pgm_xively_feed_val[] PROGMEM = "##XFEEDVAL##";
static const char pgm_xively_feed_size[] PROGMEM = "##XFEEDSIZE##";
static const char pgm_xively_feed_maxlength[] PROGMEM = "##XFEEDMAXLENGTH##";
static const char pgm_free_ram[] PROGMEM = "##FREERAM##";

/**
 * Level Controller
 */
static const char pgm_state[] PROGMEM = "##STATE##";
static const char pgm_delayhigh_name[] PROGMEM = "##DELAYHNAME##";
static const char pgm_delayhigh_val[] PROGMEM = "##DELAYHVAL##";
static const char pgm_delayhigh_size[] PROGMEM = "##DELAYHSIZE##";
static const char pgm_delayhigh_maxlength[] PROGMEM = "##DELAYHMAXLENGTH##";
static const char pgm_delaylow_name[] PROGMEM = "##DELAYLNAME##";
static const char pgm_delaylow_val[] PROGMEM = "##DELAYLVAL##";
static const char pgm_delaylow_size[] PROGMEM = "##DELAYLSIZE##";
static const char pgm_delaylow_maxlength[] PROGMEM = "##DELAYLMAXLENGTH##";
static const char pgm_timeout_name[] PROGMEM = "##TIMEOUTNAME##";
static const char pgm_timeout_val[] PROGMEM = "##TIMEOUTVAL##";
static const char pgm_timeout_size[] PROGMEM = "##TIMEOUTSIZE##";
static const char pgm_timeout_maxlength[] PROGMEM = "##TIMEOUTMAXLENGTH##";

static const char pgm_ok[] PROGMEM = "OK";
static const char pgm_debounce[] PROGMEM = "DEBOUNCE";
static const char pgm_refill[] PROGMEM = "REFILL";
static const char pgm_overrun[] PROGMEM = "OVERRUN";
static const char pgm_timeout[] PROGMEM = "TIMEOUT";

/**
 * Temperature Controller
 */

static const char pgm_temperature[] PROGMEM = "##TEMPERATURE##";
static const char pgm_unit[] PROGMEM = "##UNIT##";
static const char pgm_reftemp1_name[] PROGMEM = "##RTEMP1NAME##";
static const char pgm_reftemp1_val[] PROGMEM = "##RTEMP1VAL##";
static const char pgm_reftemp1_size[] PROGMEM = "##RTEMP1SIZE##";
static const char pgm_reftemp1_maxlength[] PROGMEM = "##RTEMP1MAXLENGTH##";
static const char pgm_hyst1_name[] PROGMEM = "##HYST1NAME##";
static const char pgm_hyst1_val[] PROGMEM = "##HYST1VAL##";
static const char pgm_hyst1_size[] PROGMEM = "##HYST1SIZE##";
static const char pgm_hyst1_maxlength[] PROGMEM = "##HYST1MAXLENGTH##";
static const char pgm_aselect1[] PROGMEM = "##ASELECT1##";
static const char pgm_reftemp2_name[] PROGMEM = "##RTEMP2NAME##";
static const char pgm_reftemp2_val[] PROGMEM = "##RTEMP2VAL##";
static const char pgm_reftemp2_size[] PROGMEM = "##RTEMP2SIZE##";
static const char pgm_reftemp2_maxlength[] PROGMEM = "##RTEMP2MAXLENGTH##";
static const char pgm_hyst2_name[] PROGMEM = "##HYST2NAME##";
static const char pgm_hyst2_val[] PROGMEM = "##HYST2VAL##";
static const char pgm_hyst2_size[] PROGMEM = "##HYST2SIZE##";
static const char pgm_hyst2_maxlength[] PROGMEM = "##HYST2MAXLENGTH##";
static const char pgm_aselect2[] PROGMEM = "##ASELECT2##";

/**
 * Digital Output
 */
static const char pgm_pwm_name[] PROGMEM = "##PWMNAME##";
static const char pgm_pwm_val[] PROGMEM = "##PWMVAL##";
static const char pgm_pwm_size[] PROGMEM = "##PWMSIZE##";
static const char pgm_pwm_maxlength[] PROGMEM = "##PWMMAXLENGTH##";
static const char pgm_on_name[] PROGMEM = "##ONNAME##";
static const char pgm_on_options[] PROGMEM = "##ONOPTIONS##";

/**
 * DS182S20
 */
static const char pgm_addressselect[] PROGMEM = "##ADDRESSSELECT##";

/**
 * Clock Timer Controller
 */
static const char pgm_actionurl[] PROGMEM = "##ACTIONURL##";
static const char pgm_clocktimerselect[] PROGMEM = "##CLOCKTIMERSELECT##";
static const char pgm_actuatorselect[] PROGMEM = "##ACTUATORSELECT##";
static const char pgm_clocktimerrow[] PROGMEM = "##CLOCKTIMERROW##";
static const char pgm_dow_name[] PROGMEM = "##DOWNAME##";
static const char pgm_checked_mo[] PROGMEM = "##CHECKEDMO##";
static const char pgm_checked_tu[] PROGMEM = "##CHECKEDTU##";
static const char pgm_checked_we[] PROGMEM = "##CHECKEDWE##";
static const char pgm_checked_th[] PROGMEM = "##CHECKEDTH##";
static const char pgm_checked_fr[] PROGMEM = "##CHECKEDFR##";
static const char pgm_checked_sa[] PROGMEM = "##CHECKEDSA##";
static const char pgm_checked_su[] PROGMEM = "##CHECKEDSU##";
static const char pgm_hon_name[] PROGMEM = "##HONNAME##";
static const char pgm_hon_val[] PROGMEM = "##HONVAL##";
static const char pgm_hon_size[] PROGMEM = "##HONSIZE##";
static const char pgm_hon_maxlength[] PROGMEM = "##HONMAXLENGTH##";
static const char pgm_mon_name[] PROGMEM = "##MONNAME##";
static const char pgm_mon_val[] PROGMEM = "##MONVAL##";
static const char pgm_mon_size[] PROGMEM = "##MONSIZE##";
static const char pgm_mon_maxlength[] PROGMEM = "##MONMAXLENGTH##";
static const char pgm_hoff_name[] PROGMEM = "##HOFFNAME##";
static const char pgm_hoff_val[] PROGMEM = "##HOFFVAL##";
static const char pgm_hoff_size[] PROGMEM = "##HOFFSIZE##";
static const char pgm_hoff_maxlength[] PROGMEM = "##HOFFMAXLENGTH##";
static const char pgm_moff_name[] PROGMEM = "##MOFFNAME##";
static const char pgm_moff_val[] PROGMEM = "##MOFFVAL##";
static const char pgm_moff_size[] PROGMEM = "##MOFFSIZE##";
static const char pgm_moff_maxlength[] PROGMEM = "##MOFFMAXLENGTH##";


/**
 * Template POST Strings
 */
extern const char pgm_input_mac1[] PROGMEM = "mac1";
extern const char pgm_input_mac2[] PROGMEM = "mac2";
extern const char pgm_input_mac3[] PROGMEM = "mac3";
extern const char pgm_input_mac4[] PROGMEM = "mac4";
extern const char pgm_input_mac5[] PROGMEM = "mac5";
extern const char pgm_input_mac6[] PROGMEM = "mac6";
extern const char pgm_input_dhcp[] PROGMEM = "dhcp";
extern const char pgm_input_ip1[] PROGMEM = "ip1";
extern const char pgm_input_ip2[] PROGMEM = "ip2";
extern const char pgm_input_ip3[] PROGMEM = "ip3";
extern const char pgm_input_ip4[] PROGMEM = "ip4";
extern const char pgm_input_netmask1[] PROGMEM = "netmask1";
extern const char pgm_input_netmask2[] PROGMEM = "netmask2";
extern const char pgm_input_netmask3[] PROGMEM = "netmask3";
extern const char pgm_input_netmask4[] PROGMEM = "netmask4";
extern const char pgm_input_gateway1[] PROGMEM = "gateway1";
extern const char pgm_input_gateway2[] PROGMEM = "gateway2";
extern const char pgm_input_gateway3[] PROGMEM = "gateway3";
extern const char pgm_input_gateway4[] PROGMEM = "gateway4";
extern const char pgm_input_dns1[] PROGMEM = "dns1";
extern const char pgm_input_dns2[] PROGMEM = "dns2";
extern const char pgm_input_dns3[] PROGMEM = "dns3";
extern const char pgm_input_dns4[] PROGMEM = "dns4";
extern const char pgm_input_ntp[] PROGMEM = "ntp";
extern const char pgm_input_ntp1[] PROGMEM = "ntp1";
extern const char pgm_input_ntp2[] PROGMEM = "ntp2";
extern const char pgm_input_ntp3[] PROGMEM = "ntp3";
extern const char pgm_input_ntp4[] PROGMEM = "ntp4";
extern const char pgm_input_ntp_period[] PROGMEM = "ntpperiod";
extern const char pgm_input_timezone[] PROGMEM = "timezone";
extern const char pgm_input_hour[] PROGMEM = "hour";
extern const char pgm_input_minute[] PROGMEM = "minute";
extern const char pgm_input_second[] PROGMEM = "second";
extern const char pgm_input_month[] PROGMEM = "month";
extern const char pgm_input_day[] PROGMEM = "day";
extern const char pgm_input_year[] PROGMEM = "year";
extern const char pgm_input_xively[] PROGMEM = "xively";
extern const char pgm_input_xively_api_key[] PROGMEM = "xivelykey";
extern const char pgm_input_xively_feed[] PROGMEM = "xivelyfeed";
extern const char pgm_input_actuator[] PROGMEM = "actuator";
extern const char pgm_input_actuator1[] PROGMEM = "actuator1";
extern const char pgm_input_actuator2[] PROGMEM = "actuator2";
extern const char pgm_input_dow[] PROGMEM = "dow";
extern const char pgm_input_delayl[] PROGMEM = "delayl";
extern const char pgm_input_delayh[] PROGMEM = "delayh";
extern const char pgm_input_timeout[] PROGMEM = "timeout";
extern const char pgm_input_sensor[] PROGMEM = "sensor";
extern const char pgm_input_reftemp1[] PROGMEM = "reftemp1";
extern const char pgm_input_hyst1[] PROGMEM = "hyst1";
extern const char pgm_input_reftemp2[] PROGMEM = "reftemp2";
extern const char pgm_input_hyst2[] PROGMEM = "hyst2";
extern const char pgm_input_pin[] PROGMEM = "pin";
extern const char pgm_input_pwm[] PROGMEM = "pwm";
extern const char pgm_input_on[] PROGMEM = "on";
extern const char pgm_url_select[] PROGMEM = "select";
extern const char pgm_input_address[] PROGMEM = "address";
extern const char pgm_input_timer[] PROGMEM = "timer";

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

extern const char* const template_main[] PROGMEM =
{
    pgm_hour,
    pgm_minute,
    pgm_second,
    pgm_dow,
    pgm_month,
    pgm_day,
    pgm_year,
    pgm_controllerrow,
    pgm_sensor,
    pgm_actuatorrow};

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
    CONTROLLERROW_I_CNAME_NAME,
    CONTROLLERROW_I_CNAME_VAL,
    CONTROLLERROW_I_CNAME_SIZE,
    CONTROLLERROW_I_CNAME_MAXLENGTH,
    CONTROLLERROW_CLINK
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_main_controllerrow[] PROGMEM =
{
    pgm_crowcolor,
    pgm_cname,
    pgm_cval,
    pgm_csize,
    pgm_cmaxlength,
    pgm_clink};

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
    SENSORROW_I_SNAME_NAME,
    SENSORROW_I_SNAME_VAL,
    SENSORROW_I_SNAME_SIZE,
    SENSORROW_I_SNAME_MAXLENGTH,
    SENSORROW_SVALUE,
    SENSORROW_I_XCHANNEL_NAME,
    SENSORROW_I_XCHANNEL_VAL,
    SENSORROW_I_XCHANNEL_SIZE,
    SENSORROW_I_XCHANNEL_MAXLENGTH,
    SENSORROW_SLINK
};

#ifdef _FLASHVAR_IMPLEMENTATION_

extern const char* const template_main_sensorrow[] PROGMEM =
{
    pgm_srowcolor,
    pgm_sname,
    pgm_sval,
    pgm_ssize,
    pgm_smaxlength,
    pgm_svalue,
    pgm_xchannelname,
    pgm_xchannelval,
    pgm_xchannelsize,
    pgm_xchannelmaxlength,
    pgm_slink};

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
    ACTUATORROW_ACTUATOR_NAME,
    ACTUATORROW_ACTUATOR_VAL,
    ACTUATORROW_ACTUATOR_SIZE,
    ACTUATORROW_ACTUATOR_MAXLENGTH,
    ACTUATORROW_CSELECT,
    ACTUATORROW_COPTIONS,
    ACTUATORROW_LSELECT,
    ACTUATORROW_LOPTIONS,
    ACTUATORROW_SSELECT,
    ACTUATORROW_SOPTIONS,
    ACTUATORROW_LINK
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_main_actuatorrow[] PROGMEM =
{
    pgm_arowcolor,
    pgm_aname,
    pgm_aval,
    pgm_asize,
    pgm_amaxlength,
    pgm_cselect,
    pgm_coptions,
    pgm_lselect,
    pgm_loptions,
    pgm_sselect,
    pgm_soptions,
    pgm_alink};

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
    CONFIG_MAC1_NAME,
    CONFIG_MAC1_VAL,
    CONFIG_MAC2_NAME,
    CONFIG_MAC2_VAL,
    CONFIG_MAC3_NAME,
    CONFIG_MAC3_VAL,
    CONFIG_MAC4_NAME,
    CONFIG_MAC4_VAL,
    CONFIG_MAC5_NAME,
    CONFIG_MAC5_VAL,
    CONFIG_MAC6_NAME,
    CONFIG_MAC6_VAL,
    CONFIG_DHCPSELECTOPTION,
    CONFIG_IP1_NAME,
    CONFIG_IP1_VAL,
    CONFIG_IP2_NAME,
    CONFIG_IP2_VAL,
    CONFIG_IP3_NAME,
    CONFIG_IP3_VAL,
    CONFIG_IP4_NAME,
    CONFIG_IP4_VAL,
    CONFIG_NETMASK1_NAME,
    CONFIG_NETMASK1_VAL,
    CONFIG_NETMASK2_NAME,
    CONFIG_NETMASK2_VAL,
    CONFIG_NETMASK3_NAME,
    CONFIG_NETMASK3_VAL,
    CONFIG_NETMASK4_NAME,
    CONFIG_NETMASK4_VAL,
    CONFIG_GATEWAY1_NAME,
    CONFIG_GATEWAY1_VAL,
    CONFIG_GATEWAY2_NAME,
    CONFIG_GATEWAY2_VAL,
    CONFIG_GATEWAY3_NAME,
    CONFIG_GATEWAY3_VAL,
    CONFIG_GATEWAY4_NAME,
    CONFIG_GATEWAY4_VAL,
    CONFIG_DNS1_NAME,
    CONFIG_DNS1_VAL,
    CONFIG_DNS2_NAME,
    CONFIG_DNS2_VAL,
    CONFIG_DNS3_NAME,
    CONFIG_DNS3_VAL,
    CONFIG_DNS4_NAME,
    CONFIG_DNS4_VAL,
    CONFIG_NTPSELECTOPTION,
    CONFIG_NTP1_NAME,
    CONFIG_NTP1_VAL,
    CONFIG_NTP2_NAME,
    CONFIG_NTP2_VAL,
    CONFIG_NTP3_NAME,
    CONFIG_NTP3_VAL,
    CONFIG_NTP4_NAME,
    CONFIG_NTP4_VAL,
    CONFIG_NTPPERIODVAL,
    CONFIG_TIMEZONE,
    CONFIG_HOUR,
    CONFIG_MINUTE,
    CONFIG_SECOND,
    CONFIG_MONTH,
    CONFIG_DAY,
    CONFIG_YEAR,
    CONFIG_XIVELYSELECTNAME,
    CONFIG_XIVELYSELECT,
    CONFIG_XIVELYAPIKEYNAME,
    CONFIG_XIVELYAPIKEYVAL,
    CONFIG_XIVELYAPIKEYSIZE,
    CONFIG_XIVELYAPIKEYMAXLENGTH,
    CONFIG_XIVELYFEEDNAME,
    CONFIG_XIVELYFEEDVAL,
    CONFIG_XIVELYFEEDSIZE,
    CONFIG_XIVELYFEEDMAXLENGTH,
    CONFIG_FREERAM
};

#ifdef _FLASHVAR_IMPLEMENTATION_

extern const char* const template_config[] PROGMEM
=
{
    pgm_mac1_name,
    pgm_mac1_val,
    pgm_mac2_name,
    pgm_mac2_val,
    pgm_mac3_name,
    pgm_mac3_val,
    pgm_mac4_name,
    pgm_mac4_val,
    pgm_mac5_name,
    pgm_mac5_val,
    pgm_mac6_name,
    pgm_mac6_val,
    pgm_dhcp_select,
    pgm_ip1_name,
    pgm_ip1_val,
    pgm_ip2_name,
    pgm_ip2_val,
    pgm_ip3_name,
    pgm_ip3_val,
    pgm_ip4_name,
    pgm_ip4_val,
    pgm_nm1_name,
    pgm_nm1_val,
    pgm_nm2_name,
    pgm_nm2_val,
    pgm_nm3_name,
    pgm_nm3_val,
    pgm_nm4_name,
    pgm_nm4_val,
    pgm_gw1_name,
    pgm_gw1_val,
    pgm_gw2_name,
    pgm_gw2_val,
    pgm_gw3_name,
    pgm_gw3_val,
    pgm_gw4_name,
    pgm_gw4_val,
    pgm_dns1_name,
    pgm_dns1_val,
    pgm_dns2_name,
    pgm_dns2_val,
    pgm_dns3_name,
    pgm_dns3_val,
    pgm_dns4_name,
    pgm_dns4_val,
    pgm_ntp_select,
    pgm_ntp1_name,
    pgm_ntp1_val,
    pgm_ntp2_name,
    pgm_ntp2_val,
    pgm_ntp3_name,
    pgm_ntp3_val,
    pgm_ntp4_name,
    pgm_ntp4_val,
    pgm_ntp_period_val,
    pgm_timezone,
    pgm_hour,
    pgm_minute,
    pgm_second,
    pgm_month,
    pgm_day,
    pgm_year,
    pgm_xively_select_name,
    pgm_xively_select,
    pgm_xively_api_name,
    pgm_xively_api_val,
    pgm_xively_api_size,
    pgm_xively_api_maxlength,
    pgm_xively_feed_name,
    pgm_xively_feed_val,
    pgm_xively_feed_size,
    pgm_xively_feed_maxlength,
    pgm_free_ram};

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
extern const char* const template_config_inputs[] PROGMEM =
{
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
    pgm_input_xively_feed};

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
    DO_ANAME,
    DO_PIN_NAME,
    DO_PIN_VAL,
    DO_PIN_SIZE,
    DO_PIN_MAXLENGTH,
    DO_PWM_NAME,
    DO_PWM_VAL,
    DO_PWM_SIZE,
    DO_PWM_MAXLENGTH,
    DO_ON_NAME,
    DO_ON_OPTIONS
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitaloutput[] PROGMEM =
{
    pgm_aname,
    pgm_pin_name,
    pgm_pin_val,
    pgm_pin_size,
    pgm_pin_maxlength,
    pgm_pwm_name,
    pgm_pwm_val,
    pgm_pwm_size,
    pgm_pwm_maxlength,
    pgm_on_name,
    pgm_on_options};

extern const uint16_t template_digitaloutput_elements =
sizeof(template_digitaloutput) / sizeof(char*);
#endif

enum DIGITAL_OUTPUT_INPUTS
{
    DO_I_PIN, DO_I_PWM, DO_I_ON
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitaloutput_inputs[] PROGMEM =
{
    pgm_input_pin,
    pgm_input_pwm,
    pgm_input_on
};

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
    DI_SNAME, DI_PIN_NAME, DI_PIN_VAL, DI_PIN_SIZE, DI_PIN_MAXLENGTH
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitalinput[] PROGMEM =
{
    pgm_sname,
    pgm_pin_name,
    pgm_pin_val,
    pgm_pin_size,
    pgm_pin_maxlength
};
extern const uint16_t template_digitalinput_elements =
sizeof(template_digitalinput) / sizeof(char*);
#endif

enum TEMPLATE_DIGITALINPUT_INPUTS
{
    DI_I_PIN
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_digitalinput_inputs[] PROGMEM =
{
    pgm_input_pin
};
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
    DS18S20_SNAME,
    DS18S20_PIN_NAME,
    DS18S20_PIN_VAL,
    DS18S20_PIN_SIZE,
    DS18S20_PIN_MAXLENGTH,
    DS18S20_ADDRESSSELECT
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_ds18s20[] PROGMEM =
{
    pgm_sname,
    pgm_pin_name,
    pgm_pin_val,
    pgm_pin_size,
    pgm_pin_maxlength,
    pgm_addressselect
};

extern const uint16_t template_ds18s20_elements = sizeof(template_ds18s20)
/ sizeof(char*);
#endif

enum TEMPLATE_DS18S20_INPUTS
{
    DS18S20_I_PIN, DS18S20_I_ADDRESS
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_ds18s20_inputs[] PROGMEM =
{
    pgm_input_pin,
    pgm_input_address
};
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
    CLOCKTIMERCONTROLLER_CNAME,
    CLOCKTIMERCONTROLLER_ACTIONURL,
    CLOCKTIMERCONTROLLER_CLOCKTIMERSELECT,
    CLOCKTIMERCONTROLLER_ACTUATORSELECT,
    CLOCKTIMERCONTROLLER_ROW,
    CLOCKTIMERCONTROLLER_DOW_NAME,
    CLOCKTIMERCONTROLLER_CHECKED_MO,
    CLOCKTIMERCONTROLLER_CHECKED_TU,
    CLOCKTIMERCONTROLLER_CHECKED_WE,
    CLOCKTIMERCONTROLLER_CHECKED_TH,
    CLOCKTIMERCONTROLLER_CHECKED_FR,
    CLOCKTIMERCONTROLLER_CHECKED_SA,
    CLOCKTIMERCONTROLLER_CHECKED_SU
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_clocktimercontroller[] PROGMEM =
{
    pgm_cname,
    pgm_actionurl,
    pgm_clocktimerselect,
    pgm_actuatorselect,
    pgm_clocktimerrow,
    pgm_dow_name,
    pgm_checked_mo,
    pgm_checked_tu,
    pgm_checked_we,
    pgm_checked_th,
    pgm_checked_fr,
    pgm_checked_sa,
    pgm_checked_su};
extern const uint16_t template_clocktimercontroller_elements =
sizeof(template_clocktimercontroller) / sizeof(char*);
#endif

enum TEMPLATE_CLOCKTIMERCONTROLLER_ROW
{
    CLOCKTIMERCONTROLLER_COLOR,
    CLOCKTIMERCONTROLLER_HON_NAME,
    CLOCKTIMERCONTROLLER_HON_VAL,
    CLOCKTIMERCONTROLLER_HON_SIZE,
    CLOCKTIMERCONTROLLER_HON_MAXLENGTH,
    CLOCKTIMERCONTROLLER_MON_NAME,
    CLOCKTIMERCONTROLLER_MON_VAL,
    CLOCKTIMERCONTROLLER_MON_SIZE,
    CLOCKTIMERCONTROLLER_MON_MAXLENGTH,
    CLOCKTIMERCONTROLLER_HOFF_NAME,
    CLOCKTIMERCONTROLLER_HOFF_VAL,
    CLOCKTIMERCONTROLLER_HOFF_SIZE,
    CLOCKTIMERCONTROLLER_HOFF_MAXLENGTH,
    CLOCKTIMERCONTROLLER_MOFF_NAME,
    CLOCKTIMERCONTROLLER_MOFF_VAL,
    CLOCKTIMERCONTROLLER_MOFF_SIZE,
    CLOCKTIMERCONTROLLER_MOFF_MAXLENGTH,
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_clocktimercontroller_row[] PROGMEM =
{
    pgm_color,
    pgm_hon_name,
    pgm_hon_val,
    pgm_hon_size,
    pgm_hon_maxlength,
    pgm_mon_name,
    pgm_mon_val,
    pgm_mon_size,
    pgm_mon_maxlength,
    pgm_hoff_name,
    pgm_hoff_val,
    pgm_hoff_size,
    pgm_hoff_maxlength,
    pgm_moff_name,
    pgm_moff_val,
    pgm_moff_size,
    pgm_moff_maxlength
};
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
extern const char* const template_clocktimercontroller_inputs[] PROGMEM =
{
    pgm_input_timer,
    pgm_input_actuator,
    pgm_input_dow};
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
    LEVELCONTROLLER_CNAME,
    LEVELCONTROLLER_STATE,
    LEVELCONTROLLER_SSELECT,
    LEVELCONTROLLER_DELAYHNAME,
    LEVELCONTROLLER_DELAYHVAL,
    LEVELCONTROLLER_DELAYHSIZE,
    LEVELCONTROLLER_DELAYHMAXLENGTH,
    LEVELCONTROLLER_DELAYLNAME,
    LEVELCONTROLLER_DELAYLVAL,
    LEVELCONTROLLER_DELAYLSIZE,
    LEVELCONTROLLER_DELAYLMAXLENGTH,
    LEVELCONTROLLER_TIMEOUTNAME,
    LEVELCONTROLLER_TIMEOUTVAL,
    LEVELCONTROLLER_TIMEOUTSIZE,
    LEVELCONTROLLER_TIMEOUTMAXLENGTH,
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_levelcontroller[] PROGMEM =
{
    pgm_cname,
    pgm_state,
    pgm_sselect,
    pgm_delayhigh_name,
    pgm_delayhigh_val,
    pgm_delayhigh_size,
    pgm_delayhigh_maxlength,
    pgm_delaylow_name,
    pgm_delaylow_val,
    pgm_delaylow_size,
    pgm_delaylow_maxlength,
    pgm_timeout_name,
    pgm_timeout_val,
    pgm_timeout_size,
    pgm_timeout_maxlength,
};

extern const uint16_t template_levelcontroller_elements =
sizeof(template_levelcontroller) / sizeof(char*);
#endif

enum TEMPLATE_LEVELCONTROLLER_STATES
{
    LEVELCONTROLLER_STATE_OK,
    LEVELCONTROLLER_STATE_DEBOUNCE,
    LEVELCONTROLLER_STATE_REFILL,
    LEVELCONTROLLER_STATE_OVERRUN,
    LEVELCONTROLLER_STATE_REFILL_TIMEOUT
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_levelcontroller_states[] PROGMEM =
{
    pgm_ok,
    pgm_debounce,
    pgm_refill,
    pgm_overrun,
    pgm_timeout};

extern const uint16_t template_levelcontroller_states_elements =
sizeof(template_levelcontroller) / sizeof(char*);
#endif

enum TEMPLATE_LEVELCONTROLLER_INPUTS
{
    LEVELCONTROLLER_I_DELAYL,
    LEVELCONTROLLER_I_DELAYH,
    LEVELCONTROLLER_I_TIMEOUT,
    LEVELCONTROLLER_I_SENSOR
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_levelcontroller_inputs[] PROGMEM =
{
    pgm_input_delayl,
    pgm_input_delayh,
    pgm_input_timeout,
    pgm_input_sensor};

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
    TEMPERATURECONTROLLER_CNAME,
    TEMPERATURECONTROLLER_SSELECT,
    TEMPERATURECONTROLLER_TEMPERATURE,
    TEMPERATURECONTROLLER_UNIT,
    TEMPERATURECONTROLLER_REFTEMP1_NAME,
    TEMPERATURECONTROLLER_REFTEMP1_VAL,
    TEMPERATURECONTROLLER_REFTEMP1_SIZE,
    TEMPERATURECONTROLLER_REFTEMP1_MAXLENGTH,
    TEMPERATURECONTROLLER_HYST1_NAME,
    TEMPERATURECONTROLLER_HYST1_VAL,
    TEMPERATURECONTROLLER_HYST1_SIZE,
    TEMPERATURECONTROLLER_HYST1_MAXLENGTH,
    TEMPERATURECONTROLLER_ASELECT1,
    TEMPERATURECONTROLLER_REFTEMP2_NAME,
    TEMPERATURECONTROLLER_REFTEMP2_VAL,
    TEMPERATURECONTROLLER_REFTEMP2_SIZE,
    TEMPERATURECONTROLLER_REFTEMP2_MAXLENGTH,
    TEMPERATURECONTROLLER_HYST2_NAME,
    TEMPERATURECONTROLLER_HYST2_VAL,
    TEMPERATURECONTROLLER_HYST2_SIZE,
    TEMPERATURECONTROLLER_HYST2_MAXLENGTH,
    TEMPERATURECONTROLLER_ASELECT2,
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_temperaturecontroller[] PROGMEM =
{
    pgm_cname,
    pgm_sselect,
    pgm_temperature,
    pgm_unit,
    pgm_reftemp1_name,
    pgm_reftemp1_val,
    pgm_reftemp1_size,
    pgm_reftemp1_maxlength,
    pgm_hyst1_name,
    pgm_hyst1_val,
    pgm_hyst1_size,
    pgm_hyst1_maxlength,
    pgm_aselect1,
    pgm_reftemp2_name,
    pgm_reftemp2_val,
    pgm_reftemp2_size,
    pgm_reftemp2_maxlength,
    pgm_hyst2_name,
    pgm_hyst2_val,
    pgm_hyst2_size,
    pgm_hyst2_maxlength,
    pgm_aselect2,
};
extern const uint16_t template_temperaturecontroller_elements =
sizeof(template_temperaturecontroller) / sizeof(char*);
#endif

enum TEMPLATE_TEMPERATURECONTROLLER_INPUTS
{
    TEMPERATURECONTROLLER_I_SENSOR,
    TEMPERATURECONTROLLER_I_REFTEMP1,
    TEMPERATURECONTROLLER_I_HYST1,
    TEMPERATURECONTROLLER_I_ACTUATOR1,
    TEMPERATURECONTROLLER_I_REFTEMP2,
    TEMPERATURECONTROLLER_I_HYST2,
    TEMPERATURECONTROLLER_I_ACTUATOR2,
};

#ifdef _FLASHVAR_IMPLEMENTATION_
extern const char* const template_temperaturecontroller_inputs[] PROGMEM =
{
    pgm_input_sensor,
    pgm_input_reftemp1,
    pgm_input_hyst1,
    pgm_input_actuator1,
    pgm_input_reftemp2,
    pgm_input_hyst2,
    pgm_input_actuator2
};

extern const uint16_t template_temperaturecontroller_inputs_elements =
sizeof(template_temperaturecontroller_inputs) / sizeof(char*);
#endif
