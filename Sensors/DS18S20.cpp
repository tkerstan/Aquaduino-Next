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

#define TEMP_HISTORY	10

static float temp_hist[TEMP_HISTORY];

static byte runs = 0;

DS18S20::DS18S20(int pin) :
		myOneWire(pin)
{
	int i;
	m_Type = SENSOR_DS18S20;
	for (i = 0; i < TEMP_HISTORY; i++)
	{
		temp_hist[i] = 0.0;
	}
}

double DS18S20::read()
{
	byte i;
	static byte type_s;
	static byte data[12];
	static byte addr[8];
	static int readPending = false;
	static unsigned long lastReadCommand = 0;

	if (readPending == 0)
	{
		if (!myOneWire.search(addr))
		{
			myOneWire.reset_search();
			return celsius;
		}

		if (OneWire::crc8(addr, 7) != addr[7])
		{
			return 0.0;
		}

		// the first ROM byte indicates which chip
		switch (addr[0])
		{
		case 0x10:
			type_s = 1;
			break;
		case 0x28:
			type_s = 0;
			break;
		case 0x22:
			type_s = 0;
			break;
		default:
			return 0.0;
		}
		myOneWire.reset();
		myOneWire.select(addr);
		myOneWire.write(0x44, 1); // start conversion, with parasite power on at the end
		lastReadCommand = millis();
		readPending = 1;

	}
	else if ((readPending == 1) && (millis() - lastReadCommand > 750))
	{
		myOneWire.reset();
		myOneWire.select(addr);
		myOneWire.write(0xBE);         // Read Scratchpad

		for (i = 0; i < 9; i++)
		{           // we need 9 bytes
			data[i] = myOneWire.read();
		}

		// convert the data to actual temperature

		unsigned int raw = (data[1] << 8) | data[0];
		if (type_s)
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
		celsius = (float) raw / 16.0;
		fahrenheit = celsius * 1.8 + 32.0;
		readPending = 0;
		temp_hist[runs] = celsius;
		celsius = 0;
		runs = (runs + 1) % TEMP_HISTORY;
		for (i = 0; i < TEMP_HISTORY; i++)
		{
			celsius += temp_hist[i];
		}
		celsius /= TEMP_HISTORY;
	}
	return celsius;
}

