/*
 * Copyright (c) 2012 Timo Kerstan.  All right reserved.
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

#include <Aquaduino.h>
#include "ClockTimerController.h"
#include <Time.h>
#include <SD.h>
#include <TemplateParser.h>

static const char progMainTemplate[] PROGMEM = "clock.htm";
static const char progRowTemplate[] PROGMEM = "clockrow.htm";

static const char progMainURL[] PROGMEM = "##URL##";
static const char progMainSelect[] PROGMEM = "##CLOCKTIMERSELECT##";
static const char progActuatorSelect[] PROGMEM = "##ACTUATORSELECT##";
static const char progMainRow[] PROGMEM = "##CLOCKTIMERROW##";
static const char progRowIHON[] PROGMEM = "##I_HON##";
static const char progRowIMON[] PROGMEM = "##I_MON##";
static const char progRowIHOFF[] PROGMEM = "##I_HOFF##";
static const char progRowIMOFF[] PROGMEM = "##I_MOFF##";
static const char progRowHON[] PROGMEM = "##HON##";
static const char progRowMON[] PROGMEM = "##MON##";
static const char progRowHOFF[] PROGMEM = "##HOFF##";
static const char progRowMOFF[] PROGMEM = "##MOFF##";

static const char progStringTimer[] PROGMEM = "timer";
static const char progStringNone[] PROGMEM = "None";
static const char progStringSelect[] PROGMEM = "select";
static const char progStringActuator[] PROGMEM = "actuator";

enum
{
    MAIN_URL, MAIN_SELECT, MAIN_ACTUATORSELECT, MAIN_ROW
};

enum
{
    CTR_IHON,
    CTR_IMON,
    CTR_IHOFF,
    CTR_IMOFF,
    CTR_HON,
    CTR_MON,
    CTR_HOFF,
    CTR_MOFF,
};

static const char* const mainStrings[] PROGMEM =
    { progMainURL, progMainSelect, progActuatorSelect, progMainRow };

static const char* const rowStrings[] PROGMEM =
    { progRowIHON, progRowIMON, progRowIHOFF, progRowIMOFF, progRowHON,
      progRowMON, progRowHOFF, progRowMOFF };

ClockTimerController::ClockTimerController(const char* name) :
        Controller(name), selectedTimer(0), selectedActuator(0)
{
    int8_t i = 0;
    m_Type = CONTROLLER_CLOCKTIMER;
    for (; i < MAX_CLOCKTIMERS; i++)
    {
        m_ActuatorMapping[i] = -1;
    }
}

uint16_t ClockTimerController::serialize(void* buffer, uint16_t size)
{
    return 0;
}

uint16_t ClockTimerController::deserialize(void* data, uint16_t size)
{
    return 0;
}

int8_t ClockTimerController::run()
{
    return 0;
}

int8_t ClockTimerController::isMapped(int8_t actuatorNr)
{
    int8_t mapped = 0;
    int8_t i = 0;
    for (i = 0; i < MAX_CLOCKTIMERS; i++)
        mapped += m_ActuatorMapping[i] == actuatorNr;
    return mapped;
}

int8_t ClockTimerController::prepareActuatorSelect(
        const char* actuatorNames[MAX_ACTUATORS + 1],
        const char* actuatorValuePointers[MAX_ACTUATORS + 1],
        char actuatorValArray[MAX_ACTUATORS + 1][3])
{
    int8_t i, j;

    int8_t myActuators[MAX_ACTUATORS];
    int8_t actuators = aquaduino->getAssignedActuatorIDs(this,
                                                         myActuators,
                                                         MAX_ACTUATORS);

    actuatorNames[0] = "None";
    actuatorValuePointers[0] = "-1";

    selectedActuator = 0;

    for (i = 1, j = 0; i <= actuators; i++)
    {
        if (m_ActuatorMapping[selectedTimer] == myActuators[i - 1])
        {
            selectedActuator = j + 1;
        }
        if (selectedActuator == j + 1 || !isMapped(myActuators[i - 1]))
        {
            actuatorNames[++j] =
                    aquaduino->getActuator(myActuators[i - 1])->getName();

            itoa(myActuators[i - 1], actuatorValArray[j], 10);
            actuatorValuePointers[j] = actuatorValArray[j];
        }
    }

    return j;
}

int8_t ClockTimerController::printMain(WebServer* server,
                                       WebServer::ConnectionType type,
                                       char* url)
{
    TemplateParser* parser;
    char templateFileName[sizeof(progMainTemplate)];

    File mainTemplateFile;
    int16_t matchIdx = 0;

    const char* actuatorNames[MAX_ACTUATORS + 1];
    char actuatorValArray[MAX_ACTUATORS + 1][3];
    const char* actuatorValuePointers[MAX_ACTUATORS + 1];

    char timerNameValArray[MAX_CLOCKTIMERS][3];
    char* timerNameValPointers[MAX_CLOCKTIMERS];

    int8_t actuators = 0;
    int8_t i = 0;

    strcpy_P(templateFileName, progMainTemplate);

    server->httpSuccess();
    parser = aquaduino->getTemplateParser();
    mainTemplateFile = SD.open(templateFileName, FILE_READ);

    for (i = 0; i < MAX_CLOCKTIMERS; i++)
    {
        itoa(i, timerNameValArray[i], 10);
        timerNameValPointers[i] = timerNameValArray[i];
    }

    actuators = prepareActuatorSelect(actuatorNames,
                                      actuatorValuePointers,
                                      actuatorValArray);

    while ((matchIdx =
            parser->processTemplateUntilNextMatch(&mainTemplateFile,
                                                  mainStrings,
                                                  sizeof(mainStrings) / sizeof(char*),
                                                  server))
           != -1)
    {
        switch (matchIdx)
        {
        case MAIN_URL:
            server->print(getURL());
            server->print("select");
            break;
        case MAIN_SELECT:
            parser->selectList("timer",
                               timerNameValPointers,
                               timerNameValPointers,
                               selectedTimer,
                               sizeof(timerNameValPointers) / sizeof(char*),
                               server);
            break;

        case MAIN_ACTUATORSELECT:
            parser->selectList("actuator",
                               actuatorNames,
                               actuatorValuePointers,
                               selectedActuator,
                               actuators + 1,
                               server);
            break;
        case MAIN_ROW:
            printRow(server, type, url);
            break;
        }
    }

    mainTemplateFile.close();

    return 0;
}

int8_t ClockTimerController::printRow(WebServer* server,
                                      WebServer::ConnectionType type, char* url)
{
    TemplateParser* parser;
    int8_t i;
    File rowTemplateFile;
    int16_t matchIdx = 0;
    char templateRowFileName[sizeof(progRowTemplate)];

    strcpy_P(templateRowFileName, progRowTemplate);

    parser = aquaduino->getTemplateParser();

    for (i = 0; i < CLOCKTIMER_MAX_TIMERS; i++)
    {

        rowTemplateFile = SD.open(templateRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&rowTemplateFile,
                                                      rowStrings,
                                                      sizeof(rowStrings) / sizeof(char*),
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case CTR_IHON:
                server->print(i * 4 + 1);
                break;
            case CTR_IMON:
                server->print(i * 4 + 2);
                break;
            case CTR_IHOFF:
                server->print(i * 4 + 3);
                break;
            case CTR_IMOFF:
                server->print(i * 4 + 4);
                break;
            case CTR_HON:
                server->print(m_Timers[selectedTimer].getHourOn(i));
                break;
            case CTR_MON:
                server->print(m_Timers[selectedTimer].getMinuteOn(i));
                break;
            case CTR_HOFF:
                server->print(m_Timers[selectedTimer].getHourOff(i));
                break;
            case CTR_MOFF:
                server->print(m_Timers[selectedTimer].getMinuteOff(i));
                break;
            }
        }
        rowTemplateFile.close();
    }
    return 0;
}

int8_t ClockTimerController::processPost(WebServer* server,
                                         WebServer::ConnectionType type,
                                         char* url)
{
    int8_t repeat;
    int8_t x, y;
    int8_t input;
    int16_t val;
    char name[16], value[16];
    do
    {
        repeat = server->readPOSTparam(name, 16, value, 16);
        if (strstr_P(url, progStringSelect) != 0)
        {
            if (strcmp_P(name, progStringTimer) == 0)
            {
                selectedTimer = atoi(value);
            }
        }
        else
        {

            if (strcmp_P(name, progStringActuator) == 0)
            {
                val = atoi(value);
                if (val != -1){
                    if (!isMapped (atoi(value)))
                            m_ActuatorMapping[selectedTimer] = atoi(value);
                } else
                {
                    m_ActuatorMapping[selectedTimer] = -1;
                }
            }
            else
            {
                input = atoi(name);
                x = input / 4;
                y = input % 4;
                switch (y)
                {
                case 0:
                    m_Timers[selectedTimer].setMinuteOff(x, atoi(value));

                    break;
                case 1:
                    m_Timers[selectedTimer].setHourOn(x, atoi(value));

                    break;
                case 2:
                    m_Timers[selectedTimer].setMinuteOn(x, atoi(value));

                    break;
                case 3:
                    m_Timers[selectedTimer].setHourOff(x, atoi(value));
                    break;
                }
            }
        }

    } while (repeat);
    return 0;
}

int8_t ClockTimerController::showWebinterface(WebServer* server,
                                              WebServer::ConnectionType type,
                                              char* url)
{

    if (type == WebServer::POST)
    {
        processPost(server, type, url);
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        printMain(server, type, url);
    }
    return true;

}
