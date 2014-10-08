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

#include <Framework/Aquaduino.h>
#include "ClockTimerController.h"
#include <Time.h>
#include <SD.h>

#ifdef FEATURE_WEBIF
#include <TemplateParser.h>
#include <Framework/Flashvars.h>
#endif

/**
 * \brief Constructor
 * \param[in] name The name of the controller.
 *
 * Initializes the mapping of actuators to clocktimers.
 */
ClockTimerController::ClockTimerController(const char* name) :
        Controller(name), m_SelectedTimer(0), m_SelectedActuator(0)
{
    int8_t i = 0;
    m_Type = CONTROLLER_CLOCKTIMER;
    for (; i < MAX_CLOCKTIMERS; i++)
    {
        m_ActuatorMapping[i] = -1;
    }
}

/**
 * \brief Destructor
 *
 * Empty.
 */
ClockTimerController::~ClockTimerController()
{
}

uint16_t ClockTimerController::serialize(Stream* s)
{
    uint8_t i;
    uint8_t clockTimers = MAX_CLOCKTIMERS;

    s->write(clockTimers);

    s->write((uint8_t*)m_ActuatorMapping, sizeof(m_ActuatorMapping));
    for (i = 0; i < MAX_CLOCKTIMERS; i++)
    {
        m_Timers[i].serialize(s);
    }
	return 1;
}

uint16_t ClockTimerController::deserialize(Stream* s)
{
    uint8_t i;
    uint8_t clockTimers = 0;

    clockTimers = (uint8_t) s->read();

    if (clockTimers != MAX_CLOCKTIMERS)
        return 0;

    s->readBytes((char*) m_ActuatorMapping, sizeof(m_ActuatorMapping));

    for (i = 0; i < MAX_CLOCKTIMERS; i++)
    {
        if (m_Timers[i].deserialize(s) == 0)
            return 0;
    }
	return 1;
}

/**
 * \brief This method is regularly triggered by Aquaduino::run.
 *
 * Enables or disables the actuators based on the clocktimers and the
 * internal mapping of actuators to clocktimers.
 */
int8_t ClockTimerController::run()
{
    int8_t i;
    Actuator* actuator;
    Controller* controller;
    for (i = 0; i < MAX_CLOCKTIMERS; i++)
    {
        actuator = __aquaduino->getActuator(m_ActuatorMapping[i]);
        controller = __aquaduino->getController(actuator->getController());
        if (actuator != NULL && controller == this)
        {
            if (m_Timers[i].check())
                actuator->on();
            else
                actuator->off();
        }
    }
    return 0;
}

/**
 * \brief Internal method
 *
 * Checks if an actuator is mapped to a clocktimer
 *
 * \returns 1 if yes, 0 otherwise.
 */
int8_t ClockTimerController::isMapped(int8_t actuatorNr)
{
    int8_t mapped = 0;
    int8_t i = 0;
    for (i = 0; i < MAX_CLOCKTIMERS; i++)
        mapped += m_ActuatorMapping[i] == actuatorNr;
    return mapped;
}

#ifdef FEATURE_WEBIF
/**
 * \brief Internal method
 * \param[out] actuatorNames Array of pointers to the actuator names.
 * \param[out] actuatorValue Array of pointers to the actuator value names.
 * \param[out] actuatorValArray Array of char buffers where the value names are
 *                             stored. Cannot be stored here on the stack.
 *
 * Fills the passed parameters to be usable in the selectList method of
 * the TemplateParser instance.
 *
 * \returns number of available select options
 */
int8_t ClockTimerController::prepareActuatorSelect(
        const char* actuatorNames[MAX_ACTUATORS + 1],
        const char* actuatorValuePointers[MAX_ACTUATORS + 1],
        char actuatorValArray[MAX_ACTUATORS + 1][3])
{
    int8_t i, j;

    int8_t myActuators[MAX_ACTUATORS];
    int8_t actuators = __aquaduino->getAssignedActuatorIDs(this,
                                                         myActuators,
                                                         MAX_ACTUATORS);

    actuatorNames[0] = "None";
    actuatorValuePointers[0] = "-1";

    m_SelectedActuator = 0;

    for (i = 1, j = 0; i <= actuators; i++)
    {
        if (m_ActuatorMapping[m_SelectedTimer] == myActuators[i - 1])
        {
            m_SelectedActuator = j + 1;
        }
        //if (m_SelectedActuator == j + 1 || !isMapped(myActuators[i - 1]))
        actuatorNames[++j] =
                __aquaduino->getActuator(myActuators[i - 1])->getName();
        itoa(myActuators[i - 1], actuatorValArray[j], 10);
        actuatorValuePointers[j] = actuatorValArray[j];
//        }
    }

    return j;
}
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Prints the main page of the WebInterface.
 * \param[in] server Instance of the Webduino webserver
 * \param[in] type Type of the request (GET, POST,...)
 * \param[in] url URL used in request. Allows for subURL decoding.
 */
