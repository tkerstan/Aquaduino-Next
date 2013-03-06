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

#include "DigitalOutput.h"
#include <Arduino.h>

DigitalOutput::DigitalOutput(const char* name, int pin, int onValue) :
        Actuator(name)
{
    m_Type = ACTUATOR_DIGITALOUTPUT;
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->onValue = onValue;
    this->m_Enabled = true;
    this->off();
}

uint16_t DigitalOutput::serialize(void* buffer, uint16_t size)
{
    memcpy(buffer, &onValue, sizeof(onValue));
    return 1;
}

uint16_t DigitalOutput::deserialize(void* data, uint16_t size)
{
    memcpy(&onValue, data, sizeof(onValue));
    return 1;
}

void DigitalOutput::on()
{
    if (m_Enabled)
    {
        digitalWrite(pin, onValue);
    }
}

void DigitalOutput::off()
{
    if (m_Enabled)
    {
        digitalWrite(pin, !onValue);
    }
}

int8_t DigitalOutput::isOn()
{
    if (onValue == 0)
    {
        return digitalRead(pin) == 0;
    }
    else
    {
        return digitalRead(pin) != 0;
    }

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
