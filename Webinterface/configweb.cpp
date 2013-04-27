/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
 *
 * This file is part of Aquaduino.
 *
 * Aquaduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Aquaduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aquaduino.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Framework/WebInterface.h>
#include <SD.h>
#include <Time.h>
#include <Aquaduino.h>
#include "TemplateParser.h"

static const char progConfigTemplateFileName[] PROGMEM = "conf.htm";

enum
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
    T_HOUR,
    T_MINUTE,
    T_SECOND
};

enum
{
    T_ROW_COLOR,
    T_ROW_IACTUATOR,
    T_ROW_ACTUATORNAME,
    T_ROW_CSELECT,
    T_ROW_COPTIONS,
    T_ROW_LSELECT,
    T_ROW_LOPTIONS,
    T_ROW_SSELECT,
    T_ROW_SOPTIONS,
};

enum
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
    I_HOUR,
    I_MINUTE,
    I_SECOND,

};

static const char progConfigTemplateRow[] PROGMEM = "##ACTORROW##";
static const char progConfigTemplateMAC1[] PROGMEM = "##MAC1##";
static const char progConfigTemplateMAC2[] PROGMEM = "##MAC2##";
static const char progConfigTemplateMAC3[] PROGMEM = "##MAC3##";
static const char progConfigTemplateMAC4[] PROGMEM = "##MAC4##";
static const char progConfigTemplateMAC5[] PROGMEM = "##MAC5##";
static const char progConfigTemplateMAC6[] PROGMEM = "##MAC6##";
static const char progConfigTemplateDHCPSelectOption[] PROGMEM
= "##DHCPSELECTOPTION##";
static const char progConfigTemplateIP1[] PROGMEM = "##IP1##";
static const char progConfigTemplateIP2[] PROGMEM = "##IP2##";
static const char progConfigTemplateIP3[] PROGMEM = "##IP3##";
static const char progConfigTemplateIP4[] PROGMEM = "##IP4##";
static const char progConfigTemplateNM1[] PROGMEM = "##NETMASK1##";
static const char progConfigTemplateNM2[] PROGMEM = "##NETMASK2##";
static const char progConfigTemplateNM3[] PROGMEM = "##NETMASK3##";
static const char progConfigTemplateNM4[] PROGMEM = "##NETMASK4##";
static const char progConfigTemplateGW1[] PROGMEM = "##GATEWAY1##";
static const char progConfigTemplateGW2[] PROGMEM = "##GATEWAY2##";
static const char progConfigTemplateGW3[] PROGMEM = "##GATEWAY3##";
static const char progConfigTemplateGW4[] PROGMEM = "##GATEWAY4##";
static const char progConfigTemplateDNS1[] PROGMEM = "##DNS1##";
static const char progConfigTemplateDNS2[] PROGMEM = "##DNS2##";
static const char progConfigTemplateDNS3[] PROGMEM = "##DNS3##";
static const char progConfigTemplateDNS4[] PROGMEM = "##DNS4##";
static const char progConfigTemplateNTPSelectOption[] PROGMEM = "##NTPSELECTOPTION##";
static const char progConfigTemplateNTP1[] PROGMEM = "##NTP1##";
static const char progConfigTemplateNTP2[] PROGMEM = "##NTP2##";
static const char progConfigTemplateNTP3[] PROGMEM = "##NTP3##";
static const char progConfigTemplateNTP4[] PROGMEM = "##NTP4##";
static const char progConfigTemplateNTPPeriod[] PROGMEM = "##NTPPERIOD##";
static const char progConfigTemplateTimeHour[] PROGMEM = "##HOUR##";
static const char progConfigTemplateTimeMinute[] PROGMEM = "##MINUTE##";
static const char progConfigTemplateTimeSecond[] PROGMEM = "##SECOND##";

