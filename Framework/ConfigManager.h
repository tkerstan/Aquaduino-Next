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

/**
 * \brief This structure is used to represent the configuration data of
 * Aquaduino components.
 */
struct configuration
{
    /**
     * \brief Name of the component
     */
    char name[AQUADUINO_STRING_LENGTH];

    /**
     * \brief type of the component
     *
     * The type can be used to identify whether the data is suitable for
     * deserialization in a specific component.
     */
    uint8_t objectType;

    /**
     * \brief Index of the associated controller.
     *
     * This field can be used to store the associated controller of a component
     */
    int8_t controllerIdx;

    /**
     * \brief Index of the associated actuator.
     *
     * This field can be used to store the associated actuator of a component.
     * Not used yet.
     */
    int8_t actuatorIdx;

    /**
     * \brief Index of the associated sensor.
     *
     * This field can be used to store the associated sensor of a component.
     * Not used yet.
     */
    int8_t sensorIdx;

    /**
     * \brief Buffer for user data
     *
     * This buffer may hold the user data of the component. Only the component
     * itself can interpret it using its Serializable::deserialize method. It is filled
     * using the Serializable::serialize method of the component.
     *
     * The size is specified by the macro #SERIALIZATION_BUFFER
     */
    char data[bufferSize];
};

/**
 * \brief Interface for managing the configuration of the classes Aqaduino,
 * Actuator, Controller an Sensor.
 *
 * Provides interface methods for reading and writing configurations.
 */
class ConfigManager
{
public:

    /**
     * \brief Writes the configuration of an Aquaduino object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t writeConfig(Aquaduino* aquaduino) = 0;

    /**
     * \brief Writes the configuration of an Actuator object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t writeConfig(Actuator* actuator) = 0;

    /**
     * \brief Writes the configuration of an Controller object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t writeConfig(Controller* controller) = 0;

    /**
     * \brief Writes the configuration of an Sensor object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t writeConfig(Sensor* sensor) = 0;

    /**
     * \brief Writes the configuration of an Aquaduino object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t readConfig(Aquaduino* aquaduino) = 0;

    /**
     * \brief Writes the configuration of an Actuator object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t readConfig(Actuator* actuator) = 0;

    /**
     * \brief Writes the configuration of an Controller object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t readConfig(Controller* controller) = 0;

    /**
     * \brief Writes the configuration of an Sensor object.
     *
     * Implementing class needs to implement this.
     */
    virtual int8_t readConfig(Sensor* sensor) = 0;
};

#endif /* CONFIGMANAGER_H_ */
