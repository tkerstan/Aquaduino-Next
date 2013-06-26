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

#ifndef AQUADUINOSENSOR_H_
#define AQUADUINOSENSOR_H_

#include "Object.h"
#include "Serializable.h"
#include "WebInterface.h"

/**
 * \brief Base class for Sensors
 *
 * Sensors shall provide a read method to read their values. This class may
 * also derive from Serializable and WebInterface when there is need for
 * configuration of sensors.
 */
class Sensor: public Object, public WebInterface, public Serializable
{
public:
    Sensor();
    virtual double read() = 0;

protected:
    virtual ~Sensor();
private:
    Sensor(Sensor&);
    Sensor(const Sensor&);
};

#endif /* AQUADUINOSENSOR_H_ */
