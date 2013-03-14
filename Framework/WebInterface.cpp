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

#include "WebInterface.h"
#include <WebServer.h>
#include <SD.h>
#include <Time.h>
#include "Aquaduino.h"
#include "TemplateParser.h"

static const char progMainTemplateFileName[] PROGMEM = "main.htm";
static const char progMainTemplateStringHour[] PROGMEM = "##HOUR##";
static const char progMainTemplateStringMinute[] PROGMEM = "##MINUTE##";
static const char progMainTemplateStringSecond[] PROGMEM = "##SECOND##";
static const char progMainTemplateStringTemperature[] PROGMEM
= "##TEMPERATURE##";
static const char progMainTemplateStringCR[] PROGMEM = "##CONTROLLERROW##";
static const char progMainTemplateStringAR[] PROGMEM = "##ACTUATORROW##";

static const char progMainTemplateRowFileName[] PROGMEM = "mainrow.htm";
static const char progMainTemplateStringCL[] PROGMEM = "##LINK##";
static const char progMainTemplateStringCN[] PROGMEM = "##NAME##";

static const char* const mainTemplateStrings[] PROGMEM =
    { progMainTemplateStringTemperature, progMainTemplateStringHour,
      progMainTemplateStringMinute, progMainTemplateStringSecond,
      progMainTemplateStringCR, progMainTemplateStringAR };

static const char* const mainTemplateRowStrings[] PROGMEM =
    { progMainTemplateStringCL, progMainTemplateStringCN };

static const char progConfigFileName[] PROGMEM = "CONTROL.CFG";
static const char progTemplateFileName[] PROGMEM = "conf.htm";

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
    T_COLOR,
    T_IACTUATOR,
    T_ACTUATORNAME,
    T_CSELECT,
    T_COPTIONS,
    T_LSELECT,
    T_LOPTIONS,
    T_SSELECT,
    T_SOPTIONS,
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

static const char progTemplateRow[] PROGMEM = "##ACTORROW##";
static const char progTemplateMAC1[] PROGMEM = "##MAC1##";
static const char progTemplateMAC2[] PROGMEM = "##MAC2##";
static const char progTemplateMAC3[] PROGMEM = "##MAC3##";
static const char progTemplateMAC4[] PROGMEM = "##MAC4##";
static const char progTemplateMAC5[] PROGMEM = "##MAC5##";
static const char progTemplateMAC6[] PROGMEM = "##MAC6##";
static const char progTemplateDHCPSelectOption[] PROGMEM
= "##DHCPSELECTOPTION##";
static const char progTemplateIP1[] PROGMEM = "##IP1##";
static const char progTemplateIP2[] PROGMEM = "##IP2##";
static const char progTemplateIP3[] PROGMEM = "##IP3##";
static const char progTemplateIP4[] PROGMEM = "##IP4##";
static const char progTemplateNM1[] PROGMEM = "##NETMASK1##";
static const char progTemplateNM2[] PROGMEM = "##NETMASK2##";
static const char progTemplateNM3[] PROGMEM = "##NETMASK3##";
static const char progTemplateNM4[] PROGMEM = "##NETMASK4##";
static const char progTemplateGW1[] PROGMEM = "##GATEWAY1##";
static const char progTemplateGW2[] PROGMEM = "##GATEWAY2##";
static const char progTemplateGW3[] PROGMEM = "##GATEWAY3##";
static const char progTemplateGW4[] PROGMEM = "##GATEWAY4##";
static const char progTemplateDNS1[] PROGMEM = "##DNS1##";
static const char progTemplateDNS2[] PROGMEM = "##DNS2##";
static const char progTemplateDNS3[] PROGMEM = "##DNS3##";
static const char progTemplateDNS4[] PROGMEM = "##DNS4##";
static const char progTemplateNTPSelectOption[] PROGMEM = "##NTPSELECTOPTION##";
static const char progTemplateNTP1[] PROGMEM = "##NTP1##";
static const char progTemplateNTP2[] PROGMEM = "##NTP2##";
static const char progTemplateNTP3[] PROGMEM = "##NTP3##";
static const char progTemplateNTP4[] PROGMEM = "##NTP4##";
static const char progTemplateNTPPeriod[] PROGMEM = "##NTPPERIOD##";
static const char progTemplateTimeHour[] PROGMEM = "##HOUR##";
static const char progTemplateTimeMinute[] PROGMEM = "##MINUTE##";
static const char progTemplateTimeSecond[] PROGMEM = "##SECOND##";

