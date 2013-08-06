/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
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

#include "OneWireHandler.h"

/**
 * \brief Default constructor
 */
OneWireHandler::OneWireHandler()
{
    memset(m_OneWires, 0, sizeof(m_OneWires));
    memset(m_Used, 0, sizeof(m_Used));
    memset(m_Pins, 0, sizeof(m_Pins));
}

/**
 * \brief Default destructor
 */
OneWireHandler::~OneWireHandler()
{
}

/**
 * \brief
 */
int8_t OneWireHandler::addPin(uint8_t pin)
{
    int8_t i = 0;

    for (; i < MAX_ONEWIRE_DEVICES; i++)
    {
        if (m_Pins[i] == pin)
            return i;
        if (m_OneWires[i] == NULL)
        {
            m_Pins[i] = pin;
            m_OneWires[i] = new OneWire(pin);
            return i;
        }
    }

    return -1;
}

/**
 * \brief Scans for OneWire devices
 * \param[in] idx Index of OneWire object to be used
 * \param[out] address Buffer where this method stores the address of the
 * device found
 * \param[in] size Size of the buffer. Needs to be at least 8 bytes.
 *
 * \returns 1 when device found. 0 otherwise
 */
int8_t OneWireHandler::findDevice(uint8_t idx, uint8_t *address, uint8_t size)
{
    if (m_OneWires[idx] == NULL || size < 8)
        return 0;

    if (!m_OneWires[idx]->search(address))
    {
        m_OneWires[idx]->reset_search();
        return 0;
    }

    if (OneWire::crc8(address, 7) != address[7])
    {
        return 0;
    }
    return 1;
}

/**
 * \brief Issue OneWire read command to given address
 * \param[in] idx Index of OneWire object to be used
 * \param[in] addr The address of the OneWire device to read from
 */
void OneWireHandler::issueReadCommand(uint8_t idx, uint8_t* addr)
{
    int8_t i = 0;
    if (m_OneWires[idx] == NULL)
        return;

    m_OneWires[idx]->reset();
    m_OneWires[idx]->select(addr);
    m_OneWires[idx]->write(0x44, 1); // start conversion, with parasite power on at the end
}

/**
 * \brief Reads the scratchpad of the OneWire device.
 * \param[in] idx Index of OneWire object to be used
 * \param[in] addr Address of the OneWire device
 * \param[out] data Buffer for the data
 * \param[in] size Size of the buffer. Needs to be at least 12 Bytes.
 */
int8_t OneWireHandler::read(uint8_t idx, uint8_t* addr, uint8_t* data, uint8_t size)
{
    int8_t i = 0;

    if (m_OneWires[idx] == NULL || size < 12)
        return 0;

    m_OneWires[idx]->reset();
    m_OneWires[idx]->select(addr);
    m_OneWires[idx]->write(0xBE);         // Read Scratchpad

    for (i = 0; i < 9; i++)
    {           // we need 9 bytes
        data[i] = m_OneWires[idx]->read();
    }

    return 1;
}
