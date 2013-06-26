#include <avr/pgmspace.h>

/*
 * Global Literals
 */
extern const char pgm_hour[] PROGMEM = "##HOUR##";
extern const char pgm_minute[] PROGMEM = "##MINUTE##";
extern const char pgm_second[] PROGMEM = "##SECOND##";
extern const char pgm_iname[] PROGMEM = "##INAME##";
extern const char pgm_link[] PROGMEM = "##LINK##";
extern const char pgm_name[] PROGMEM = "##NAME##";
extern const char pgm_value[] PROGMEM = "##VALUE##";
extern const char pgm_color[] PROGMEM = "##COLOR##";

/**
 * Main Webpage
 */
extern const char pMFileName[] PROGMEM = "main.htm";
extern const uint8_t size_pMFileName = sizeof(pMFileName);

extern const char pCRowFileName[] PROGMEM = "maincrow.htm";
extern const uint8_t size_pCRowFileName = sizeof(pCRowFileName);

extern const char pSRowFileName[] PROGMEM = "mainsrow.htm";
extern const uint8_t size_pSRowFileName = sizeof(pSRowFileName);

extern const char pARowFileName[] PROGMEM = "mainarow.htm";
extern const uint8_t size_pARowFileName = sizeof(pARowFileName);

extern const char pController[] PROGMEM = "##CONTROLLERROW##";
extern const char pActuator[] PROGMEM = "##ACTUATORROW##";
extern const char pSensor[] PROGMEM = "##SENSORROW##";

extern const char* const mainTemplate[] PROGMEM =
    { pgm_hour, pgm_minute, pgm_second, pController, pSensor, pActuator };
extern const uint8_t nr_mainTemplate = sizeof(mainTemplate) / sizeof(char*);

extern const char* const controllerTemplate[] PROGMEM =
    { pgm_color, pgm_iname, pgm_name, pgm_link };
extern const uint8_t nr_controllerTemplate = sizeof(controllerTemplate)
        / sizeof(char*);

extern const char* const sensorTemplate[] PROGMEM =
    { pgm_color, pgm_iname, pgm_name, pgm_value, pgm_link };
extern const uint8_t nr_sensorTemplate = sizeof(sensorTemplate) / sizeof(char*);

extern const char pCSelect[] PROGMEM = "##CSELECT##";
extern const char pCOptions[] PROGMEM = "##COPTIONS##";
extern const char pLSelect[] PROGMEM = "##LSELECT##";
extern const char pLOptions[] PROGMEM = "##LOPTIONS##";
extern const char pSSelect[] PROGMEM = "##SSELECT##";
extern const char pSOptions[] PROGMEM = "##SOPTIONS##";

extern const char* const actuatorTemplate[] PROGMEM =
    { pgm_color, pgm_iname, pgm_name, pCSelect, pCOptions, pLSelect, pLOptions,
      pSSelect, pSOptions, pgm_link };
extern const uint8_t nr_actuatorTemplate = sizeof(actuatorTemplate)
        / sizeof(char*);

/*
 * Configuration Webpage
 */
extern const char progConfigTemplateFileName[] PROGMEM = "conf.htm";
extern const uint16_t size_progConfigTemplateFileName =
        sizeof(progConfigTemplateFileName);

extern const char progConfigTemplateRow[] PROGMEM = "##ACTORROW##";
extern const char progConfigTemplateMAC1[] PROGMEM = "##MAC1##";
extern const char progConfigTemplateMAC2[] PROGMEM = "##MAC2##";
extern const char progConfigTemplateMAC3[] PROGMEM = "##MAC3##";
extern const char progConfigTemplateMAC4[] PROGMEM = "##MAC4##";
extern const char progConfigTemplateMAC5[] PROGMEM = "##MAC5##";
extern const char progConfigTemplateMAC6[] PROGMEM = "##MAC6##";
extern const char progConfigTemplateDHCPSelectOption[] PROGMEM
= "##DHCPSELECTOPTION##";
extern const char progConfigTemplateIP1[] PROGMEM = "##IP1##";
extern const char progConfigTemplateIP2[] PROGMEM = "##IP2##";
extern const char progConfigTemplateIP3[] PROGMEM = "##IP3##";
extern const char progConfigTemplateIP4[] PROGMEM = "##IP4##";
extern const char progConfigTemplateNM1[] PROGMEM = "##NETMASK1##";
extern const char progConfigTemplateNM2[] PROGMEM = "##NETMASK2##";
extern const char progConfigTemplateNM3[] PROGMEM = "##NETMASK3##";
extern const char progConfigTemplateNM4[] PROGMEM = "##NETMASK4##";
extern const char progConfigTemplateGW1[] PROGMEM = "##GATEWAY1##";
extern const char progConfigTemplateGW2[] PROGMEM = "##GATEWAY2##";
extern const char progConfigTemplateGW3[] PROGMEM = "##GATEWAY3##";
extern const char progConfigTemplateGW4[] PROGMEM = "##GATEWAY4##";
extern const char progConfigTemplateDNS1[] PROGMEM = "##DNS1##";
extern const char progConfigTemplateDNS2[] PROGMEM = "##DNS2##";
extern const char progConfigTemplateDNS3[] PROGMEM = "##DNS3##";
extern const char progConfigTemplateDNS4[] PROGMEM = "##DNS4##";
extern const char progConfigTemplateNTPSelectOption[] PROGMEM
= "##NTPSELECTOPTION##";
extern const char progConfigTemplateNTP1[] PROGMEM = "##NTP1##";
extern const char progConfigTemplateNTP2[] PROGMEM = "##NTP2##";
extern const char progConfigTemplateNTP3[] PROGMEM = "##NTP3##";
extern const char progConfigTemplateNTP4[] PROGMEM = "##NTP4##";
extern const char progConfigTemplateNTPPeriod[] PROGMEM = "##NTPPERIOD##";
extern const char progConfigTemplateTimezone[] PROGMEM = "##TIMEZONE##";
extern const char progConfigTemplateFreeRAM[] PROGMEM = "##FREERAM##";

