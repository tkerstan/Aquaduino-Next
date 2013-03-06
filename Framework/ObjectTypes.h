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

#ifndef OBJECTTYPES_H_
#define OBJECTTYPES_H_

enum
{
    OBJECT,
    ACTUATOR_DIGITALOUTPUT,
    ACTUATOR_PWMOUTPUT,
    CONTROLLER_LEVEL,
    CONTROLLER_TEMPERATURE,
    CONTROLLER_CLOCKTIMER,
    SENSOR_DIGITALINPUT,
    SENSOR_DS18S20
};

#endif /* OBJECTTYPES_H_ */
