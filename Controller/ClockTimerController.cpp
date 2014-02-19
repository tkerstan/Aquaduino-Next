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

uint16_t ClockTimerController::serialize(void* buffer, uint16_t size)
{
    uint8_t i;
    uint8_t clockTimers = MAX_CLOCKTIMERS;
    uint8_t* charBuffer = (uint8_t*) buffer;
    uint16_t pos = 0;
    uint16_t timerSize = 0;
    int32_t left = size;

    charBuffer[0] = clockTimers;

    if (sizeof(m_ActuatorMapping) + sizeof(uint8_t) > size)
        return 0;

    pos += sizeof(uint8_t);
    memcpy(charBuffer + pos, m_ActuatorMapping, sizeof(m_ActuatorMapping));
    pos += sizeof(m_ActuatorMapping);
    for (i = 0; i < MAX_CLOCKTIMERS; i++)
    {
        left = ((int32_t) size) - pos - 2;
        if (left < 0)
            return 0;
        timerSize = m_Timers[i].serialize(charBuffer + pos + 2, left);
        charBuffer[pos] = (timerSize & 0xFF00) >> 8;
        charBuffer[pos + 1] = timerSize & 0xFF;
        pos += timerSize + 2;
    }
    return pos;
}

uint16_t ClockTimerController::deserialize(void* data, uint16_t size)
{
    uint8_t i;
    uint8_t clockTimers = 0;
    uint8_t* charBuffer = (uint8_t*) data;
    uint16_t pos = 0;
    uint16_t timerSize = 0;

    clockTimers = charBuffer[0];

    if (clockTimers != MAX_CLOCKTIMERS)
        return 0;

    pos += sizeof(uint8_t);

    memcpy(m_ActuatorMapping, charBuffer + pos, sizeof(m_ActuatorMapping));

    pos += sizeof(m_ActuatorMapping);
    for (i = 0; i < MAX_CLOCKTIMERS; i++)
    {
        timerSize = charBuffer[pos] << 8;
        timerSize += charBuffer[pos + 1];
        pos += 2;

        if (m_Timers[i].deserialize(charBuffer + pos, timerSize) == 0)
            return 0;
        pos += timerSize;
    }
    return pos;
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
