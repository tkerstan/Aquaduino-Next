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

#ifndef OBJECT_H_
#define OBJECT_H_

#include <Framework/FrameworkConfig.h>
#include <Framework/ObjectTypes.h>

/**\brief Base class for all Controller, Actuators, Sensors and the Aquaduino.
 *
 * Provides common functionality to manage name, URL and type of an Aquaduino
 * component.
 */
class Object
{
public:
    Object();
    Object(const char* name, const char* url);

    void setName(const char* name);
    const char* getName();

    void setURL(const char* url);
    const char* getURL();

    virtual int16_t getType();

protected:
    virtual ~Object();
    char m_Name[AQUADUINO_STRING_LENGTH];
    char m_URL[AQUADUINO_STRING_LENGTH];
    int16_t m_Type;
};

#endif /* OBJECT_H_ */