static const char progConfigTemplateRowFileName[] PROGMEM = "confrow.htm";
static const char progConfigTemplateRowColor[] PROGMEM = "##COLOR##";
static const char progConfigTemplateRowIActuator[] PROGMEM = "##IACTUATOR##";
static const char progConfigTemplateRowActuatorName[] PROGMEM = "##ACTUATORNAME##";
static const char progConfigTemplateRowCSelect[] PROGMEM = "##CSELECT##";
static const char progConfigTemplateRowCOptions[] PROGMEM = "##COPTIONS##";
static const char progConfigTemplateRowLSelect[] PROGMEM = "##LSELECT##";
static const char progConfigTemplateRowLOptions[] PROGMEM = "##LOPTIONS##";
static const char progConfigTemplateRowSSelect[] PROGMEM = "##SSELECT##";
static const char progConfigTemplateRowSOptions[] PROGMEM = "##SOPTIONS##";

static const char* const configTemplateStrings[] PROGMEM =
    { progConfigTemplateRow, progConfigTemplateMAC1, progConfigTemplateMAC2, progConfigTemplateMAC3,
      progConfigTemplateMAC4, progConfigTemplateMAC5, progConfigTemplateMAC6,
      progConfigTemplateDHCPSelectOption, progConfigTemplateIP1, progConfigTemplateIP2,
      progConfigTemplateIP3, progConfigTemplateIP4, progConfigTemplateNM1, progConfigTemplateNM2,
      progConfigTemplateNM3, progConfigTemplateNM4, progConfigTemplateGW1, progConfigTemplateGW2,
      progConfigTemplateGW3, progConfigTemplateGW4, progConfigTemplateDNS1, progConfigTemplateDNS2,
      progConfigTemplateDNS3, progConfigTemplateDNS4, progConfigTemplateNTPSelectOption,
      progConfigTemplateNTP1, progConfigTemplateNTP2, progConfigTemplateNTP3, progConfigTemplateNTP4,
      progConfigTemplateNTPPeriod, progConfigTemplateTimeHour, progConfigTemplateTimeMinute,
      progConfigTemplateTimeSecond };

static const char* const configTemplateRowStrings[] PROGMEM =
    { progConfigTemplateRowColor, progConfigTemplateRowIActuator,
      progConfigTemplateRowActuatorName, progConfigTemplateRowCSelect,
      progConfigTemplateRowCOptions, progConfigTemplateRowLSelect, progConfigTemplateRowLOptions,
      progConfigTemplateRowSSelect, progConfigTemplateRowSOptions };

static const char progConfigInputMAC1[] PROGMEM = "mac1";
static const char progConfigInputMAC2[] PROGMEM = "mac2";
static const char progConfigInputMAC3[] PROGMEM = "mac3";
static const char progConfigInputMAC4[] PROGMEM = "mac4";
static const char progConfigInputMAC5[] PROGMEM = "mac5";
static const char progConfigInputMAC6[] PROGMEM = "mac6";
static const char progConfigInputDHCP[] PROGMEM = "dhcp";
static const char progConfigInputIP1[] PROGMEM = "ip1";
static const char progConfigInputIP2[] PROGMEM = "ip2";
static const char progConfigInputIP3[] PROGMEM = "ip3";
static const char progConfigInputIP4[] PROGMEM = "ip4";
static const char progConfigInputNetmask1[] PROGMEM = "netmask1";
static const char progConfigInputNetmask2[] PROGMEM = "netmask2";
static const char progConfigInputNetmask3[] PROGMEM = "netmask3";
static const char progConfigInputNetmask4[] PROGMEM = "netmask4";
static const char progConfigInputGateway1[] PROGMEM = "gateway1";
static const char progConfigInputGateway2[] PROGMEM = "gateway2";
static const char progConfigInputGateway3[] PROGMEM = "gateway3";
static const char progConfigInputGateway4[] PROGMEM = "gateway4";
static const char progConfigInputDNS1[] PROGMEM = "dns1";
static const char progConfigInputDNS2[] PROGMEM = "dns2";
static const char progConfigInputDNS3[] PROGMEM = "dns3";
static const char progConfigInputDNS4[] PROGMEM = "dns4";
static const char progConfigInputNTP[] PROGMEM = "ntp";
static const char progConfigInputNTP1[] PROGMEM = "ntp1";
static const char progConfigInputNTP2[] PROGMEM = "ntp2";
static const char progConfigInputNTP3[] PROGMEM = "ntp3";
static const char progConfigInputNTP4[] PROGMEM = "ntp4";
static const char progConfigInputNTPPeriod[] PROGMEM = "ntpperiod";
static const char progConfigInputHour[] PROGMEM = "hour";
static const char progConfigInputMinute[] PROGMEM = "minute";
static const char progConfigInputSecond[] PROGMEM = "second";

