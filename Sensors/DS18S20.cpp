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

#include "DS18S20.h"
#include <Framework/Aquaduino.h>
#include "DigitalInput.h"
#include <Arduino.h>
#include <SD.h>
#include <Framework/util.h>
#include <Framework/OneWireHandler.h>

/**
 * \brief Constructor
 */
DS18S20::DS18S20()
{
    int i;
    m_Type = SENSOR_DS18S20;
    for (i = 0; i < TEMP_HISTORY; i++)
    {
        temp_hist[i] = 0.0;
    }
    m_Pin = 0;
    m_Idx = 0;
    m_ReadPending = 0;
    m_LastReadIssue = 0;
}

/**
 * \brief Reads the sensor
 *
 * Currently only supports one DS18S20.
 * Reading is done on two phases to remove delay calls.
 * First phase (readPending = 0) searches for attached DS18S20 and issues a
 * conversion.
 *
 * Second phase (readPending = 1) reads the value when more than one second
 * has passed since the read was issued.
 */
double DS18S20::read()
{
    uint8_t data[12];
    int8_t i = 0;
    OneWireHandler* handler = __aquaduino->getOneWireHandler();

    if (!m_ReadPending)
    {
        handler->issueReadCommand(m_Idx, m_Address);
        m_LastReadIssue = millis();
        m_ReadPending = 1;
    }
    else if (millis() - m_LastReadIssue > 750)
    {
        if (handler->read(m_Idx, m_Address, data, 12))
            return 0.0;
        temp_hist[m_Runs++] = ((double) convertToRaw(data,
                                                   12,
                                                   m_Address[0] == 0x10))
                            / 16;


        m_Celsius = 0;
        for (i = 0; i < TEMP_HISTORY; i++)
            m_Celsius += temp_hist[i];
        m_Celsius /= TEMP_HISTORY;
        if (m_Runs == TEMP_HISTORY)
            m_Runs = 0;
        m_ReadPending = 0;
    }

    return m_Celsius;
}

/**
 * \brief Converts the data to a raw 16 bit value
 * \param[in] data The data to be converted
 * \param[in] size Size of the data.
 * \param[in] type Device type. Held in first byte of the address.
 */
uint16_t DS18S20::convertToRaw(uint8_t* data, uint8_t size, int8_t type)
{
    unsigned int raw = (data[1] << 8) | data[0];
    if (type == 1)
    {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10)
        {
            // count remain gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    }
    else
    {
        byte cfg = (data[4] & 0x60);
        if (cfg == 0x00)
            raw = raw << 3;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20)
            raw = raw << 2; // 10 bit res, 187.5 ms
        else if (cfg == 0x40)
            raw = raw << 1; // 11 bit res, 375 ms
        // default is 12 bit resolution, 750 ms conversion time
    }
    return raw;
}

uint16_t DS18S20::serialize(Stream* s)
{
	s->write(m_Pin);
	s->write(m_Address, sizeof(m_Address));
    return sizeof(m_Pin) + sizeof(m_Address);
}

uint16_t DS18S20::deserialize(Stream* s)
{
    OneWireHandler* handler = __aquaduino->getOneWireHandler();

    m_Pin = s->read();
	s->readBytes((char*)m_Address, sizeof(m_Address));
    if (handler != NULL)
        m_Idx = handler->addPin(m_Pin);
    return sizeof(m_Pin) + sizeof(m_Address);
}

void DS18S20::setPin(uint8_t pin)
{
    m_Pin = pin;
}

uint8_t DS18S20::getPin()
{
    return m_Pin;
}

void DS18S20::setAddress(uint8_t* addr)
{
    uint8_t i = 0;
    for (; i < sizeof(m_Address); i++)
        m_Address[i] = addr[i];
}

void DS18S20::getAddress(uint8_t* addr)
{
    uint8_t i = 0;
    for (; i < sizeof(m_Address); i++)
        addr[i] = m_Address[i];
}
