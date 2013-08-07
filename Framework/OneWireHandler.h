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

#ifndef ONEWIREHANDLER_H_
#define ONEWIREHANDLER_H_

#include <OneWire.h>
#include <Framework/FrameworkConfig.h>

/**
 * \brief Muxing and Demuxing of multiple OneWire devices
 */
class OneWireHandler
{
public:
    OneWireHandler();
    virtual ~OneWireHandler();

    int8_t addPin(uint8_t pin);
    int8_t findDevice(uint8_t idx, uint8_t *address, uint8_t size);
    void issueReadCommand(uint8_t idx, uint8_t* addr);
    int8_t read(uint8_t idx, uint8_t* addr, uint8_t* data, uint8_t size);

private:
    OneWireHandler(const OneWireHandler&);
    OneWireHandler(OneWireHandler&);

    OneWire* m_OneWires[MAX_ONEWIRE_DEVICES];
    uint8_t m_Pins[MAX_ONEWIRE_DEVICES];
    uint8_t m_Used[MAX_ONEWIRE_DEVICES];
};

#endif /* ONEWIREHANDLER_H_ */
