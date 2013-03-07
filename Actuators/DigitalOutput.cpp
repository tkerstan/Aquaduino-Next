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

DigitalOutput::DigitalOutput(const char* name, int8_t pin, uint8_t onValue,
                             uint8_t offValue) :
        Actuator(name)
{
    m_Type = ACTUATOR_DIGITALOUTPUT;
    this->pin = pin;
    pinMode(pin, OUTPUT);
    this->onValue = onValue;
    this->offValue = offValue;
    this->m_Enabled = true;
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
    if (m_Enabled)
        digitalWrite(pin, onValue);
}

void DigitalOutput::off()
{
    if (m_Enabled)
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
