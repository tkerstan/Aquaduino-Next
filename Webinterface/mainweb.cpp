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
#include <TemplateParser.h>

static const char pMFileName[] PROGMEM = "main.htm";
static const char pCRowFileName[] PROGMEM = "maincrow.htm";
static const char pARowFileName[] PROGMEM = "mainarow.htm";

/*
 * Shared Keywords
 */
static const char pLink[] PROGMEM = "##LINK##";
static const char pName[] PROGMEM = "##NAME##";
static const char pColor[] PROGMEM = "##COLOR##";

/*
 * Main Template
 */

enum MAIN_TEMPLATE
{
    M_TEMPERATURE, M_HOUR, M_MINUTE, M_SECOND, M_CONTROLLER, M_ACTUATOR
};

static const char pHour[] PROGMEM = "##HOUR##";
static const char pMinute[] PROGMEM = "##MINUTE##";
static const char pSecond[] PROGMEM = "##SECOND##";
static const char pTemperature[] PROGMEM
= "##TEMPERATURE##";
static const char pController[] PROGMEM = "##CONTROLLERROW##";
static const char pActuator[] PROGMEM = "##ACTUATORROW##";

static const char* const mainTemplate[] PROGMEM =
    { pTemperature, pHour, pMinute, pSecond, pController, pActuator };

/*
 * Controller Template
 */

static const char* const controllerTemplate[] PROGMEM =
    { pLink, pName, pColor };

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

static const char pIActuator[] PROGMEM = "##IACTUATOR##";
static const char pCSelect[] PROGMEM = "##CSELECT##";
static const char pCOptions[] PROGMEM = "##COPTIONS##";
static const char pLSelect[] PROGMEM = "##LSELECT##";
static const char pLOptions[] PROGMEM = "##LOPTIONS##";
static const char pSSelect[] PROGMEM = "##SSELECT##";
static const char pSOptions[] PROGMEM = "##SOPTIONS##";

static const char* const actuatorTemplate[] PROGMEM =
    { pColor, pIActuator, pName, pCSelect, pCOptions, pLSelect, pLOptions,
      pSSelect, pSOptions, pLink };

/**
 * \brief Prints the actuator table below the main information.
 *
 * Prints the actuator table using the template specified in pARowFileName.
 */
void printActuatorTable(WebServer* server)
{
    int i, j;
    int16_t matchIdx = 0;
    TemplateParser* parser;
    char actuatorID[5], controllerID[5], lockedID[5], stateID[5];
    char aRowFileName[sizeof(pARowFileName)];
    File templateARow;
    Actuator* currentActuator;
    Controller* currentController;

    parser = aquaduino->getTemplateParser();
    strcpy_P(aRowFileName, pARowFileName);

    aquaduino->resetActuatorIterator();

    while ((i = aquaduino->getNextActuator(&currentActuator)) != -1)
    {
        templateARow = SD.open(aRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateARow,
                                                      actuatorTemplate,
                                                      sizeof(actuatorTemplate) / sizeof(char*),
                                                      server))
               != -1)
        {

            switch (matchIdx)
            {
            case A_COLOR:
                if (i % 2 == 0)
                {
                    server->print("#FFFFFF");
                }
                else
                {
                    server->print("#99CCFF");
                }
                break;
            case A_IACTUATOR:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            case A_ACTUATORNAME:
                server->print(currentActuator->getName());
                break;
            case A_CSELECT:
                controllerID[0] = 'C';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case A_COPTIONS:
                aquaduino->resetControllerIterator();
                while ((j = aquaduino->getNextController(&currentController)) != -1)
                {
                    itoa(j, controllerID, 10);
                    parser->selectListOption(currentController->getName(),
                                             controllerID,
                                             currentActuator->getController() == j,
                                             server);
                }
                break;
            case A_LSELECT:
                lockedID[0] = 'L';
                itoa(i, &lockedID[1], 10);
                server->print(lockedID);
                break;
            case A_LOPTIONS:
                parser->selectListOption("Unlocked", "0", 0, server);
                parser->selectListOption("Locked",
                                         "1",
                                         currentActuator->isLocked(),
                                         server);
                break;
            case A_SSELECT:
                stateID[0] = 'S';
                itoa(i, &stateID[1], 10);
                server->print(stateID);
                break;
            case A_SOPTIONS:
                parser->selectListOption("Off", "0", 0, server);
                parser->selectListOption("On",
                                         "1",
                                         currentActuator->isOn(),
                                         server);
                break;
            case A_LINK:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            }
        }
        templateARow.close();
    }

}

/**
 * \brief Prints the controller table below the main information.
 *
 * Prints the controller table using the template specified in pCRowFileName.
 */
void printControllerTable(WebServer* server)
{
    TemplateParser* parser;
    Controller* controller;
    char curl[AQUADUINO_STRING_LENGTH];
    char cname[AQUADUINO_STRING_LENGTH];
    char* replacementStrings[3];
    File templateCRow;
    char cRowFileName[sizeof(pCRowFileName)];

    strcpy_P(cRowFileName, pCRowFileName);
    templateCRow = SD.open(cRowFileName, FILE_READ);

    parser = aquaduino->getTemplateParser();

    aquaduino->resetControllerIterator();
    while (aquaduino->getNextController(&controller) != -1)
    {
        strcpy(curl, controller->getURL());
        strcpy(cname, controller->getName());
        replacementStrings[0] = curl;
        replacementStrings[1] = cname;
        replacementStrings[2] = "99CCFF";
        parser->processSingleTemplate(&templateCRow,
                                      controllerTemplate,
                                      replacementStrings,
                                      sizeof(controllerTemplate) / sizeof(char*),
                                      server);
    }
    templateCRow.close();
}

/**
 * \brief Prints the main web page.
 *
 * Prints the main web page specified in pMFileName.
 */
void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    TemplateParser* parser;
    char fileName[sizeof(pMFileName)];
    File templateFile;
    int16_t matchIdx;
    char name[30], value[30];
    uint16_t actuatorIdx;
    uint16_t controllerIdx;
    Actuator* actuator;
    int8_t repeat = 0;

    parser = aquaduino->getTemplateParser();

    strcpy_P(fileName, pMFileName);

    templateFile = SD.open(fileName, FILE_READ);

    if (type == WebServer::POST)
    {
        while ((repeat = server.readPOSTparam(name, 30, value, 30)) > 0)
        {

            if (name[0] == 'A' && name[1] >= '0' && name[1] <= '9')
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

        aquaduino->resetActuatorIterator();
        while (aquaduino->getNextActuator(&actuator) != -1)
        {
            aquaduino->writeConfig(actuator);
        }

        server.httpSeeOther("/");

    }
    else if (type != WebServer::HEAD)
    {
        server.httpSuccess();

        matchIdx = 0;

        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      mainTemplate,
                                                      sizeof(mainTemplate) / sizeof(char*),
                                                      &server))
               >= 0)
        {
            switch (matchIdx)
            {
            case M_TEMPERATURE:
                server.print(aquaduino->getTemperature());
                break;
            case M_HOUR:
                server.print(hour());
                break;
            case M_MINUTE:
                server.print(minute());
                break;
            case M_SECOND:
                server.print(second());
                break;
            case M_CONTROLLER:
                printControllerTable(&server);
                break;
            case M_ACTUATOR:
                printActuatorTable(&server);
                break;
            }
        }
    }
    templateFile.close();
}
