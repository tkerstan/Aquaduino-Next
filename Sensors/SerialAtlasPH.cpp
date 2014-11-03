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
#include "SerialAtlasPH.h"
#include <Arduino.h>
#include <SD.h>

static String sensorstringPH = "";
static double actualPH;

void serialEvent3(){
  char inchar = (char)Serial3.read();
  sensorstringPH += inchar;
  if(inchar == '\r') {
      char tempPH[6];
          sensorstringPH.toCharArray(tempPH,5);
          actualPH = 0;
          actualPH = atof(tempPH);
          sensorstringPH="";
  }
}


/**
 * \brief Constructor
 */
SerialAtlasPH::SerialAtlasPH()
{
    Serial3.begin(38400);
    m_Type = SENSOR_DIGITALINPUT;
    m_Pin = 0;
}

/**
 * \brief Returns the value of the digital input
 *
 * \returns 1 if input is HIGH or 0 if input is LOW.
 */
double SerialAtlasPH::read()
{
    return actualPH;
}

uint16_t SerialAtlasPH::serialize(Stream* s)
{
	s->write(m_Pin);
	return 1;
}

uint16_t SerialAtlasPH::deserialize(Stream* s)
{
	m_Pin = s->read();
	pinMode(m_Pin, INPUT);
	return 1;
}

int8_t SerialAtlasPH::setPin(int8_t pin)
{
    m_Pin = pin;
    pinMode(m_Pin, INPUT);
    return m_Pin;
}

int8_t SerialAtlasPH::getPin()
{
    return m_Pin;
}

