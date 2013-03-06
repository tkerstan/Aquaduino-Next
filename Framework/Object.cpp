/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
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

#include "Object.h"
#include "string.h"

Object::Object()
{
}

Object::Object(const char* name, const char* url)
{
    setName(name);
    setURL(url);
    m_Type = 0;
}

Object::~Object()
{
}

void Object::setName(const char* name)
{
    strncpy(m_Name, name, sizeof(m_Name));
    m_Name[sizeof(m_Name) - 1] = 0;
}

const char* Object::getName()
{
    return m_Name;
}

void Object::setURL(const char* url)
{
    strncpy(m_URL, url, sizeof(m_URL));
    m_URL[sizeof(m_URL) - 1] = 0;
}

const char* Object::getURL()
{
    return m_URL;
}

int16_t Object::getType()
{
    return m_Type;
}
