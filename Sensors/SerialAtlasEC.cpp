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
#include "SerialAtlasEC.h"
#include <Arduino.h>
#include <SD.h>
#include <Time.h>
#include <Framework/ObjectTypes.h>

static String sensorstringEC = "";
static String sensorstringECRaw = "";
static double actualEC;
static int tempSensorID;
int initCounter = 0;
static int8_t curMin = minute();

void serialEvent1()
{
    char inchar = (char) Serial1.read();
    sensorstringEC += inchar;
    sensorstringECRaw += inchar;
    if (inchar == ',')
    {
        sensorstringEC = "";
    }
    if (inchar == '\r')
    {
        char tempEC[6];
        sensorstringEC.toCharArray(tempEC, 5);
        actualEC = 0;
        actualEC = atof(tempEC);
        sensorstringEC = "";
        Serial.println(sensorstringECRaw);
        sensorstringECRaw = "";

    }
}

/**
 * \brief Constructor
 */
SerialAtlasEC::SerialAtlasEC()
{

    Serial1.begin(38400);
    //Serial1.print("P3\r");
    // Serial1.print("26.0\r");
    //Serial1.print("C\r");

    m_Type = SENSOR_DIGITALINPUT;
    m_Pin = 0;
}

/**
 * \brief Returns the value of the digital input
 *
 * \returns 1 if input is HIGH or 0 if input is LOW.
 */
double SerialAtlasEC::read()
{
    Sensor* sensor;
    // Serial.println("EC");
    // Serial.println(actualEC);

    // init Sensor
    if (initCounter < 10000)
        initCounter++;
    if (initCounter == 10000)
    {
        __aquaduino->resetSensorIterator();
        while (__aquaduino->getNextSensor(&sensor) != -1)
        {
            if (sensor->getType() == SENSOR_DS18S20)
            {
                tempSensorID=__aquaduino->getSensorID(sensor);
            }
        }

        Serial1.print("25.00,C\r");
        Serial.println("EC set to send continuous data");
        initCounter++;
    }
    //set Temperature every Minute
    if (minute() != curMin)
        {
            curMin = minute();
            Serial.print(__aquaduino->getSensorValue(tempSensorID));
            Serial.println((" Set new Temperature to EC"));
            Serial1.print(__aquaduino->getSensorValue(tempSensorID));
            Serial1.print(",C\r");

        }
    return actualEC;
}

uint16_t SerialAtlasEC::serialize(Stream* s)
{
	s->write(m_Pin);
	return 1;
}

uint16_t SerialAtlasEC::deserialize(Stream* s)
{
	m_Pin = s->read();
	pinMode(m_Pin, INPUT);
	return 1;
}

int8_t SerialAtlasEC::setPin(int8_t pin)
{
    m_Pin = pin;
    pinMode(m_Pin, INPUT);
    return m_Pin;
}

int8_t SerialAtlasEC::getPin()
{
    return m_Pin;
}
