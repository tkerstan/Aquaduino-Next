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
const static char progTemplateString1[] PROGMEM = "##INAME##";
const static char progTemplateString2[] PROGMEM = "##PIN##";
const static char progTemplateString3[] PROGMEM = "##ACTUATORNAME##";
const static char progTemplateString4[] PROGMEM = "##TYPEOPTIONS##";

enum
{
    DO_IPIN, DO_PIN, DO_NAME, DO_ONVALUE
};

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3,
      progTemplateString4 };

static const char progInputType[] PROGMEM = "type";
const static char progInputPin[] PROGMEM = "ipin";
static const char* const inputStrings[] PROGMEM =
    { progInputType, progInputPin };

enum
{
    I_TYPE, I_PIN
};

/**
 * \brief Constructor
 * \param[in] name Initial name of the Actuator
 * \param[in] pin Pin number of the Actuator
 * \param[in] onValue Value that enables the Actuator
 * \param[in] offValue Value that disables the Actuator
 *
 * The parameters onValue and OffValue allow for Actuators that are driven
 * inverted (On=0 and Off=1) like normally closed relays.
 */
DigitalOutput::DigitalOutput(const char* name, uint8_t onValue,
                             uint8_t offValue) :
        Actuator(name)
{
    m_Type = ACTUATOR_DIGITALOUTPUT;
    this->m_Pin = 0;
    this->m_OnValue = onValue;
    this->m_OffValue = offValue;
    this->m_locked = false;
    this->m_DutyCycle = 0;
    this->on();
}

uint16_t DigitalOutput::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint16_t mySize = sizeof(m_OnValue) + sizeof(m_OffValue);

    if (mySize <= size)
    {
        memcpy(bPtr, &m_OnValue, sizeof(m_OnValue));
        memcpy(bPtr + sizeof(m_OnValue), &m_OffValue, sizeof(m_OffValue));
        memcpy(bPtr + sizeof(m_OnValue) + sizeof(m_OffValue),
               &m_Pin,
               sizeof(m_Pin));
    }
    else
        return 0;

    return mySize;
}

uint16_t DigitalOutput::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    uint16_t mySize = sizeof(m_OnValue) + sizeof(m_OffValue);

    if (size < mySize)
        return 0;

    memcpy(&m_OnValue, bPtr, sizeof(m_OnValue));
    memcpy(&m_OffValue, bPtr + sizeof(m_OnValue), sizeof(m_OffValue));
    memcpy(&m_Pin,
           bPtr + sizeof(m_OnValue) + sizeof(m_OffValue),
           sizeof(m_Pin));
    pinMode(m_Pin, OUTPUT);

    return mySize;
}

/**
 * \brief Enables the DigitalOutput if not locked
 *
 * If not locked writes onValue to the pin
 */
void DigitalOutput::on()
{
    if (!m_locked)
    {
        digitalWrite(m_Pin, m_OnValue);
        if (supportsPWM())
            m_DutyCycle = 1.0;
    }
}

/**
 * \brief Disables the DigitalOutput if not locked
 *
 * If not locked writes offValue to the pin
 */
void DigitalOutput::off()
{
    if (!m_locked)
    {
        digitalWrite(m_Pin, m_OffValue);
        if (supportsPWM())
            m_DutyCycle = 0.0;
    }

}

/**
 * \brief Enables the DigitalOutput
 *
 * Writes onValue to the pin.
 */
void DigitalOutput::forceOn()
{
    digitalWrite(m_Pin, m_OnValue);
    if (supportsPWM())
        m_DutyCycle = 1.0;
}

/**
 * \brief Disables the DigitalOutput
 *
 * Writes offValue to the pin.
 */
void DigitalOutput::forceOff()
{
    digitalWrite(m_Pin, m_OffValue);
    if (supportsPWM())
        m_DutyCycle = 0.0;
}

/**
 * \brief Checks whether the DigitalOutput is enabled or not.
 *
 * returns state of the actuator
 */
int8_t DigitalOutput::isOn()
{
    return digitalRead(m_Pin) == m_OnValue;
}

/**
 * \brief Checks whether the DigitalOutput supports PWM or not.
 *
 * \returns 0 if not, 1 otherwise.
 */
int8_t DigitalOutput::supportsPWM()
{
    return digitalPinHasPWM(m_Pin);
}

/**
 * \brief Sets the PWM duty cycle.
 * \param[in] dutyCycle Must be less or equal 1.0
 *
 * If the DigitalOutput supports PWM this method enables the PWM mode with
 * the given duty cycle.
 */
void DigitalOutput::setPWM(float dutyCycle)
{
    if (supportsPWM() && dutyCycle <= 1.0)
    {
        this->m_DutyCycle = (uint8_t) (dutyCycle * 255);
        analogWrite(m_Pin, dutyCycle);
    }
}

/**
 * \brief Gets current duty cycle when PWM is active.
 *
 * \returns Current duty cycle
 */
float DigitalOutput::getPWM()
{
    return m_DutyCycle;
}

int8_t DigitalOutput::showWebinterface(WebServer* server,
                                       WebServer::ConnectionType type,
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
                m_OnValue = atoi(value);
                m_OffValue = 1 - m_OnValue;
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_PIN])))
                     == 0)
            {
                m_Pin = atoi(value);
                pinMode(m_Pin,OUTPUT);
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
            case DO_IPIN:
                server->print((const __FlashStringHelper *) (progInputPin));
                break;
            case DO_PIN:
                server->print(m_Pin);
                break;
            case DO_NAME:
                server->print(getName());
                break;
            case DO_ONVALUE:
                parser->selectListOption("LOW", "0", m_OnValue == 0, server);
                parser->selectListOption("HIGH", "1", m_OnValue == 1, server);
                break;
            }
        }

        templateFile.close();
    }
    return true;

}