static const char* const configInputStrings[] PROGMEM =
    { progConfigInputMAC1, progConfigInputMAC2, progConfigInputMAC3, progConfigInputMAC4, progConfigInputMAC5,
      progConfigInputMAC6, progConfigInputDHCP, progConfigInputIP1, progConfigInputIP2, progConfigInputIP3,
      progConfigInputIP4, progConfigInputNetmask1, progConfigInputNetmask2, progConfigInputNetmask3,
      progConfigInputNetmask4, progConfigInputGateway1, progConfigInputGateway2,
      progConfigInputGateway3, progConfigInputGateway4, progConfigInputDNS1, progConfigInputDNS2,
      progConfigInputDNS3, progConfigInputDNS4, progConfigInputNTP, progConfigInputNTP1, progConfigInputNTP2,
      progConfigInputNTP3, progConfigInputNTP4, progConfigInputNTPPeriod, progConfigInputHour,
      progConfigInputMinute, progConfigInputSecond };

void printActuatorTable(WebServer* server)
{
    char templateRowFileName[sizeof(progConfigTemplateRowFileName)];
    File templateRowFile;
    int i, j;
    int16_t matchIdx = 0;
    TemplateParser* parser;
    char actuatorID[5], controllerID[5], lockedID[5], stateID[5];

    Actuator* currentActuator;
    Controller* currentController;

    parser = aquaduino->getTemplateParser();

    strcpy_P(templateRowFileName, progConfigTemplateRowFileName);

    aquaduino->resetActuatorIterator();

    while ((i = aquaduino->getNextActuator(&currentActuator)) != -1)
    {
        templateRowFile = SD.open(templateRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateRowFile,
                                                      configTemplateRowStrings,
                                                      sizeof(configTemplateRowStrings) / sizeof(char*),
                                                      server))
               != -1)
        {

            switch (matchIdx)
            {
            case T_ROW_COLOR:
                if (i % 2 == 0)
                {
                    server->print("#FFFFFF");
                }
                else
                {
                    server->print("#99CCFF");
                }
                break;
            case T_ROW_IACTUATOR:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            case T_ROW_ACTUATORNAME:
                server->print(currentActuator->getName());
                break;
            case T_ROW_CSELECT:
                controllerID[0] = 'C';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case T_ROW_COPTIONS:
                aquaduino->resetControllerIterator();
                while ((j = aquaduino->getNextController(&currentController)) != -1)
                {
                    itoa(j, controllerID, 10);
                    parser->optionListItem(currentController->getName(),
                                           controllerID,
                                           currentActuator->getController() == j,
                                           server);
                }
                break;
            case T_ROW_LSELECT:
                lockedID[0] = 'L';
                itoa(i, &lockedID[1], 10);
                server->print(lockedID);
                break;
            case T_ROW_LOPTIONS:
                parser->optionListItem("Unlocked", "0", 0, server);
                parser->optionListItem("Locked",
                                       "1",
                                       currentActuator->isLocked(),
                                       server);
                break;
            case T_ROW_SSELECT:
                stateID[0] = 'S';
                itoa(i, &stateID[1], 10);
                server->print(stateID);
                break;
            case T_ROW_SOPTIONS:
                parser->optionListItem("Off", "0", 0, server);
                parser->optionListItem("On",
                                       "1",
                                       currentActuator->isOn(),
                                       server);
                break;
            }
        }
        templateRowFile.close();
    }

}

