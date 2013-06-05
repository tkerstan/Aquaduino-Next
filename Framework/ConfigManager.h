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

#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#include <Arduino.h>
#include <Aquaduino.h>
#include <Framework/Actuator.h>
#include <Framework/Controller.h>
#include <Framework/Sensor.h>

static const uint16_t bufferSize = SERIALIZATION_BUFFER;

class Aquaduino;

struct configuration
{
    char name[AQUADUINO_STRING_LENGTH];
    uint8_t objectType;
    int8_t controllerIdx;
    int8_t actuatorIdx;
    int8_t sensorIdx;
    char data[bufferSize];
};

class ConfigManager
{
public:

    virtual int8_t writeConfig(Aquaduino* aquaduino) = 0;
    virtual int8_t writeConfig(Actuator* actuator) = 0;
    virtual int8_t writeConfig(Controller* controller) = 0;
    virtual int8_t writeConfig(Sensor* sensor) = 0;

    virtual int8_t readConfig(Aquaduino* aquaduino) = 0;
    virtual int8_t readConfig(Actuator* actuator) = 0;
    virtual int8_t readConfig(Controller* controller) = 0;
    virtual int8_t readConfig(Sensor* sensor) = 0;
};

#endif /* CONFIGMANAGER_H_ */
