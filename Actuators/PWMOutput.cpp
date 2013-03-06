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

PWMOutput::PWMOutput(const char* name) :
        Actuator(name)
{
    m_Type = ACTUATOR_PWMOUTPUT;

    // Currently fixed at pin 7
    // TODO: Use digital_pin_to_timer!
    pinMode(7, OUTPUT);

    //Enable Fast PWM @ ~8kHZ for all OCR on Timer 4
    //This was enough for my PC Fan to run without whistling
    TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM41) | _BV(WGM40);
    TCCR4B = _BV(WGM42) | _BV(CS40);

    //No duty cycle
    dutyCycle = 0.0;
    OCR4B = dutyCycle * 0x3FF;

}

uint16_t PWMOutput::serialize(void* buffer, uint16_t size)
{

}

uint16_t PWMOutput::deserialize(void* data, uint16_t size)
{

}


void PWMOutput::on()
{
    if (m_Enabled)
    {
        setPWM(1.0);
    }
}

void PWMOutput::off()
{
    if (m_Enabled)
    {
        setPWM(0.0);
    }
}

int8_t PWMOutput::isOn()
{
    return dutyCycle < 1.0;
}

int8_t PWMOutput::supportsPWM()
{
    return true;
}

void PWMOutput::setPWM(float dC)
{
    if (m_Enabled)
    {
        //100% is enough...
        if (dC > 1.0)
            dC = 1.0;

        this->dutyCycle = 1.0 - dC;
        OCR4B = this->dutyCycle * 0x3FF;
    }
}

float PWMOutput::getPWM()
{
    return dutyCycle;
}
