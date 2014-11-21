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

#include <Framework/Aquaduino.h>
#include "DigitalInput.h"
#include <Arduino.h>
#include <SD.h>

/**
 * \brief Constructor
 */
DigitalInput::DigitalInput()
{
    m_Type = SENSOR_DIGITALINPUT;
    m_Pin = 0;
}

/**
 * \brief Returns the value of the digital input
 *
 * \returns 1 if input is HIGH or 0 if input is LOW.
 */
double DigitalInput::read()
{
    return digitalRead(m_Pin);
}

uint16_t DigitalInput::serialize(Stream* s)
{
	s->write(m_Pin);
	return 1;
}

uint16_t DigitalInput::deserialize(Stream* s)
{
	m_Pin = s->read();
	return 1;
}

void  DigitalInput::setPin(uint8_t pin)
{
	m_Pin = pin;
	pinMode(m_Pin, INPUT);
}

uint8_t DigitalInput::getPin()
{
    return m_Pin;
}
