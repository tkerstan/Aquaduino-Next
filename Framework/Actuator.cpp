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

#include "Actuator.h"

Actuator::Actuator(const char* name) :
        m_locked(1)
{
    m_ControlledBy = -1;
    setName(name);
}

Actuator::~Actuator()
{
}

void Actuator::setController(int8_t controller)
{
    this->m_ControlledBy = controller;
}

int8_t Actuator::getController()
{
    return m_ControlledBy;
}

void Actuator::lock()
{
    m_locked = true;
}

void Actuator::unlock()
{
    m_locked = false;
}

int8_t Actuator::isLocked()
{
    return m_locked;
}
