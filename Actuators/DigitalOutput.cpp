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
#include <DigitalOutput.h>
#include <Arduino.h>

/**
 * \brief Constructor
 * \param[in] name Initial name of the Actuator
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

uint16_t DigitalOutput::serialize(Stream* s)
{
    uint16_t mySize = sizeof(m_OnValue) + sizeof(m_OffValue);

	s->write(&m_OnValue, sizeof(m_OnValue));
	s->write(&m_OffValue, sizeof(m_OffValue));
	s->write((uint8_t*) &m_Pin, sizeof(m_Pin));
	s->write(&m_On, sizeof(m_On));
	s->write((uint8_t*)&m_DutyCycle, sizeof(m_DutyCycle));

    return mySize;
}

uint16_t DigitalOutput::deserialize(Stream* s)
{
	uint16_t mySize = sizeof(m_OnValue) + sizeof(m_OffValue);

    if (s->available() < mySize)
        return 0;

	s->readBytes((char*)&m_OnValue, sizeof(m_OnValue));
	s->readBytes((char*)&m_OffValue, sizeof(m_OffValue));
	s->readBytes((char*)&m_Pin, sizeof(m_Pin));
	s->readBytes((char*)&m_On, sizeof(m_On));
	s->readBytes((char*)&m_DutyCycle, sizeof(m_DutyCycle));

    if (supportsPWM())
        setPWM(m_DutyCycle);
    else if (m_On)
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
        if (m_DutyCycle > 0)
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

void DigitalOutput::setPin(uint8_t pin)
{
    pinMode(pin, OUTPUT);
    m_Pin = pin;
}

uint8_t DigitalOutput::getPin()
{
    return m_Pin;
}
