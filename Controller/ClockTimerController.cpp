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

/**
 * \brief Getter for the controller clocktimers
 *
 * \param[in] clockTimerID Index of the clocktimer
 * \returns pointer to the requested clocktimer object
 */
ClockTimer* ClockTimerController::getClockTimer(int8_t clockTimerID)
{
    if (clockTimerID >= 0 && clockTimerID < MAX_CLOCKTIMERS)
    {
        return &m_Timers[clockTimerID];
    }
    return NULL;
}

/**
 * \brief Getter for the controlled actuator of a clocktimer
 *
 * \param[in] clockTimerID Index of clocktimer
 * \returns index of the controlled actuator
 */
int8_t ClockTimerController::getAssignedActuatorID(int8_t clockTimerID)
{
    if (clockTimerID >= 0 && clockTimerID < MAX_CLOCKTIMERS)
    {
        return m_ActuatorMapping[clockTimerID];
    }

    return -1;
}

/**
 * \brief Setter for the controlled actuator of a clocktimer
 *
 * \param[in] clockTimerID Index of the clocktimer
 * \param[in] actuatorID Index of the actuator to be controlled by the clocktimer
 */
void ClockTimerController::assignActuatorToClockTimer(int8_t clockTimerID,
                                                      int8_t actuatorID)
{
    if (actuatorID >= 0&& clockTimerID >= 0 &&
    actuatorID < __aquaduino->getNrOfActuators() &&
    clockTimerID < MAX_CLOCKTIMERS)
    {
        m_ActuatorMapping[clockTimerID] = actuatorID;
    }
}

uint16_t ClockTimerController::serialize(Stream* s)
{
    uint8_t i;
    uint8_t clockTimers = MAX_CLOCKTIMERS;

    s->write(clockTimers);

    s->write((uint8_t*) m_ActuatorMapping, sizeof(m_ActuatorMapping));
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
