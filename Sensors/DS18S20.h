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
#ifndef DS18S20_H_
#define DS18S20_H

#include <Framework/Sensor.h>
#include <Arduino.h>
#include <OneWire.h>
#include <Framework/FrameworkConfig.h>

/**
 * \brief Basic implementation for DS18S20 Temperature Sensors
 *
 * Allows for reading a single DS18S20. The temperature is calculated as
 * arithmetic mean of a history of #TEMP_HISTORY values. The returned value
 * thus adjusts slowly temperature differences. This prevents controllers
 * from enabling or disabling actuators when there is a single faulty read.
 */
class DS18S20: public Sensor
{
public:
    DS18S20();
    double read();

    uint16_t serialize(void* buffer, uint16_t size);
    uint16_t deserialize(void* data, uint16_t size);
    int8_t showWebinterface(WebServer* server, WebServer::ConnectionType type,
                            char* url);

private:
    uint16_t convertToRaw(uint8_t* data, uint8_t size, int8_t type);
    uint8_t m_Pin;
    uint8_t m_Idx;
    double m_Celsius;
    double m_Fahrenheit;
    float temp_hist[TEMP_HISTORY];
    uint8_t m_Address[8];
    unsigned long m_LastReadIssue;
    uint8_t m_ReadPending;
    uint8_t m_Runs;
};

#endif /* DS18S20_H_ */