int8_t ClockTimerController::printMain(WebServer* server,
                                       WebServer::ConnectionType type,
                                       char* url)
{
    TemplateParser* parser;
    char templateFileName[template_clocktimercontroller_fnsize];

    File mainTemplateFile;
    int16_t matchIdx = 0;

    const char* actuatorNames[MAX_ACTUATORS + 1];
    char actuatorValArray[MAX_ACTUATORS + 1][3];
    const char* actuatorValuePointers[MAX_ACTUATORS + 1];

    char timerNameValArray[MAX_CLOCKTIMERS][3];
    char* timerNameValPointers[MAX_CLOCKTIMERS];

    char input_name[20];

    int8_t actuators = 0;
    int8_t i = 0;

    strcpy_P(templateFileName, template_clocktimercontroller_fname);

    server->httpSuccess();
    parser = __aquaduino->getTemplateParser();
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
                                                  template_clocktimercontroller,
                                                  template_clocktimercontroller_elements,
                                                  server))
           != -1)
    {
        switch (matchIdx)
        {
            case CLOCKTIMERCONTROLLER_CNAME:
                server->print(getName());
                break;
            case CLOCKTIMERCONTROLLER_ACTIONURL:
                server->print(getURL());
                server->print(".");
                server->print((__FlashStringHelper *) pgm_url_select);
                break;
            case CLOCKTIMERCONTROLLER_CLOCKTIMERSELECT:
                strcpy_P(input_name, pgm_input_timer);
                parser->selectList(input_name,
                                   timerNameValPointers,
                                   timerNameValPointers,
                                   m_SelectedTimer,
                                   sizeof(timerNameValPointers) / sizeof(char*),
                                   server);
                break;

            case CLOCKTIMERCONTROLLER_ACTUATORSELECT:
                strcpy_P(input_name, pgm_input_actuator);
                parser->selectList(input_name,
                                   actuatorNames,
                                   actuatorValuePointers,
                                   m_SelectedActuator,
                                   actuators + 1,
                                   server);
                break;
            case CLOCKTIMERCONTROLLER_ROW:
                printRow(server, type, url);
                break;
            case CLOCKTIMERCONTROLLER_DOW_NAME:
                server->print((__FlashStringHelper *) pgm_input_dow);
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_MO:
                if (m_Timers[m_SelectedTimer].isMondayEnabled())
                    server->print(F("checked"));
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_TU:
                if (m_Timers[m_SelectedTimer].isTuesdayEnabled())
                server->print(F("checked"));
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_WE:
                if (m_Timers[m_SelectedTimer].isWednesdayEnabled())
                    server->print(F("checked"));
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_TH:
                if (m_Timers[m_SelectedTimer].isThursdayEnabled())
                    server->print(F("checked"));
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_FR:
                if (m_Timers[m_SelectedTimer].isFridayEnabled())
                    server->print(F("checked"));
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_SA:
                if (m_Timers[m_SelectedTimer].isSaturdayEnabled())
                    server->print(F("checked"));
                break;
            case CLOCKTIMERCONTROLLER_CHECKED_SU:
                if (m_Timers[m_SelectedTimer].isSundayEnabled())
                    server->print(F("checked"));
                break;
        }
    }

    mainTemplateFile.close();

    return 0;
}
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Prints the row of a clocktimer entry.
 * \param[in] server Instance of the Webduino webserver
 * \param[in] type Type of the request (GET, POST,...)
 * \param[in] url URL used in request. Allows for subURL decoding.
 */