static const char progTemplateRowFileName[] PROGMEM = "confrow.htm";
static const char progTemplateRowColor[] PROGMEM = "##COLOR##";
static const char progTemplateRowIActuator[] PROGMEM = "##IACTUATOR##";
static const char progTemplateRowActuatorName[] PROGMEM = "##ACTUATORNAME##";
static const char progTemplateRowCSelect[] PROGMEM = "##CSELECT##";
static const char progTemplateRowCOptions[] PROGMEM = "##COPTIONS##";
static const char progTemplateRowLSelect[] PROGMEM = "##LSELECT##";
static const char progTemplateRowLOptions[] PROGMEM = "##LOPTIONS##";
static const char progTemplateRowSSelect[] PROGMEM = "##SSELECT##";
static const char progTemplateRowSOptions[] PROGMEM = "##SOPTIONS##";

static const char* const templateFileString[] PROGMEM =
    { progTemplateRow, progTemplateMAC1, progTemplateMAC2, progTemplateMAC3,
      progTemplateMAC4, progTemplateMAC5, progTemplateMAC6,
      progTemplateDHCPSelectOption, progTemplateIP1, progTemplateIP2,
      progTemplateIP3, progTemplateIP4, progTemplateNM1, progTemplateNM2,
      progTemplateNM3, progTemplateNM4, progTemplateGW1, progTemplateGW2,
      progTemplateGW3, progTemplateGW4, progTemplateDNS1, progTemplateDNS2,
      progTemplateDNS3, progTemplateDNS4, progTemplateNTPSelectOption,
      progTemplateNTP1, progTemplateNTP2, progTemplateNTP3, progTemplateNTP4,
      progTemplateNTPPeriod, progTemplateTimeHour, progTemplateTimeMinute,
      progTemplateTimeSecond };

static const char* const templateRowFileString[] PROGMEM =
    { progTemplateRowColor, progTemplateRowIActuator,
      progTemplateRowActuatorName, progTemplateRowCSelect,
      progTemplateRowCOptions, progTemplateRowLSelect, progTemplateRowLOptions,
      progTemplateRowSSelect, progTemplateRowSOptions };

static const char progInputMAC1[] PROGMEM = "mac1";
static const char progInputMAC2[] PROGMEM = "mac2";
static const char progInputMAC3[] PROGMEM = "mac3";
static const char progInputMAC4[] PROGMEM = "mac4";
static const char progInputMAC5[] PROGMEM = "mac5";
static const char progInputMAC6[] PROGMEM = "mac6";
static const char progInputDHCP[] PROGMEM = "dhcp";
static const char progInputIP1[] PROGMEM = "ip1";
static const char progInputIP2[] PROGMEM = "ip2";
static const char progInputIP3[] PROGMEM = "ip3";
static const char progInputIP4[] PROGMEM = "ip4";
static const char progInputNetmask1[] PROGMEM = "netmask1";
static const char progInputNetmask2[] PROGMEM = "netmask2";
static const char progInputNetmask3[] PROGMEM = "netmask3";
static const char progInputNetmask4[] PROGMEM = "netmask4";
static const char progInputGateway1[] PROGMEM = "gateway1";
static const char progInputGateway2[] PROGMEM = "gateway2";
static const char progInputGateway3[] PROGMEM = "gateway3";
static const char progInputGateway4[] PROGMEM = "gateway4";
static const char progInputDNS1[] PROGMEM = "dns1";
static const char progInputDNS2[] PROGMEM = "dns2";
static const char progInputDNS3[] PROGMEM = "dns3";
static const char progInputDNS4[] PROGMEM = "dns4";
static const char progInputNTP[] PROGMEM = "ntp";
static const char progInputNTP1[] PROGMEM = "ntp1";
static const char progInputNTP2[] PROGMEM = "ntp2";
static const char progInputNTP3[] PROGMEM = "ntp3";
static const char progInputNTP4[] PROGMEM = "ntp4";
static const char progInputNTPPeriod[] PROGMEM = "ntpperiod";
static const char progInputHour[] PROGMEM = "hour";
static const char progInputMinute[] PROGMEM = "minute";
static const char progInputSecond[] PROGMEM = "second";

