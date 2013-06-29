enum CONFIG_TEMPLATE
{
    T_ACTORROW,
    T_MAC1,
    T_MAC2,
    T_MAC3,
    T_MAC4,
    T_MAC5,
    T_MAC6,
    T_DHCPSELECTOPTION,
    T_IP1,
    T_IP2,
    T_IP3,
    T_IP4,
    T_NETMASK1,
    T_NETMASK2,
    T_NETMASK3,
    T_NETMASK4,
    T_GATEWAY1,
    T_GATEWAY2,
    T_GATEWAY3,
    T_GATEWAY4,
    T_DNS1,
    T_DNS2,
    T_DNS3,
    T_DNS4,
    T_NTPSELECTOPTION,
    T_NTP1,
    T_NTP2,
    T_NTP3,
    T_NTP4,
    T_NTPPERIOD,
    T_TIMEZONE,
    T_HOUR,
    T_MINUTE,
    T_SECOND,
    T_XIVELY,
    T_XIVELYAPIKEY,
    T_XIVELYFEED,
    T_FREERAM
};

enum CONFIG_INPUTS
{
    I_MAC1,
    I_MAC2,
    I_MAC3,
    I_MAC4,
    I_MAC5,
    I_MAC6,
    I_DHCP,
    I_IP1,
    I_IP2,
    I_IP3,
    I_IP4,
    I_NETMASK1,
    I_NETMASK2,
    I_NETMASK3,
    I_NETMASK4,
    I_GATEWAY1,
    I_GATEWAY2,
    I_GATEWAY3,
    I_GATEWAY4,
    I_DNS1,
    I_DNS2,
    I_DNS3,
    I_DNS4,
    I_NTP,
    I_NTP1,
    I_NTP2,
    I_NTP3,
    I_NTP4,
    I_NTPPERIOD,
    I_TIMEZONE,
    I_HOUR,
    I_MINUTE,
    I_SECOND,
    I_XIVELY,
    I_XIVELYAPIKEY,
    I_XIVELYFEED

};

/*
 * Sensor Template
 */

enum CONTROLLER_TEMPLATE
{
    C_COLOR,
    C_INAME,
    C_NAME,
    C_LINK
};

/*
 * Sensor Template
 */

enum SENSOR_TEMPLATE
{
    S_COLOR,
    S_INAME,
    S_NAME,
    S_VALUE,
    S_IXIVELYCHANNEL,
    S_XIVELYCHANNEL,
    S_LINK
};

/*
 * Actuator Template
 */

enum ACTUATOR_TEMPLATE
{
    A_COLOR,
    A_IACTUATOR,
    A_ACTUATORNAME,
    A_CSELECT,
    A_COPTIONS,
    A_LSELECT,
    A_LOPTIONS,
    A_SSELECT,
    A_SOPTIONS,
    A_LINK
};

/*
 * Main Template
 */

enum MAIN_TEMPLATE
{
    M_HOUR, M_MINUTE, M_SECOND, M_CONTROLLER, M_SENSOR, M_ACTUATOR
};

#include <avr/pgmspace.h>

/*
 * Global Literals
 */
extern const char pgm_hour[];
extern const char pgm_minute[];
extern const char pgm_second[];
extern const char pgm_link[];
extern const char pgm_name[];
extern const char pgm_iname[];
extern const char pgm_color[];

/**
 * Main Webpage
 */
extern const char pMFileName[];
extern const uint8_t size_pMFileName;

extern const char pCRowFileName[];
extern const uint8_t size_pCRowFileName;

extern const char pSRowFileName[];
extern const uint8_t size_pSRowFileName;

extern const char pARowFileName[];
extern const uint8_t size_pARowFileName;

extern const char pTemperature[];
extern const char pController[];
extern const char pActuator[];

extern const char* const mainTemplate[];
extern const uint8_t nr_mainTemplate;

extern const char* const sensorTemplate[];
extern const uint8_t nr_sensorTemplate;

extern const char* const controllerTemplate[];
extern const uint8_t nr_controllerTemplate;

extern const char* const actuatorTemplate[];
extern const uint8_t nr_actuatorTemplate;

/*
 * Configuration Webpage
 */
extern const char progConfigTemplateFileName[];
extern const uint16_t size_progConfigTemplateFileName;

extern const char* const configTemplateStrings[];
extern const uint16_t nr_configTemplateStrings;

extern const char* const configInputStrings[];
extern const uint16_t nr_configInputStrings;
