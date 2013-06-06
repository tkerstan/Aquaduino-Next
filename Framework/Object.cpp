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

/**
 * \brief Default constructor
 *
 * Empty.
 */
Object::Object()
{
}

/**
 * \brief Constructor
 * \param[in] name The name of the object.
 * \param[in] url The url of the object.
 */
Object::Object(const char* name, const char* url)
{
    setName(name);
    setURL(url);
    m_Type = 0;
}

/**
 * \brief Destructor
 *
 * Empty.
 */
Object::~Object()
{
}

/**
 * \brief Setter for the object name.
 * \param[in] name Name to be assigned
 *
 * The name is copied to the member variable of this object.
 */
void Object::setName(const char* name)
{
    strncpy(m_Name, name, sizeof(m_Name));
    m_Name[sizeof(m_Name) - 1] = 0;
}

/**
 * \brief Getter for the name of the object.
 *
 * \returns The name of this object.
 */
const char* Object::getName()
{
    return m_Name;
}

/**
 * \brief Setter for the object URL.
 * \param[in] url URL to be assigned
 *
 * The url is copied to the member variable of this object.
 */
void Object::setURL(const char* url)
{
    strncpy(m_URL, url, sizeof(m_URL));
    m_URL[sizeof(m_URL) - 1] = 0;
}

/**
 * \brief Getter for the URL of the object.
 *
 * \returns The URL of this object.
 */
const char* Object::getURL()
{
    return m_URL;
}

/**
 * \brief Getter for the type of the object.
 *
 * \returns The type of this object.
 */
int16_t Object::getType()
{
    return m_Type;
}