extern const char* const configTemplateStrings[] PROGMEM =
    { progConfigTemplateRow, progConfigTemplateMAC1, progConfigTemplateMAC2,
      progConfigTemplateMAC3, progConfigTemplateMAC4, progConfigTemplateMAC5,
      progConfigTemplateMAC6, progConfigTemplateDHCPSelectOption,
      progConfigTemplateIP1, progConfigTemplateIP2, progConfigTemplateIP3,
      progConfigTemplateIP4, progConfigTemplateNM1, progConfigTemplateNM2,
      progConfigTemplateNM3, progConfigTemplateNM4, progConfigTemplateGW1,
      progConfigTemplateGW2, progConfigTemplateGW3, progConfigTemplateGW4,
      progConfigTemplateDNS1, progConfigTemplateDNS2, progConfigTemplateDNS3,
      progConfigTemplateDNS4, progConfigTemplateNTPSelectOption,
      progConfigTemplateNTP1, progConfigTemplateNTP2, progConfigTemplateNTP3,
      progConfigTemplateNTP4, progConfigTemplateNTPPeriod,
      progConfigTemplateTimezone, pgm_hour, pgm_minute, pgm_second,
      progConfigTemplateFreeRAM };
extern const uint16_t nr_configTemplateStrings = sizeof(configTemplateStrings)
        / sizeof(char*);

extern const char progConfigInputMAC1[] PROGMEM = "mac1";
extern const char progConfigInputMAC2[] PROGMEM = "mac2";
extern const char progConfigInputMAC3[] PROGMEM = "mac3";
extern const char progConfigInputMAC4[] PROGMEM = "mac4";
extern const char progConfigInputMAC5[] PROGMEM = "mac5";
extern const char progConfigInputMAC6[] PROGMEM = "mac6";
extern const char progConfigInputDHCP[] PROGMEM = "dhcp";
extern const char progConfigInputIP1[] PROGMEM = "ip1";
extern const char progConfigInputIP2[] PROGMEM = "ip2";
extern const char progConfigInputIP3[] PROGMEM = "ip3";
extern const char progConfigInputIP4[] PROGMEM = "ip4";
extern const char progConfigInputNetmask1[] PROGMEM = "netmask1";
extern const char progConfigInputNetmask2[] PROGMEM = "netmask2";
extern const char progConfigInputNetmask3[] PROGMEM = "netmask3";
extern const char progConfigInputNetmask4[] PROGMEM = "netmask4";
extern const char progConfigInputGateway1[] PROGMEM = "gateway1";
extern const char progConfigInputGateway2[] PROGMEM = "gateway2";
extern const char progConfigInputGateway3[] PROGMEM = "gateway3";
extern const char progConfigInputGateway4[] PROGMEM = "gateway4";
extern const char progConfigInputDNS1[] PROGMEM = "dns1";
extern const char progConfigInputDNS2[] PROGMEM = "dns2";
extern const char progConfigInputDNS3[] PROGMEM = "dns3";
extern const char progConfigInputDNS4[] PROGMEM = "dns4";
extern const char progConfigInputNTP[] PROGMEM = "ntp";
extern const char progConfigInputNTP1[] PROGMEM = "ntp1";
extern const char progConfigInputNTP2[] PROGMEM = "ntp2";
extern const char progConfigInputNTP3[] PROGMEM = "ntp3";
extern const char progConfigInputNTP4[] PROGMEM = "ntp4";
extern const char progConfigInputNTPPeriod[] PROGMEM = "ntpperiod";
extern const char progConfigInputTimezone[] PROGMEM = "timezone";
extern const char progConfigInputHour[] PROGMEM = "hour";
extern const char progConfigInputMinute[] PROGMEM = "minute";
extern const char progConfigInputSecond[] PROGMEM = "second";

extern const char* const configInputStrings[] PROGMEM =
    { progConfigInputMAC1, progConfigInputMAC2, progConfigInputMAC3,
      progConfigInputMAC4, progConfigInputMAC5, progConfigInputMAC6,
      progConfigInputDHCP, progConfigInputIP1, progConfigInputIP2,
      progConfigInputIP3, progConfigInputIP4, progConfigInputNetmask1,
      progConfigInputNetmask2, progConfigInputNetmask3, progConfigInputNetmask4,
      progConfigInputGateway1, progConfigInputGateway2, progConfigInputGateway3,
      progConfigInputGateway4, progConfigInputDNS1, progConfigInputDNS2,
      progConfigInputDNS3, progConfigInputDNS4, progConfigInputNTP,
      progConfigInputNTP1, progConfigInputNTP2, progConfigInputNTP3,
      progConfigInputNTP4, progConfigInputNTPPeriod, progConfigInputTimezone,
      progConfigInputHour, progConfigInputMinute, progConfigInputSecond };
extern const uint16_t nr_configInputStrings = sizeof(configInputStrings)
        / sizeof(char*);
