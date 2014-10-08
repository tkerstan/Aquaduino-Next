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

#ifdef FEATURE_WEBIF
#include <TemplateParser.h>
#include <Framework/Flashvars.h>
#endif

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
}

uint8_t DigitalInput::getPin()
{
    return m_Pin;
}

#ifdef FEATURE_WEBIF
int8_t DigitalInput::showWebinterface(WebServer* server,
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