int8_t ClockTimerController::printRow(WebServer* server,
                                      WebServer::ConnectionType type, char* url)
{
    TemplateParser* parser;
    int8_t i;
    File rowTemplateFile;
    int16_t matchIdx = 0;
    char templateRowFileName[template_clocktimercontroller_row_fnsize];

    strcpy_P(templateRowFileName, template_clocktimercontroller_row_fname);

    parser = __aquaduino->getTemplateParser();

    for (i = 0; i < max_timers; i++)
    {

        rowTemplateFile = SD.open(templateRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&rowTemplateFile,
                                                      template_clocktimercontroller_row,
                                                      template_clocktimercontroller_row_elements,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
                case CLOCKTIMERCONTROLLER_COLOR:
                    if (i % 2 == 0)
                        server->print("#99CCFF");
                    else
                        server->print("#FFFFFF");
                    break;
                case CLOCKTIMERCONTROLLER_HON_NAME:
                    server->print(i * 4 + 1);
                    break;
                case CLOCKTIMERCONTROLLER_HON_VAL:
                    server->print(m_Timers[m_SelectedTimer].getHourOn(i));
                    break;
                case CLOCKTIMERCONTROLLER_HON_SIZE:
                    server->print(3);
                    break;
                case CLOCKTIMERCONTROLLER_HON_MAXLENGTH:
                    server->print(2);
                    break;
                case CLOCKTIMERCONTROLLER_MON_NAME:
                    server->print(i * 4 + 2);
                    break;
                case CLOCKTIMERCONTROLLER_MON_VAL:
                    server->print(m_Timers[m_SelectedTimer].getMinuteOn(i));
                    break;
                case CLOCKTIMERCONTROLLER_MON_SIZE:
                    server->print(3);
                    break;
                case CLOCKTIMERCONTROLLER_MON_MAXLENGTH:
                    server->print(2);
                    break;
                case CLOCKTIMERCONTROLLER_HOFF_NAME:
                    server->print(i * 4 + 3);
                    break;
                case CLOCKTIMERCONTROLLER_HOFF_VAL:
                    server->print(m_Timers[m_SelectedTimer].getHourOff(i));
                    break;
                case CLOCKTIMERCONTROLLER_HOFF_SIZE:
                    server->print(3);
                    break;
                case CLOCKTIMERCONTROLLER_HOFF_MAXLENGTH:
                    server->print(2);
                    break;
                case CLOCKTIMERCONTROLLER_MOFF_NAME:
                    server->print(i * 4 + 4);
                    break;
                case CLOCKTIMERCONTROLLER_MOFF_VAL:
                    server->print(m_Timers[m_SelectedTimer].getMinuteOff(i));
                    break;
                case CLOCKTIMERCONTROLLER_MOFF_SIZE:
                    server->print(3);
                    break;
                case CLOCKTIMERCONTROLLER_MOFF_MAXLENGTH:
                    server->print(2);
                    break;
            }
        }
        rowTemplateFile.close();
    }
    return 0;
}
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Processes the input submitted by a POST request.
 * \param[in] server Instance of the Webduino webserver
 * \param[in] type Type of the request (GET, POST,...)
 * \param[in] url URL used in request. Allows for subURL decoding.
 */
int8_t ClockTimerController::processPost(WebServer* server,
                                         WebServer::ConnectionType type,
                                         char* url)
{
    int8_t repeat;
    int8_t x, y;
    int8_t input;
    int16_t val;
    char name[16], value[16];

    //No SubURL given. The post target is the clocktimer form and
    //not the timer select form. Thus the selected DOW are cleared before and
    //the selected DOW will be extracted in the do while loop.
    if (url == NULL)
        m_Timers[m_SelectedTimer].disableAllDays();

    do
    {
        repeat = server->readPOSTparam(name, 16, value, 16);
        if (url != NULL && strcmp_P(url, pgm_url_select) == 0)
        {
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_clocktimercontroller_inputs[CLOCKTIMERCONTROLLER_I_TIMER])))
                == 0)
            {
                m_SelectedTimer = atoi(value);
            }
        }
        {

            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_clocktimercontroller_inputs[CLOCKTIMERCONTROLLER_I_ACTUATOR])))
                == 0)
            {
                val = atoi(value);
                if (val != -1)
                {
                    //if (!isMapped(atoi(value)))
                    m_ActuatorMapping[m_SelectedTimer] = atoi(value);
                }
                else
                {
                    m_ActuatorMapping[m_SelectedTimer] = -1;
                }
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_clocktimercontroller_inputs[CLOCKTIMERCONTROLLER_I_DOW])))
                     == 0)
            {
                val = atoi(value);
                switch (val)
                {
                    case 0:
                        m_Timers[m_SelectedTimer].enableMonday();
                        break;
                    case 1:
                        m_Timers[m_SelectedTimer].enableTuesday();
                        break;
                    case 2:
                        m_Timers[m_SelectedTimer].enableWednesday();
                        break;
                    case 3:
                        m_Timers[m_SelectedTimer].enableThursday();
                        break;
                    case 4:
                        m_Timers[m_SelectedTimer].enableFriday();
                        break;
                    case 5:
                        m_Timers[m_SelectedTimer].enableSaturday();
                        break;
                    case 6:
                        m_Timers[m_SelectedTimer].enableSunday();
                        break;

                }
            }
            else
            {
                input = atoi(name);
                x = (input - 1) / 4;
                y = input % 4;

                //Ignore input with name 0!
                if (input != 0)
                {
                    switch (y)
                    {
                        case 0:
                            m_Timers[m_SelectedTimer].setMinuteOff(x,
                                                                   atoi(value));

                            break;
                        case 1:
                            m_Timers[m_SelectedTimer].setHourOn(x, atoi(value));

                            break;
                        case 2:
                            m_Timers[m_SelectedTimer].setMinuteOn(x,
                                                                  atoi(value));

                            break;
                        case 3:
                            m_Timers[m_SelectedTimer].setHourOff(x,
                                                                 atoi(value));
                            break;
                    }
                }
            }
        }

    } while (repeat);

    return 0;
}
#endif

#ifdef FEATURE_WEBIF
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
#endif
