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

#ifndef FRAMEWORKCONFIG_H_
#define FRAMEWORKCONFIG_H_

#include <stdint.h>
#include <Framework/ObjectTypes.h>
#include <Framework/Colors.h>

/**
 * \brief Defines the maximum number of controllers the system can manage
 */
#define MAX_CONTROLLERS             8

/**
 * \brief Defines the maximum number of actuators the system can manage
 */
#define MAX_ACTUATORS               24

/**
 * \brief Defines the maximum number of sensors the system can manage
 */
#define MAX_SENSORS                 8

/**
 * \brief Defines the maximum number of Clocktimers the system can manage
 */
#define MAX_CLOCKTIMERS             24

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
#define SERIALIZATION_BUFFER        500

/**
 * \brief Defines the maximum length of Strings stored in Aquaduino components
 * like Controller, Actuator, Sensor, ...
 */
#define AQUADUINO_STRING_LENGTH     20

/**
 * \brief Defines the maximum length for the name of a Xively channel.
 */
#define XIVELY_CHANNEL_NAME_LENGTH  20

/**
 * \brief Defines the maximum length for the API Key of Xively.
 */
#define XIVELY_API_KEY_LENGTH       51

/**
 * \brief Defines the maximum length for the feed name of Xively.
 */
#define XIVELY_FEED_NAME_LENGTH     21

/**
 * \brief Defines the delimiter in URLs to mark the beginning of a subURL
 */
#define URL_DELIMITER               "."

/**
 * \brief Defines the maximum number of OneWire devices
 */
#define MAX_ONEWIRE_DEVICES         8

/**
 * \brief Defines the number of temperature values kept in a history to
 * smoothen temperature sensor readings.
 */
#define TEMP_HISTORY                10

/**
 * \brief Enables/Disabled Interrupt driven mode. Note concurrent HW accesses
 * are note protected! Usage of SPI in Controllers may lead to not deterministic
 * behavior.
 */
#undef INTERRUPT_DRIVEN

#endif /*CONFIG_H_*/
