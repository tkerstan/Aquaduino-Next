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

    virtual uint16_t serialize(void* buffer, uint16_t size);
    virtual uint16_t deserialize(void* data, uint16_t size);

    virtual int8_t run();
    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type,
                                    char* url);
private:
    int8_t m_Sensor;
    double m_RefTemp1;
    double m_Hysteresis1;
    int8_t m_Actuator1;
    double m_RefTemp2;
    double m_Hysteresis2;
    int8_t m_Actuator2;

    int8_t m_Cooling;
    int8_t m_Heating;
};

#endif /* TEMPERATURECONTROLLER_H_ */