static const char* const inputStrings[] PROGMEM =
    { progInputMAC1, progInputMAC2, progInputMAC3, progInputMAC4, progInputMAC5,
      progInputMAC6, progInputDHCP, progInputIP1, progInputIP2, progInputIP3,
      progInputIP4, progInputNetmask1, progInputNetmask2, progInputNetmask3,
      progInputNetmask4, progInputGateway1, progInputGateway2,
      progInputGateway3, progInputGateway4, progInputDNS1, progInputDNS2,
      progInputDNS3, progInputDNS4, progInputNTP, progInputNTP1, progInputNTP2,
      progInputNTP3, progInputNTP4, progInputNTPPeriod, progInputHour,
      progInputMinute, progInputSecond };

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    TemplateParser* parser;
    char templateFileString[sizeof(progMainTemplateFileName)];
    char templateRowString[sizeof(progMainTemplateRowFileName)];
    char* replacementStrings[2];
    File templateFile;
    File templateRow;
    int16_t matchIdx;
    Controller* controller;
    Actuator* actuator;

    char url[AQUADUINO_STRING_LENGTH];
    char name[AQUADUINO_STRING_LENGTH];

    parser = aquaduino->getTemplateParser();

    strcpy_P(templateFileString, progMainTemplateFileName);
    strcpy_P(templateRowString, progMainTemplateRowFileName);

    templateFile = SD.open(templateFileString, FILE_READ);
    templateRow = SD.open(templateRowString, FILE_READ);

    if (type != WebServer::HEAD)
    {
        server.httpSuccess();

        matchIdx = 0;

        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      mainTemplateStrings,
                                                      sizeof(mainTemplateStrings) / sizeof(char*),
                                                      &server))
               >= 0)
        {
            switch (matchIdx)
            {
            case 0:
                server.print(aquaduino->getTemperature());
                break;
            case 1:
                server.print(hour());
                break;
            case 2:
                server.print(minute());
                break;
            case 3:
                server.print(second());
                break;
            case 4:
                aquaduino->resetControllerIterator();
                while (aquaduino->getNextController(&controller) != -1)
                {
                    strcpy(url, controller->getURL());
                    strcpy(name, controller->getName());
                    replacementStrings[0] = url;
                    replacementStrings[1] = name;
                    parser->processSingleTemplate(&templateRow,
                                                  mainTemplateRowStrings,
                                                  replacementStrings,
                                                  sizeof(mainTemplateRowStrings) / sizeof(char*),
                                                  &server);
                }
                break;
            case 5:
                aquaduino->resetActuatorIterator();
                while (aquaduino->getNextActuator(&actuator) != -1)
                {
                    strcpy(url, actuator->getURL());
                    strcpy(name, actuator->getName());
                    replacementStrings[0] = url;
                    replacementStrings[1] = name;
                    parser->processSingleTemplate(&templateRow,
                                                  mainTemplateRowStrings,
                                                  replacementStrings,
                                                  sizeof(mainTemplateRowStrings) / sizeof(char*),
                                                  &server);
                }
                break;
            }
        }
    }
    templateFile.close();
    templateRow.close();
}

