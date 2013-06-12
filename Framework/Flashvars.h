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
    T_SECOND
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
    M_TEMPERATURE, M_HOUR, M_MINUTE, M_SECOND, M_CONTROLLER, M_ACTUATOR
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
extern const char pgm_color[];

/**
 * Main Webpage
 */
extern const char pMFileName[];
extern const uint8_t size_pMFileName;

extern const char pCRowFileName[];
extern const uint8_t size_pCRowFileName;

extern const char pARowFileName[];
extern const uint8_t size_pARowFileName;

extern const char pTemperature[];
extern const char pController[];
extern const char pActuator[];

extern const char* const mainTemplate[];
extern const uint8_t nr_mainTemplate;

extern const char* const controllerTemplate[];
extern const uint8_t nr_controllerTemplate;

extern const char pIActuator[];
extern const char pCSelect[];
extern const char pCOptions[];
extern const char pLSelect[];
extern const char pLOptions[];
extern const char pSSelect[];
extern const char pSOptions[];

extern const char* const actuatorTemplate[];
extern const uint8_t nr_actuatorTemplate;

/*
 * Configuration Webpage
 */
extern const char progConfigTemplateFileName[];
extern const uint16_t size_progConfigTemplateFileName;

extern const char progConfigTemplateRow[];
extern const char progConfigTemplateMAC1[];
extern const char progConfigTemplateMAC2[];
extern const char progConfigTemplateMAC3[];
extern const char progConfigTemplateMAC4[];
extern const char progConfigTemplateMAC5[];
extern const char progConfigTemplateMAC6[];
extern const char progConfigTemplateDHCPSelectOption[];
extern const char progConfigTemplateIP1[];
extern const char progConfigTemplateIP2[];
extern const char progConfigTemplateIP3[];
extern const char progConfigTemplateIP4[];
extern const char progConfigTemplateNM1[];
extern const char progConfigTemplateNM2[];
extern const char progConfigTemplateNM3[];
extern const char progConfigTemplateNM4[];
extern const char progConfigTemplateGW1[];
extern const char progConfigTemplateGW2[];
extern const char progConfigTemplateGW3[];
extern const char progConfigTemplateGW4[];
extern const char progConfigTemplateDNS1[];
extern const char progConfigTemplateDNS2[];
extern const char progConfigTemplateDNS3[];
extern const char progConfigTemplateDNS4[];
extern const char progConfigTemplateNTPSelectOption[];
extern const char progConfigTemplateNTP1[];
extern const char progConfigTemplateNTP2[];
extern const char progConfigTemplateNTP3[];
extern const char progConfigTemplateNTP4[];
extern const char progConfigTemplateNTPPeriod[];
extern const char progConfigTemplateTimezone[];

extern const char* const configTemplateStrings[];
extern const uint16_t nr_configTemplateStrings;

extern const char progConfigInputMAC1[];
extern const char progConfigInputMAC2[];
extern const char progConfigInputMAC3[];
extern const char progConfigInputMAC4[];
extern const char progConfigInputMAC5[];
extern const char progConfigInputMAC6[];
extern const char progConfigInputDHCP[];
extern const char progConfigInputIP1[];
extern const char progConfigInputIP2[];
extern const char progConfigInputIP3[];
extern const char progConfigInputIP4[];
extern const char progConfigInputNetmask1[];
extern const char progConfigInputNetmask2[];
extern const char progConfigInputNetmask3[];
extern const char progConfigInputNetmask4[];
extern const char progConfigInputGateway1[];
extern const char progConfigInputGateway2[];
extern const char progConfigInputGateway3[];
extern const char progConfigInputGateway4[];
extern const char progConfigInputDNS1[];
extern const char progConfigInputDNS2[];
extern const char progConfigInputDNS3[];
extern const char progConfigInputDNS4[];
extern const char progConfigInputNTP[];
extern const char progConfigInputNTP1[];
extern const char progConfigInputNTP2[];
extern const char progConfigInputNTP3[];
extern const char progConfigInputNTP4[];
extern const char progConfigInputNTPPeriod[];
extern const char progConfigInputTimezone[];
extern const char progConfigInputHour[];
extern const char progConfigInputMinute[];
extern const char progConfigInputSecond[];

extern const char* const configInputStrings[];
extern const uint16_t nr_configInputStrings;
