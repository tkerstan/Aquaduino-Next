/*
 * Copyright (c) 2014 Timo Kerstan.  All right reserved.
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

#ifndef SERIALATLASEC_H
#define SERIALATLASEC_H

#include <Framework/Sensor.h>

/**
 * \brief Class for using Arduino pins as digital input.
 */
class SerialAtlasEC: public Sensor
{
public:
    SerialAtlasEC();
    double read();

    uint16_t serialize(Stream* s);
    uint16_t deserialize(Stream* s);

    int8_t getPin();
    int8_t setPin(int8_t pin);

private:
    unsigned char m_Pin;
};

#endif
