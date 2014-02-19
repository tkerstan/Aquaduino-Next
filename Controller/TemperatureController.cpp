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

#include <Controller/TemperatureController.h>
#include <Aquaduino.h>
#include <SD.h>

/**
 * Constructor
 * \param[in] name The name of the controller
 */
TemperatureController::TemperatureController(const char* name) :
        Controller(name)
{
    m_Type = CONTROLLER_TEMPERATURE;
    m_Sensor = -1;
    m_RefTemp1 = 24.0;
    m_Hysteresis1 = 0.3;
    m_Actuator1 = -1;
    m_RefTemp2 = 25.0;
    m_Hysteresis2 = 0.3;
    m_Actuator2 = -1;
    m_Cooling = 0;
    m_Heating = 0;
}

/**
 * \brief Destructor
 *
 * Empty.
 */
TemperatureController::~TemperatureController()
{
}

uint16_t TemperatureController::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_RefTemp1) + sizeof(m_Hysteresis1)
                      + sizeof(m_RefTemp2) + sizeof(m_Hysteresis2);
    if (mySize <= size)
    {
        memcpy(bPtr, &m_Sensor, sizeof(m_Sensor));
        offset += sizeof(m_Sensor);
        memcpy(bPtr + offset, &m_RefTemp1, sizeof(m_RefTemp1));
        offset += sizeof(m_RefTemp1);
        memcpy(bPtr + offset, &m_Hysteresis1, sizeof(m_Hysteresis1));
        offset += sizeof(m_Hysteresis1);
        memcpy(bPtr + offset, &m_Actuator1, sizeof(m_Actuator1));
        offset += sizeof(m_Actuator1);
        memcpy(bPtr + offset, &m_RefTemp2, sizeof(m_RefTemp2));
        offset += sizeof(m_RefTemp2);
        memcpy(bPtr + offset, &m_Hysteresis2, sizeof(m_Hysteresis2));
        offset += sizeof(m_Hysteresis2);
        memcpy(bPtr + offset, &m_Actuator2, sizeof(m_Actuator2));
        offset += sizeof(m_Actuator2);
        return mySize;
    }
    return 0;
}

uint16_t TemperatureController::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_RefTemp1) + sizeof(m_Hysteresis1)
                      + sizeof(m_RefTemp2) + sizeof(m_Hysteresis2);
    if (mySize <= size)
    {
        memcpy(&m_Sensor, bPtr, sizeof(m_Sensor));
        offset += sizeof(m_Sensor);
        if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
            m_Sensor = -1;
        memcpy(&m_RefTemp1, bPtr + offset, sizeof(m_RefTemp1));
        offset += sizeof(m_RefTemp1);
        memcpy(&m_Hysteresis1, bPtr + offset, sizeof(m_Hysteresis1));
        offset += sizeof(m_Hysteresis1);
        memcpy(&m_Actuator1, bPtr + offset, sizeof(m_Actuator1));
        offset += sizeof(m_Actuator1);
        memcpy(&m_RefTemp2, bPtr + offset, sizeof(m_RefTemp2));
        offset += sizeof(m_RefTemp2);
        memcpy(&m_Hysteresis2, bPtr + offset, sizeof(m_Hysteresis2));
        offset += sizeof(m_Hysteresis2);
        memcpy(&m_Actuator2, bPtr + offset, sizeof(m_Actuator2));
        offset += sizeof(m_Actuator2);
        return mySize;
    }
    return 0;
}

/**
 * \brief Run method triggered by Aquaduino::run
 *
 * Turns on all assigned actuators when temperature exceeds m_Threshold.
 * When the temperature drops below m_Threshold - HYSTERESIS all assigned
 * actuators are turned off.
 */
int8_t TemperatureController::run()
{
    float temp;
    Actuator *actuator1, *actuator2;

    if (m_Sensor == -1 || (m_Actuator1 == -1 && m_Actuator2 == -1))
        return -1;

    temp = __aquaduino->getSensorValue(m_Sensor);
    actuator1 = __aquaduino->getActuator(m_Actuator1);
    actuator2 = __aquaduino->getActuator(m_Actuator2);

    if (actuator1)
    {
        if (temp < m_RefTemp1)
        {
            actuator1->on();
            m_Heating = 1;
        }
        else if (m_Heating && (temp > m_RefTemp1 + m_Hysteresis1))
        {
            actuator1->off();
            m_Heating = 0;
        }
    }

    if (actuator2)
    {
        if (temp > m_RefTemp2)
        {
            actuator2->on();
            m_Cooling = 1;
        }
        else if (actuator2 && m_Cooling && temp < m_RefTemp2 - m_Hysteresis1)
        {
            actuator2->off();
            m_Cooling = 0;
        }
    }

    return true;
}
