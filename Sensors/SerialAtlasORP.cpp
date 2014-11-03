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
#include "SerialAtlasORP.h"
#include <Arduino.h>
#include <SD.h>

static String sensorstringORP = "";
static double actualORP;

void serialEvent2(){
  char inchar = (char)Serial2.read();
  sensorstringORP += inchar;
  if(inchar == '\r') {
      char tempORP[6];
          sensorstringORP.toCharArray(tempORP,5);
          actualORP = 0;
          actualORP = atof(tempORP);
          sensorstringORP="";
  }
}


/**
 * \brief Constructor
 */
SerialAtlasORP::SerialAtlasORP()
{
    Serial2.begin(38400);
    m_Type = SENSOR_DIGITALINPUT;
    m_Pin = 0;
}

/**
 * \brief Returns the value of the digital input
 *
 * \returns 1 if input is HIGH or 0 if input is LOW.
 */
double SerialAtlasORP::read()
{
    return actualORP;
}

uint16_t SerialAtlasORP::serialize(Stream* s)
{
	s->write(m_Pin);
	return 1;
}

uint16_t SerialAtlasORP::deserialize(Stream* s)
{
	m_Pin = s->read();
	pinMode(m_Pin, INPUT);
	return 1;
}

int8_t SerialAtlasORP::setPin(int8_t pin)
{
    m_Pin = pin;
    pinMode(m_Pin, INPUT);
    return m_Pin;
}

int8_t SerialAtlasORP::getPin()
{
    return m_Pin;
}
