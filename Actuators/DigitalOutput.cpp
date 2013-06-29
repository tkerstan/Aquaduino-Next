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
const static char progTemplateString3[] PROGMEM = "##IPWM##";
const static char progTemplateString4[] PROGMEM = "##PWM##";
const static char progTemplateString5[] PROGMEM = "##ACTUATORNAME##";
const static char progTemplateString6[] PROGMEM = "##TYPEOPTIONS##";

enum
{
    DO_IPIN, DO_PIN, DO_IPWM, DO_PWM, DO_NAME, DO_ONVALUE
};

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3,
      progTemplateString4, progTemplateString5, progTemplateString6 };

static const char progInputType[] PROGMEM = "type";
const static char progInputPin[] PROGMEM = "ipin";
const static char progInputPWM[] PROGMEM = "ipwm";
static const char* const inputStrings[] PROGMEM =
    { progInputType, progInputPin, progInputPWM };

enum
{
    I_TYPE, I_PIN, I_PWM
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
    this->m_DutyCycle = 0.0;
    this->m_On = 0;
}

uint16_t DigitalOutput::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint16_t mySize = sizeof(m_OnValue) + sizeof(m_OffValue);

    if (mySize <= size)
    {
        memcpy(bPtr, &m_OnValue, sizeof(m_OnValue));
        bPtr += sizeof(m_OnValue);
        memcpy(bPtr, &m_OffValue, sizeof(m_OffValue));
        bPtr += sizeof(m_OffValue);
        memcpy(bPtr, &m_Pin, sizeof(m_Pin));
        bPtr += sizeof(m_Pin);
        memcpy(bPtr, &m_On, sizeof(m_On));
        bPtr += sizeof(m_On);
        memcpy(bPtr, &m_DutyCycle, sizeof(m_DutyCycle));

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
    bPtr += sizeof(m_OnValue);
    memcpy(&m_OffValue, bPtr, sizeof(m_OffValue));
    bPtr += sizeof(m_OffValue);
    memcpy(&m_Pin, bPtr, sizeof(m_Pin));
    bPtr += sizeof(m_Pin);
    memcpy(&m_On, bPtr, sizeof(m_On));
    bPtr += sizeof(m_On);
    memcpy(&m_DutyCycle, bPtr, sizeof(m_DutyCycle));
    pinMode(m_Pin, OUTPUT);

    if(supportsPWM())
        setPWM(m_DutyCycle);
    else if(m_On)
        forceOn();
    else
        forceOff();

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
        if (supportsPWM())
            analogWrite(m_Pin, (uint8_t) (m_OnValue * 255));
        else
            digitalWrite(m_Pin, m_OnValue);
        m_DutyCycle = 1.0;
        m_On = 1;
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
        if (supportsPWM())
            analogWrite(m_Pin, (uint8_t) (m_OffValue * 255));
        else
            digitalWrite(m_Pin, m_OffValue);
        m_On = 0;
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
    if (supportsPWM())
        analogWrite(m_Pin, (uint8_t) (m_OnValue * 255));
    else
        digitalWrite(m_Pin, m_OnValue);
    m_DutyCycle = 1.0;
    m_On = 1;
}

/**
 * \brief Disables the DigitalOutput
 *
 * Writes offValue to the pin.
 */
void DigitalOutput::forceOff()
{
    if (supportsPWM())
        analogWrite(m_Pin, (uint8_t) (m_OffValue * 255));
    else
        digitalWrite(m_Pin, m_OffValue);
    m_DutyCycle = 0.0;
    m_On = 0;
}

/**
 * \brief Checks whether the DigitalOutput is enabled or not.
 *
 * returns state of the actuator
 */
int8_t DigitalOutput::isOn()
{
    return m_On;
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
        m_DutyCycle = dutyCycle;
        if (m_OnValue == 0)
            analogWrite(m_Pin, (uint8_t) ((1.0 - dutyCycle) * 255));
        else
            analogWrite(m_Pin, (uint8_t) (dutyCycle * 255));
        if( m_DutyCycle > 0)
            m_On = 1;
        else
            m_On = 0;
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
    float dC = -1.0;

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
                pinMode(m_Pin, OUTPUT);
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(inputStrings[I_PWM])))
                     == 0)
            {
                dC = atof(value);
                if (dC < 0.0)
                    dC = 0;
                else if (dC > 100.0)
                    dC = 100.0;
                dC /= 100;
            }

        } while (repeat);
        if (dC >= 0)
            setPWM(dC);
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
            case DO_IPWM:
                server->print((const __FlashStringHelper *) (progInputPWM));
                break;
            case DO_PWM:
                if (supportsPWM())
                    server->print(getPWM() * 100);
                else
                    server->print(F("No PWM"));
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
