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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <Framework/ObjectTypes.h>

/**
 * \brief Defines the maximum number of controllers the system can manage
 */
#define MAX_CONTROLLERS             3

/**
 * \brief Defines the maximum number of actuators the system can manage
 */
#define MAX_ACTUATORS               8

/**
 * \brief Defines the maximum number of sensors the system can manage
 */
#define MAX_SENSORS                 2

/**
 * \brief Defines the actuators being used
 */
#define ACTUATOR_CONFIG             { ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT }

/**
 * \brief Defines the controllers being used
 */
#define CONTROLLER_CONFIG           { CONTROLLER_LEVEL, \
                                      CONTROLLER_TEMPERATURE, \
                                      CONTROLLER_CLOCKTIMER } \

/**
 * \brief Defines the sensors being used
 */
#define SENSOR_CONFIG               { SENSOR_DIGITALINPUT, \
                                      SENSOR_DS18S20 }

/**
 * \brief Defines the maximum number of Clocktimers the system can manage
 */
#define MAX_CLOCKTIMERS             8

/**
 * \brief Defines the maximum number of timers per clocktimer
 */
#define CLOCKTIMER_MAX_TIMERS       4

/**
 * \brief Defines the default timezone
 */
#define TIME_ZONE                   +2

/**
 * \brief Defines the size of the serialization buffer.
 */
#define SERIALIZATION_BUFFER        200

/**
 * \brief Defines the maximum length of Strings stored in Aquaduino components
 * like Controller, Actuator, Sensor, ...
 */
#define AQUADUINO_STRING_LENGTH     20

/**
 * \brief Defines the delimiter in URLs to mark the beginning of a subURL
 */
#define URL_DELIMITER               "."

#endif /*CONFIG_H_*/
