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

#include <Aquaduino.h>
#include "DigitalInput.h"
#include <Arduino.h>
#include <SD.h>
#include <TemplateParser.h>

/**
 * \brief Constructor
 * \param[in] pin pin to be used as input
 */
DigitalInput::DigitalInput()
{
    m_Type = SENSOR_DIGITALINPUT;
    myPin = 0;
}

/**
 * \brief Returns the value of the digital input
 *
 * \returns 1 if input is HIGH or 0 if input is LOW.
 */
double DigitalInput::read()
{
    return digitalRead(myPin);
}

uint16_t DigitalInput::serialize(void* buffer, uint16_t size)
{
    memcpy(buffer, &myPin, sizeof(myPin));
    return sizeof(myPin);
}

uint16_t DigitalInput::deserialize(void* data, uint16_t size)
{
    memcpy(&myPin, data, sizeof(myPin));
    return sizeof(myPin);
}

const static char progTemplateFileName[] PROGMEM = "di.htm";
const static char progTemplateString1[] PROGMEM = "##INAME##";
const static char progTemplateString2[] PROGMEM = "##NAME##";
const static char progTemplateString3[] PROGMEM = "##PIN##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3};

static const char progInput[] PROGMEM = "ipin";
static const char* const inputStrings[] PROGMEM =
    { progInput };


enum
{
    S_INAME,
    S_NAME,
    S_PIN
};

enum
{
    I_TYPE
};

int8_t DigitalInput::showWebinterface(WebServer* server, WebServer::ConnectionType type,
                        char* url)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[sizeof(progTemplateFileName)];
    strcpy_P(templateFileName, progTemplateFileName);

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_TYPE]))) == 0)
            {
                myPin = atoi(value);
            }
        } while (repeat);
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      templateStrings,
                                                      sizeof(templateStrings) / sizeof(char*),
                                                      server))
               >= 0)
        {
            switch (matchIdx)
            {
            case S_INAME:
                server->print((__FlashStringHelper*) &inputStrings[0][0]);
                break;
            case S_NAME:
                server->print(getName());
                break;
            case S_PIN:
                server->print(myPin);
                break;
            }
        }

        templateFile.close();
    }
    return true;
}