void printActuatorTable(WebServer* server)
{
    char templateRowFileName[sizeof(progTemplateRowFileName)];
    File templateRowFile;
    int i, j;
    int16_t matchIdx = 0;
    TemplateParser* parser;
    char actuatorID[5], controllerID[5], lockedID[5], stateID[5];

    Actuator* currentActuator;
    Controller* currentController;

    parser = aquaduino->getTemplateParser();

    strcpy_P(templateRowFileName, progTemplateRowFileName);

    aquaduino->resetActuatorIterator();

    while ((i = aquaduino->getNextActuator(&currentActuator)) != -1)
    {
        templateRowFile = SD.open(templateRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateRowFile,
                                                      templateRowFileString,
                                                      sizeof(templateRowFileString) / sizeof(char*),
                                                      server))
               != -1)
        {

            switch (matchIdx)
            {
            case T_COLOR:
                if (i % 2 == 0)
                {
                    server->print("#FFFFFF");
                }
                else
                {
                    server->print("#99CCFF");
                }
                break;
            case T_IACTUATOR:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            case T_ACTUATORNAME:
                server->print(currentActuator->getName());
                break;
            case T_CSELECT:
                controllerID[0] = 'C';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case T_COPTIONS:
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
            case T_LSELECT:
                lockedID[0] = 'L';
                itoa(i, &lockedID[1], 10);
                server->print(lockedID);
                break;
            case T_LOPTIONS:
                parser->optionListItem("Unlocked", "0", 0, server);
                parser->optionListItem("Locked",
                                       "1",
                                       currentActuator->isLocked(),
                                       server);
                break;
            case T_SSELECT:
                stateID[0] = 'S';
                itoa(i, &stateID[1], 10);
                server->print(stateID);
                break;
            case T_SOPTIONS:
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

    char templateFileName[sizeof(progTemplateFileName)];
    strcpy_P(templateFileName, progTemplateFileName);

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
                                                  templateFileString,
                                                  sizeof(templateFileString) / sizeof(char*),
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
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_MAC1]))) == 0)
                mac[0] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_MAC2]))) == 0)
                mac[1] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_MAC3]))) == 0)
                mac[2] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_MAC4]))) == 0)
                mac[3] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_MAC5]))) == 0)
                mac[4] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_MAC6]))) == 0)
                mac[5] = strtol(value, NULL, 16);
            else if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_IP1]))) == 0)
                ip[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_IP2])))
                     == 0)
                ip[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_IP3])))
                     == 0)
                ip[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_IP4])))
                     == 0)
                ip[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(inputStrings[I_NETMASK1])))
                == 0)
                netmask[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NETMASK2])))
                     == 0)
                netmask[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NETMASK3])))
                     == 0)
                netmask[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NETMASK4])))
                     == 0)
                netmask[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(inputStrings[I_GATEWAY1])))
                == 0)
                gw[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_GATEWAY2])))
                     == 0)
                gw[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_GATEWAY3])))
                     == 0)
                gw[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_GATEWAY4])))
                     == 0)
                gw[3] = atoi(value);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_DNS1]))) == 0)
                dns[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_DNS2])))
                     == 0)
                dns[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_DNS3])))
                     == 0)
                dns[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_DNS4])))
                     == 0)
                dns[3] = atoi(value);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_NTP1]))) == 0)
                ntp[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NTP2])))
                     == 0)
                ntp[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NTP3])))
                     == 0)
                ntp[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NTP4])))
                     == 0)
                ntp[3] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NTP])))
                     == 0)
                doNTP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_DHCP])))
                     == 0)
                doDHCP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_NTPPERIOD])))
                     == 0)
                ntpperiod = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_HOUR])))
                     == 0)
                hour = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_MINUTE])))
                     == 0)
                minute = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_SECOND])))
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

void controllerDispatchCommand(WebServer &server,
                               WebServer::ConnectionType type, char **url_path,
                               char *url_tail, bool tail_complete)
{
    Controller* controller;
    Actuator* actuator;

    if (type != WebServer::HEAD)
    {
        if (strcmp("config", *url_path) == 0)
        {
            configCmd(&server, type);
            return;
        }

        aquaduino->resetControllerIterator();
        while (aquaduino->getNextController(&controller) != -1)
        {
            if (strcmp(controller->getURL(), *url_path) == 0)
            {
                controller->showWebinterface(&server, type);
                if (type == WebServer::POST)
                {
                    aquaduino->writeConfig(controller);
                }
            }
        }

        aquaduino->resetActuatorIterator();
        while (aquaduino->getNextActuator(&actuator) != -1)
        {
            if (strcmp(actuator->getURL(), *url_path) == 0)
            {
                actuator->showWebinterface(&server, type);
                if (type == WebServer::POST)
                {
                    aquaduino->writeConfig(actuator);
                }
            }
        }
    }
}
