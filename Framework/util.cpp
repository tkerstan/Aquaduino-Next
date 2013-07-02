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

#include <Arduino.h>
#include <string.h>

void hts(const uint8_t* bytes, uint8_t byte_size, char* hexString, uint8_t string_size)
{
    int8_t j = 0;
    uint8_t low = 0, high = 0;

    if (string_size < byte_size*2+1)
        return;
    for (j = 0; j < byte_size; j++)
    {
        low = bytes[j] & 0x0F;
        high = (bytes[j] & 0xF0) >> 4;
        hexString[j * 2 + 1] =
                low < 10 ? low + '0' : low - 10 + 'A';
        hexString[j * 2] =
                high < 10 ? high + '0' : high - 10 + 'A';
    }
    hexString[string_size-1] = 0;
}

void sth(const char* hexString, uint8_t* bytes, uint8_t byte_size)
{
    int8_t j = 0;
    char low, high;

    if (byte_size*2 < strlen(hexString))
        return;

    for (j = 0; j < byte_size; j++)
    {
        low = hexString[j*2+1];
        high = hexString[j*2];
        bytes[j] = low >= 'A' ? low + 10 - 'A' : low - '0';
        bytes[j] |= (high >= 'A' ? high + 10 - 'A' : high - '0') << 4;
    }
}
