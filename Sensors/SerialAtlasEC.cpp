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

#ifdef FEATURE_WEBIF
#include <TemplateParser.h>
#include <Framework/Flashvars.h>
#endif

static bool sensorEC_stringcomplete;
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

#ifdef FEATURE_WEBIF
int8_t SerialAtlasEC::showWebinterface(WebServer* server,
                                      WebServer::ConnectionType type, char* url)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[template_digitalinput_fnsize];
    strcpy_P(templateFileName, template_digitalinput_fname);

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_digitalinput_inputs[DI_I_PIN])))
                == 0)
            {
                m_Pin = atoi(value);
                pinMode(m_Pin, INPUT);
            }
        } while (repeat);
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = __aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      template_digitalinput,
                                                      template_digitalinput_elements,
                                                      server))
               >= 0)
        {
            switch (matchIdx)
            {
            case DI_SNAME:
                server->print(getName());
                break;
            case DI_PIN_NAME:
                server->print((__FlashStringHelper *) pgm_read_word(template_digitalinput_inputs));
                break;
            case DI_PIN_VAL:
                server->print(m_Pin);
                break;
            case DI_PIN_SIZE:
                server->print(3);
                break;
            case DI_PIN_MAXLENGTH:
                server->print(2);
                break;

            }
        }

        templateFile.close();
    }
    return true;
}
#endif
