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
#include "DigitalOutput.h"
#include <Arduino.h>
#include <TemplateParser.h>

const static char progTemplateFileName[] PROGMEM = "do.htm";
const static char progTemplateString1[] PROGMEM = "##ACTUATORNAME##";
const static char progTemplateString2[] PROGMEM = "##TYPEOPTIONS##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2 };

static const char progInputType[] PROGMEM = "type";
static const char* const inputStrings[] PROGMEM =
    { progInputType };

enum
{
    I_TYPE
};

DigitalOutput::DigitalOutput(const char* name, int8_t pin, uint8_t onValue,
                             uint8_t offValue) :
        Actuator(name)
{
    m_Type = ACTUATOR_DIGITALOUTPUT;
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->onValue = onValue;
    this->offValue = offValue;
    this->m_locked = false;
    this->on();
}

uint16_t DigitalOutput::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    memcpy(bPtr, &onValue, sizeof(onValue));
    memcpy(bPtr + sizeof(onValue), &offValue, sizeof(offValue));
    return 1;
}

uint16_t DigitalOutput::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    memcpy(&onValue, bPtr, sizeof(onValue));
    memcpy(&offValue, bPtr + sizeof(onValue), sizeof(offValue));
    return 1;
}

void DigitalOutput::on()
{
    if (!m_locked)
        digitalWrite(pin, onValue);
}

void DigitalOutput::off()
{
    if (!m_locked)
        digitalWrite(pin, offValue);
}

int8_t DigitalOutput::isOn()
{
    return digitalRead(pin) == onValue;
}

int8_t DigitalOutput::supportsPWM()
{
    return false;
}

void DigitalOutput::setPWM(float dutyCycle)
{
    this->on();
}

float DigitalOutput::getPWM()
{
    return isOn() ? 1.0 : 0.0;
}

int8_t DigitalOutput::showWebinterface(WebServer* server,
                                       WebServer::ConnectionType type)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[sizeof(progTemplateFileName)];
    strcpy_P(templateFileName, progTemplateFileName);

    char* replacementStrings[1];
    //Todo: I hereby promise to not change the actuators name!!!
    replacementStrings[0] = (char*) getName();

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_TYPE]))) == 0)
            {
                onValue = atoi(value);
                offValue = 1 - onValue;
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
            case 0:
                server->print(getName());
                break;
            case 1:
                parser->optionListItem("LOW", "0", onValue == 0, server);
                parser->optionListItem("HIGH", "1", onValue == 1, server);
                break;
            }
        }

        templateFile.close();
    }
    return true;

}
