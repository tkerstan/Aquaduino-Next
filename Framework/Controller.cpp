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

#include "Controller.h"
#include "Aquaduino.h"
#include "Actuator.h"

Controller::Controller(const char* name)
{
    setName(name);
}

Controller::~Controller()
{
}

void Controller::allMyActuators(int8_t on)
{
    int8_t i = 0;
    int8_t actuators = 0;
    Actuator* assignedActuators[MAX_ACTUATORS] =
        { 0 };
    actuators = aquaduino->getAssignedActuators(this,
                                                assignedActuators,
                                                MAX_ACTUATORS);

    for (; i < actuators; i++)
        if (on)
            assignedActuators[i]->on();
        else
            assignedActuators[i]->off();
}
