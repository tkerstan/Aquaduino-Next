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
#include <Framework/Flashvars.h>

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

uint16_t DigitalInput::serialize(void* buffer, uint16_t size)
{
    memcpy(buffer, &m_Pin, sizeof(m_Pin));
    return sizeof(m_Pin);
}

uint16_t DigitalInput::deserialize(void* data, uint16_t size)
{
    memcpy(&m_Pin, data, sizeof(m_Pin));
    pinMode(m_Pin, INPUT);
    return sizeof(m_Pin);
}

void  DigitalInput::setPin(uint8_t pin)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[template_digitalinput_fnsize];
    strcpy_P(templateFileName, template_digitalinput_fname);
}

uint8_t DigitalInput::getPin()
{
    return m_Pin;
}

