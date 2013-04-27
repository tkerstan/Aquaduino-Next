/*
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
static const char pCRowFileName[] PROGMEM = "mainrow.htm";

static const char pHour[] PROGMEM = "##HOUR##";
static const char pMinute[] PROGMEM = "##MINUTE##";
static const char pSecond[] PROGMEM = "##SECOND##";
static const char pTemperature[] PROGMEM
= "##TEMPERATURE##";
static const char pController[] PROGMEM = "##CONTROLLERROW##";
static const char pActuator[] PROGMEM = "##ACTUATORROW##";
static const char pLink[] PROGMEM = "##LINK##";
static const char pName[] PROGMEM = "##NAME##";

enum {
    M_TEMPERATURE,
    M_HOUR,
    M_MINUTE,
    M_SECOND,
    M_CONTROLLER,
    M_ACTUATOR
};

static const char* const mainTemplate[] PROGMEM =
    { pTemperature, pHour, pMinute, pSecond, pController, pActuator };

static const char* const controllerTemplate[] PROGMEM =
    { pLink, pName };

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    TemplateParser* parser;
    char fileName[sizeof(pMFileName)];
    char cRowFileName[sizeof(pCRowFileName)];
    char* replacementStrings[2];
    File templateFile;
    File templateRow;
    int16_t matchIdx;
    Controller* controller;
    Actuator* actuator;

    char url[AQUADUINO_STRING_LENGTH];
    char name[AQUADUINO_STRING_LENGTH];

    parser = aquaduino->getTemplateParser();

    strcpy_P(fileName, pMFileName);
    strcpy_P(cRowFileName, pCRowFileName);

    templateFile = SD.open(fileName, FILE_READ);
    templateRow = SD.open(cRowFileName, FILE_READ);

    if (type != WebServer::HEAD)
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
                aquaduino->resetControllerIterator();
                while (aquaduino->getNextController(&controller) != -1)
                {
                    strcpy(url, controller->getURL());
                    strcpy(name, controller->getName());
                    replacementStrings[0] = url;
                    replacementStrings[1] = name;
                    parser->processSingleTemplate(&templateRow,
                                                  controllerTemplate,
                                                  replacementStrings,
                                                  sizeof(controllerTemplate) / sizeof(char*),
                                                  &server);
                }
                break;
            case M_ACTUATOR:
                aquaduino->resetActuatorIterator();
                while (aquaduino->getNextActuator(&actuator) != -1)
                {
                    strcpy(url, actuator->getURL());
                    strcpy(name, actuator->getName());
                    replacementStrings[0] = url;
                    replacementStrings[1] = name;
                    parser->processSingleTemplate(&templateRow,
                                                  controllerTemplate,
                                                  replacementStrings,
                                                  sizeof(controllerTemplate) / sizeof(char*),
                                                  &server);
                }
                break;
            }
        }
    }
    templateFile.close();
    templateRow.close();
}