void printTopLevelTemplate(WebServer* server)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx = 0;
    uint8_t mac[6];
    IPAddress* ip, *netmask, *dns, *gw, *ntp;

    char templateFileName[sizeof(progConfigTemplateFileName)];
    strcpy_P(templateFileName, progConfigTemplateFileName);

    parser = aquaduino->getTemplateParser();
    templateFile = SD.open(templateFileName, FILE_READ);

    aquaduino->getMAC(mac);
    ip = aquaduino->getIP();
    netmask = aquaduino->getNetmask();
    gw = aquaduino->getGateway();
    dns = aquaduino->getDNS();
    ntp = aquaduino->getNTP();

    while ((matchIdx =
            parser->processTemplateUntilNextMatch(&templateFile,
                                                  configTemplateStrings,
                                                  sizeof(configTemplateStrings) / sizeof(char*),
                                                  server))
           >= 0)
    {
        switch (matchIdx)
        {
        case T_ACTORROW:
            printActuatorTable(server);
            break;
        case T_MAC1:
            server->print(mac[0], HEX);
            break;
        case T_MAC2:
            server->print(mac[1], HEX);
            break;
        case T_MAC3:
            server->print(mac[2], HEX);
            break;
        case T_MAC4:
            server->print(mac[3], HEX);
            break;
        case T_MAC5:
            server->print(mac[4], HEX);
            break;
        case T_MAC6:
            server->print(mac[5], HEX);
            break;
        case T_DHCPSELECTOPTION:
            if (aquaduino->isDHCPEnabled())
            {
                parser->optionListItem("Yes", "1", 1, server);
                parser->optionListItem("No", "0", 0, server);
            }
            else
            {
                parser->optionListItem("Yes", "1", 0, server);
                parser->optionListItem("No", "0", 1, server);
            }
            break;
        case T_IP1:
            server->print((*ip)[0]);
            break;
        case T_IP2:
            server->print((*ip)[1]);
            break;
        case T_IP3:
            server->print((*ip)[2]);
            break;
        case T_IP4:
            server->print((*ip)[3]);
            break;
        case T_NETMASK1:
            server->print((*netmask)[0]);
            break;
        case T_NETMASK2:
            server->print((*netmask)[1]);
            break;
        case T_NETMASK3:
            server->print((*netmask)[2]);
            break;
        case T_NETMASK4:
            server->print((*netmask)[3]);
            break;
        case T_GATEWAY1:
            server->print((*gw)[0]);
            break;
        case T_GATEWAY2:
            server->print((*gw)[1]);
            break;
        case T_GATEWAY3:
            server->print((*gw)[2]);
            break;
        case T_GATEWAY4:
            server->print((*gw)[3]);
            break;
        case T_DNS1:
            server->print((*dns)[0]);
            break;
        case T_DNS2:
            server->print((*dns)[1]);
            break;
        case T_DNS3:
            server->print((*dns)[2]);
            break;
        case T_DNS4:
            server->print((*dns)[3]);
            break;
        case T_NTPSELECTOPTION:
            if (aquaduino->isNTPEnabled())
            {
                parser->optionListItem("Yes", "1", 1, server);
                parser->optionListItem("No", "0", 0, server);
            }
            else
            {
                parser->optionListItem("Yes", "1", 0, server);
                parser->optionListItem("No", "0", 1, server);
            }
            break;
        case T_NTP1:
            server->print((*ntp)[0]);
            break;
        case T_NTP2:
            server->print((*ntp)[1]);
            break;
        case T_NTP3:
            server->print((*ntp)[2]);
            break;
        case T_NTP4:
            server->print((*ntp)[3]);
            break;
        case T_NTPPERIOD:
            server->print(aquaduino->getNtpSyncInterval());
            break;
        case T_HOUR:
            server->print(hour());
            break;
        case T_MINUTE:
            server->print(minute());
            break;
        case T_SECOND:
            server->print(second());
            break;
        }
    }
    templateFile.close();
}

