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
    m_RefTempLow = 24.0;
    m_HeatingHysteresis = 0.3;
    m_HeatingActuator = -1;
    m_RefTempHigh = 25.0;
    m_CoolingHysteresis = 0.3;
    m_CoolingActuator = -1;
    m_isCooling = 0;
    m_isHeating = 0;
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
    uint16_t mySize = sizeof(m_RefTempLow) + sizeof(m_HeatingHysteresis)
                      + sizeof(m_RefTempHigh) + sizeof(m_CoolingHysteresis);

    s->write((uint8_t*) &m_Sensor, sizeof(m_Sensor));
    s->write((uint8_t*) &m_RefTempLow, sizeof(m_RefTempLow));
    s->write((uint8_t*) &m_HeatingHysteresis, sizeof(m_HeatingHysteresis));
    s->write((uint8_t*) &m_HeatingActuator, sizeof(m_HeatingActuator));
    s->write((uint8_t*) &m_RefTempHigh, sizeof(m_RefTempHigh));
    s->write((uint8_t*) &m_CoolingHysteresis, sizeof(m_CoolingHysteresis));
    s->write((uint8_t*) &m_CoolingActuator, sizeof(m_CoolingActuator));

    return mySize;
}

uint16_t TemperatureController::deserialize(Stream* s)
{
    uint16_t mySize = sizeof(m_RefTempLow) + sizeof(m_HeatingHysteresis)
                      + sizeof(m_RefTempHigh) + sizeof(m_CoolingHysteresis);
    if (mySize != s->available())
    {
    	s->readBytes((char*) &m_Sensor, sizeof(m_Sensor));
        if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
            m_Sensor = -1;
    	s->readBytes((char*) &m_RefTempLow, sizeof(m_RefTempLow));
    	s->readBytes((char*) &m_HeatingHysteresis, sizeof(m_HeatingHysteresis));
    	s->readBytes((char*) &m_HeatingActuator, sizeof(m_HeatingActuator));
    	s->readBytes((char*) &m_RefTempHigh, sizeof(m_RefTempHigh));
    	s->readBytes((char*) &m_CoolingHysteresis, sizeof(m_CoolingHysteresis));
    	s->readBytes((char*) &m_CoolingActuator, sizeof(m_CoolingActuator));
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

    if (m_Sensor == -1 || (m_HeatingActuator == -1 && m_CoolingActuator == -1))
        return -1;

    temp = __aquaduino->getSensorValue(m_Sensor);
    actuator1 = __aquaduino->getActuator(m_HeatingActuator);
    actuator2 = __aquaduino->getActuator(m_CoolingActuator);

    if (actuator1)
    {
        if (temp < (m_RefTempLow - m_HeatingHysteresis))
        {
            actuator1->on();
            m_isHeating = 1;
        }
        else if (m_isHeating && (temp >= m_RefTempLow) )
        {
            actuator1->off();
            m_isHeating = 0;
        }
    }

    if (actuator2)
    {
        if (temp > (m_RefTempHigh + m_CoolingHysteresis))
        {
            actuator2->on();
            m_isCooling = 1;
        }
        else if (m_isCooling && (temp <= m_RefTempHigh))
        {
            actuator2->off();
            m_isCooling = 0;
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
    return m_RefTempLow;
}

double TemperatureController::setRefTempLow(double tempLow)
{
    m_RefTempLow = tempLow;
    return m_RefTempLow;
}

double TemperatureController::setHeatingHysteresis(double hysteresis)
{
    m_HeatingHysteresis = hysteresis;
    return m_HeatingHysteresis;
}

double TemperatureController::getHeatingHysteresis()
{
    return m_HeatingHysteresis;
}

int8_t TemperatureController::assignHeatingActuator(int8_t actuatorIdx)
{
    m_HeatingActuator = actuatorIdx;
    return m_HeatingActuator;
}

int8_t TemperatureController::getHeatingActuator()
{
    return m_HeatingActuator;
}

double TemperatureController::getRefTempHigh()
{
    return m_RefTempHigh;
}

double TemperatureController::setRefTempHigh(double tempHigh)
{
    m_RefTempHigh = tempHigh;
    return m_RefTempHigh;
}

double TemperatureController::setCoolingHysteresis(double hysteresis)
{
    m_CoolingHysteresis = hysteresis;
    return m_CoolingHysteresis;
}

double TemperatureController::getCoolingHysteresis()
{
    return m_CoolingHysteresis;
}

int8_t TemperatureController::assignCoolingActuator(int8_t actuatorIdx)
{
    m_CoolingActuator = actuatorIdx;
    return m_CoolingActuator;
}

int8_t TemperatureController::getCoolingActuator()
{
    return m_CoolingActuator;
}
