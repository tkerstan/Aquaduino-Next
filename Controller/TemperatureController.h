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

#ifndef TEMPERATURECONTROLLER_H_
#define TEMPERATURECONTROLLER_H_

#include <Framework/Controller.h>
#include <Framework/Actuator.h>

/**
 * \brief Controller for temperature monitoring
 *
 * Turns on all assigned actuators when temperature exceeds m_Threshold.
 * When the temperature drops below m_Threshold - HYSTERESIS all assigned
 * actuators are turned off.
 */
class TemperatureController: public Controller
{
public:
    TemperatureController(const char* name);
    virtual ~TemperatureController();

    int8_t getAssignedSensor();
    int8_t assignSensor(int8_t sensorIdx);

    double getRefTempLow();
    double setRefTempLow(double tempLow);

    double setHeatingHysteresis(double hysteresis);
    double getHeatingHysteresis();

    int8_t assignHeatingActuator(int8_t actuatorIdx);
    int8_t getHeatingActuator();

    double getRefTempHigh();
    double setRefTempHigh(double tempHigh);

    double setCoolingHysteresis(double hysteresis);
    double getCoolingHysteresis();

    int8_t assignCoolingActuator(int8_t actuatorIdx);
    int8_t getCoolingActuator();

    virtual uint16_t serialize(Stream* s);
    virtual uint16_t deserialize(Stream* s);

    virtual int8_t run();

private:
    int8_t m_Sensor;
    double m_RefTempLow;
    double m_HeatingHysteresis;
    int8_t m_HeatingActuator;
    double m_RefTempHigh;
    double m_CoolingHysteresis;
    int8_t m_CoolingActuator;

    int8_t m_isCooling;
    int8_t m_isHeating;
};

#endif /* TEMPERATURECONTROLLER_H_ */
