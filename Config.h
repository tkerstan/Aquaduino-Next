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
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT, \
                                      ACTUATOR_DIGITALOUTPUT }

#define ACTUATORS                     16

/**
 * \brief Defines the controllers being used
 */
#define CONTROLLER_CONFIG           { CONTROLLER_LEVEL, \
                                      CONTROLLER_TEMPERATURE, \
                                      CONTROLLER_CLOCKTIMER }

#define CONTROLLERS                   3

/**
 * \brief Defines the sensors being used
 */
#define SENSOR_CONFIG               { SENSOR_SERIALINPUTPH, \
		                              SENSOR_SERIALINPUTEC, \
		                              SENSOR_SERIALINPUTORP, \
                                      SENSOR_DS18S20 }
#endif


#define SENSORS                       4
