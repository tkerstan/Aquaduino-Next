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

#include "PWMOutput.h"
#include <Arduino.h>

PWMOutput::PWMOutput(const char* name, int8_t pin, uint8_t onValue,
                     uint8_t offValue) :
        Actuator(name),
        m_pin(pin),
        m_onValue(onValue),
        m_offValue(offValue),
        m_dutyCycle(0.0)
{
    // TODO: Use digital_pin_to_timer!
}

uint16_t PWMOutput::serialize(void* buffer, uint16_t size)
{
    return 0;
}

uint16_t PWMOutput::deserialize(void* data, uint16_t size)
{
    return 0;
}

void PWMOutput::on()
{
    if (!m_locked)
    {
        setPWM(1.0);
    }
}

void PWMOutput::off()
{
    if (!m_locked)
    {
        setPWM(0.0);
    }
}

int8_t PWMOutput::isOn()
{
    return m_dutyCycle < 1.0;
}

int8_t PWMOutput::supportsPWM()
{
    return true;
}

void PWMOutput::setPWM(float dC)
{
    if (!m_locked)
    {
        //100% is enough...
        if (dC > 1.0)
            dC = 1.0;

        this->m_dutyCycle = 1.0 - dC;
        //OCR4B = this->m_dutyCycle * 0x3FF;
    }
}

float PWMOutput::getPWM()
{
    return m_dutyCycle;
}

int8_t PWMOutput::showWebinterface(WebServer* server,
                                   WebServer::ConnectionType type)
{
    return -1;
}