int8_t configCmd(WebServer* server, WebServer::ConnectionType type)
{
    int8_t repeat;
    uint8_t mac[6];
    char name[30], value[30];
    IPAddress ip, netmask, gw, dns, ntp;
    int8_t doNTP = 0, doDHCP = 0;
    uint16_t ntpperiod = 5;
    uint16_t actuatorIdx;
    uint16_t controllerIdx;
    int8_t hour, minute, second;
    Actuator* actuator;

    /*
     * TODO: Implement security checks when processing POST parameters
     */
    if (type == WebServer::POST)
    {
        while ((repeat = server->readPOSTparam(name, 30, value, 30)) > 0)
        {
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_MAC1]))) == 0)
                mac[0] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_MAC2]))) == 0)
                mac[1] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_MAC3]))) == 0)
                mac[2] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_MAC4]))) == 0)
                mac[3] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_MAC5]))) == 0)
                mac[4] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_MAC6]))) == 0)
                mac[5] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_IP1]))) == 0)
                ip[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP2])))
                     == 0)
                ip[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP3])))
                     == 0)
                ip[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP4])))
                     == 0)
                ip[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK1])))
                == 0)
                netmask[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK2])))
                     == 0)
                netmask[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK3])))
                     == 0)
                netmask[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK4])))
                     == 0)
                netmask[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY1])))
                == 0)
                gw[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY2])))
                     == 0)
                gw[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY3])))
                     == 0)
                gw[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY4])))
                     == 0)
                gw[3] = atoi(value);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_DNS1]))) == 0)
                dns[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DNS2])))
                     == 0)
                dns[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DNS3])))
                     == 0)
                dns[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DNS4])))
                     == 0)
                dns[3] = atoi(value);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(configInputStrings[I_NTP1]))) == 0)
                ntp[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP2])))
                     == 0)
                ntp[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP3])))
                     == 0)
                ntp[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP4])))
                     == 0)
                ntp[3] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP])))
                     == 0)
                doNTP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DHCP])))
                     == 0)
                doDHCP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTPPERIOD])))
                     == 0)
                ntpperiod = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_HOUR])))
                     == 0)
                hour = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MINUTE])))
                     == 0)
                minute = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_SECOND])))
                     == 0)
                second = atoi(value);
            else if (name[0] == 'A' && name[1] >= '0' && name[1] <= '9')
            {
                actuatorIdx = atoi(&name[1]);
                aquaduino->getActuator(actuatorIdx)->setName(value);
            }
            else if (name[0] == 'C' && name[1] >= '0' && name[1] <= '9')
            {
                actuatorIdx = atoi(&name[1]);
                controllerIdx = atoi(value);
                aquaduino->getActuator(actuatorIdx)->setController(controllerIdx);
            }
            else if (name[0] == 'L' && name[1] >= '0' && name[1] <= '9')
            {
                actuatorIdx = atoi(&name[1]);
                if (atoi(value) == 1)
                    aquaduino->getActuator(actuatorIdx)->lock();
                else
                    aquaduino->getActuator(actuatorIdx)->unlock();
            }
            else if (name[0] == 'S' && name[1] >= '0' && name[1] <= '9')
            {
                actuatorIdx = atoi(&name[1]);
                if ((atoi(value) == 1) && (aquaduino->getActuator(actuatorIdx)->getController()
                        == -1))
                    aquaduino->getActuator(actuatorIdx)->on();
                else
                    aquaduino->getActuator(actuatorIdx)->off();
            }

        }

        aquaduino->setMAC(mac);
        aquaduino->setIP(&ip);
        aquaduino->setNetmask(&netmask);
        aquaduino->setGateway(&gw);
        aquaduino->setDNS(&dns);
        aquaduino->setNTP(&ntp);

        if (doDHCP)
            aquaduino->enableDHCP();
        else
            aquaduino->disableDHCP();

        if (doNTP)
            aquaduino->enableNTP();
        else
        {
            aquaduino->disableNTP();
            aquaduino->setTime(hour, minute, second, 0, 0, 0);
        }

        aquaduino->setNtpSyncInterval(ntpperiod);

        aquaduino->writeConfig(aquaduino);

        aquaduino->resetActuatorIterator();
        while (aquaduino->getNextActuator(&actuator) != -1)
        {
            aquaduino->writeConfig(actuator);
        }

        server->httpSeeOther("/config");
    }
    else
    {
        server->httpSuccess();
        printTopLevelTemplate(server);
    }
    return true;
}
