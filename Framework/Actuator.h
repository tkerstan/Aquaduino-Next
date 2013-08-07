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
#include "FrameworkConfig.h"
#include "Controller.h"
#include "Serializable.h"
#include "WebInterface.h"

class Controller;

/**
 * \brief Abstract class for common actuator methods.
 *
 * This class provides the basic functionality of common actuators like turning
 * them on and off or if possible control them using a PWM signal.
 *
 * All Actuators must implement the Serializable and WebInterface interfaces to
 * provide access to the configuration of the actuator.
 *
 * An actuator can be assigned to a single controller. This relationship is
 * stored in the Actuator object as an index. The Aquaduino class itself stores
 * the controller in an ArrayMap. The index of a controller within this map is
 * stored here. The index can then be stored directly in a configuration file
 * by the ConfigManager.
 */
class Actuator: public Object, public Serializable, public WebInterface
{
private:
    Actuator(const Actuator&);
    Actuator(Actuator&);

protected:
    int8_t m_ControlledBy;
    int8_t m_locked;

    virtual ~Actuator();

public:
    Actuator(const char* name);

    void setController(int8_t controller);
    int8_t getController();

    virtual void lock();
    virtual void unlock();
    virtual int8_t isLocked();

    /**
     * \brief Activates the actuator.
     *
     * Actuator shall not be activated when locked.
     * Needs to be implemented by derived class.
     */
    virtual void on() = 0;

    /**
     * \brief Disables the actuator.
     *
     * Actuator shall not be deactivated when locked.
     * Needs to be implemented by derived class.
     */
    virtual void off() = 0;

    /**
     * \brief Activates the actuator even in locked state.
     *
     * Needs to be implemented by derived class.
     */
    virtual void forceOn() = 0;

    /**
     * \brief Disables the actuator even in locked state.
     *
     * Needs to be implemented by derived class.
     */
    virtual void forceOff() = 0;

    /**
     * \brief Checks whether this actuator is enabled or not.
     *
     * Needs to be implemented by derived class.
     */
    virtual int8_t isOn() = 0;

    /**
     * \brief Checks for PWM support.
     *
     * Needs to be implemented by derived class.
     */
    virtual int8_t supportsPWM() = 0;

    /**
     * \brief Sets PWM duty cycle.
     * \param[in] dutyCycle PWM duty cycle.
     *
     * Needs to be implemented by derived class.
     */
    virtual void setPWM(float dutyCycle) = 0;

    /**
     * \brief Gets the current PWM duty cycle this actuator operates with.
     *
     * \returns Current PWM duty cycle.
     */
    virtual float getPWM() = 0;

};

#endif /* AQUADUINOACTOR_H_ */
