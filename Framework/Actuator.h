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

#ifndef AQUADUINOACTOR_H_
#define AQUADUINOACTOR_H_

#include "Object.h"
#include "Config.h"
#include "Controller.h"

class Controller;

class Actuator: public Object
{
private:
    Actuator(const Actuator&);
    Actuator(Actuator&);

protected:
    int8_t m_ControlledBy;
    int8_t m_Enabled;
    virtual ~Actuator();

public:
    Actuator(const char* name);

    void setController(int8_t controller);
    int8_t getController();

    virtual void enable();
    virtual void disable();
    virtual int8_t isEnabled();

    virtual void on() = 0;
    virtual void off() = 0;
    virtual int8_t isOn() = 0;

    virtual int8_t supportsPWM() = 0;
    virtual void setPWM(float dutyCycle) = 0;
    virtual float getPWM() = 0;

};

#endif /* AQUADUINOACTOR_H_ */
