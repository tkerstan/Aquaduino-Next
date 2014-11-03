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
#include <Framework/Aquaduino.h>
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

uint16_t TemperatureController::serialize(Stream* s)
{
    uint16_t mySize = sizeof(m_RefTemp1) + sizeof(m_Hysteresis1)
                      + sizeof(m_RefTemp2) + sizeof(m_Hysteresis2);

    s->write((uint8_t*) &m_Sensor, sizeof(m_Sensor));
    s->write((uint8_t*) &m_RefTemp1, sizeof(m_RefTemp1));
    s->write((uint8_t*) &m_Hysteresis1, sizeof(m_Hysteresis1));
    s->write((uint8_t*) &m_Actuator1, sizeof(m_Actuator1));
    s->write((uint8_t*) &m_RefTemp2, sizeof(m_RefTemp2));
    s->write((uint8_t*) &m_Hysteresis2, sizeof(m_Hysteresis2));
    s->write((uint8_t*) &m_Actuator2, sizeof(m_Actuator2));

    return mySize;
}

uint16_t TemperatureController::deserialize(Stream* s)
{
    uint16_t mySize = sizeof(m_RefTemp1) + sizeof(m_Hysteresis1)
                      + sizeof(m_RefTemp2) + sizeof(m_Hysteresis2);
    if (mySize != s->available())
    {
    	s->readBytes((char*) &m_Sensor, sizeof(m_Sensor));
        if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
            m_Sensor = -1;
    	s->readBytes((char*) &m_RefTemp1, sizeof(m_RefTemp1));
    	s->readBytes((char*) &m_Hysteresis1, sizeof(m_Hysteresis1));
    	s->readBytes((char*) &m_Actuator1, sizeof(m_Actuator1));
    	s->readBytes((char*) &m_RefTemp2, sizeof(m_RefTemp2));
    	s->readBytes((char*) &m_Hysteresis2, sizeof(m_Hysteresis2));
    	s->readBytes((char*) &m_Actuator2, sizeof(m_Actuator2));
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

int8_t TemperatureController::getAssignedSensor()
{
    return m_Sensor;
}

int8_t TemperatureController::assignSensor(int8_t sensorIdx)
{
    m_Sensor = sensorIdx;
    return m_Sensor;
}

double TemperatureController::getRefTempLow()
{
    return m_RefTemp1;
}

double TemperatureController::setRefTempLow(double tempLow)
{
    m_RefTemp1 = tempLow;
    return m_RefTemp1;
}

double TemperatureController::setHeatingHysteresis(double hysteresis)
{
    m_Hysteresis1 = hysteresis;
    return m_Hysteresis1;
}

double TemperatureController::getHeatingHysteresis()
{
    return m_Hysteresis1;
}

int8_t TemperatureController::assignHeatingActuator(int8_t actuatorIdx)
{
    m_Actuator1 = actuatorIdx;
    return m_Actuator1;
}

int8_t TemperatureController::getHeatingActuator()
{
    return m_Actuator1;
}

double TemperatureController::getRefTempHigh()
{
    return m_RefTemp2;
}

double TemperatureController::setRefTempHigh(double tempHigh)
{
    m_RefTemp2 = tempHigh;
    return m_RefTemp2;
}

double TemperatureController::setCoolingHysteresis(double hysteresis)
{
    m_Hysteresis2 = hysteresis;
    return m_Hysteresis2;
}

double TemperatureController::getCoolingHysteresis()
{
    return m_Hysteresis2;
}

int8_t TemperatureController::assignCoolingActuator(int8_t actuatorIdx)
{
    m_Actuator2 = actuatorIdx;
    return m_Actuator2;
}

int8_t TemperatureController::getCoolingActuator()
{
    return m_Actuator2;
}
