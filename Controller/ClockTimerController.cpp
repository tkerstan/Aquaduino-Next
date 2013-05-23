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
static const char progMainRow[] PROGMEM = "##CLOCKTIMERROW##";
static const char progRowIHON[] PROGMEM = "##I_HON##";
static const char progRowIMON[] PROGMEM = "##I_MON##";
static const char progRowIHOFF[] PROGMEM = "##I_HOFF##";
static const char progRowIMOFF[] PROGMEM = "##I_MOFF##";
static const char progRowHON[] PROGMEM = "##HON##";
static const char progRowMON[] PROGMEM = "##MON##";
static const char progRowHOFF[] PROGMEM = "##HOFF##";
static const char progRowMOFF[] PROGMEM = "##MOFF##";

enum
{
    MAIN_URL, MAIN_SELECT, MAIN_ROW
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
    { progMainURL, progMainSelect, progMainRow };

static const char* const rowStrings[] PROGMEM =
    { progRowIHON, progRowIMON, progRowIHOFF, progRowIMOFF, progRowHON,
      progRowMON, progRowHOFF, progRowMOFF };

ClockTimerController::ClockTimerController(const char* name) :
        Controller(name)
{
    m_Type = CONTROLLER_CLOCKTIMER;
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

int8_t ClockTimerController::showWebinterface(WebServer* server,
                                              WebServer::ConnectionType type,
                                              char* url)
{
    static int8_t selectedTimer = 0;

    TemplateParser* parser;
    char templateFileName[sizeof(progMainTemplate)];
    char templateRowFileName[sizeof(progRowTemplate)];
    File mainTemplateFile;
    File rowTemplateFile;
    int16_t matchIdx = 0;

    int8_t myActuators[MAX_ACTUATORS];
    const char* actuatorNames[MAX_ACTUATORS];
    char actuatorValArray[MAX_ACTUATORS][3];
    char* actuatorValuePointers[MAX_ACTUATORS];

    char timerNameValArray[MAX_CLOCKTIMERS][3];
    char* timerNameValPointers[MAX_CLOCKTIMERS];

    int8_t actuators = 0;
    int8_t i = 0;

    strcpy_P(templateFileName, progMainTemplate);
    strcpy_P(templateRowFileName, progRowTemplate);

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strstr(url, "select") != 0)
            {
                if(strcmp(name, "timer") == 0)
                {
                    selectedTimer = atoi(value);
                }
            }

        } while (repeat);

        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = aquaduino->getTemplateParser();
        mainTemplateFile = SD.open(templateFileName, FILE_READ);
        actuators = aquaduino->getAssignedActuatorIDs(this,
                                                      myActuators,
                                                      MAX_ACTUATORS);

        for (i = 0; i < MAX_CLOCKTIMERS; i++)
        {
            itoa(i, timerNameValArray[i], 10);
            timerNameValPointers[i] = timerNameValArray[i];
        }

        for (i = 0; i < actuators; i++)
        {
            actuatorNames[i] =
                    aquaduino->getActuator(myActuators[i])->getName();

            itoa(myActuators[i], actuatorValArray[i], 10);
            actuatorValuePointers[i] = actuatorValArray[i];
        }

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
            case MAIN_ROW:
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
                            server->print(i * 5);
                            break;
                        case CTR_IMON:
                            server->print(i * 5 + 1);
                            break;
                        case CTR_IHOFF:
                            server->print(i * 5 + 2);
                            break;
                        case CTR_IMOFF:
                            server->print(i * 5 + 3);
                            break;
                        case CTR_HON:
                            server->print(myTimers[selectedTimer].getHourOn(i));
                            break;
                        case CTR_MON:
                            server->print(myTimers[selectedTimer].getMinuteOn(i));
                            break;
                        case CTR_HOFF:
                            server->print(myTimers[selectedTimer].getHourOff(i));
                            break;
                        case CTR_MOFF:
                            server->print(myTimers[selectedTimer].getMinuteOff(i));
                            break;
                        }
                    }
                    rowTemplateFile.close();
                }
                break;
            }
        }

        mainTemplateFile.close();
    }
    return true;

}
