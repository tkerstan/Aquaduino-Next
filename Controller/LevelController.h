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

#ifndef LEVELCONTROLLER_H_
#define LEVELCONTROLLER_H_

#include <Framework/Controller.h>
#include <Framework/Actuator.h>

/**
 * \brief Water level controller
 *
 * This controller is responsible to monitor the water level. When it detects
 * the water level is low it enables its assigned actuators to perform a refill.
 *
 * The controllers behaviour is configured by LevelController::debounceDelayHigh,
 * LevelController::hysteresis and LevelController::refillTimeOut. All values
 * are interpreted in seconds. LevelController::debounceDelayHigh defines the
 * minimum time that needs to pass before the refill process starts. This is
 * useful to filter out waves detected by the level sensor.
 * LevelController::hysteresis defines the time the refill process keeps
 * running after a detection of a finished refill process by the level sensor.
 *
 */
class LevelController: public Controller
{
public:
    LevelController(const char* name);

    virtual uint16_t serialize(Stream* s);
    virtual uint16_t deserialize(Stream* s);

    virtual int8_t run();

    int8_t setDelayHigh(int16_t delayHigh);
    int16_t getDelayHigh();

    int8_t setDelayLow(int16_t delayLow);
    int16_t getDelayLow();

    int8_t setTimeout(int16_t timeout);
    int16_t getTimeout();

    int8_t reset();

    int8_t assignSensor(int8_t sensorIdx);
    int8_t getAssignedSensor();

    int8_t getState();

private:
    int16_t m_Delayh;
    int16_t m_Delayl;
    int16_t m_Timeout;
    int8_t m_State;
    int8_t m_Sensor;
};

#endif /* LEVELCONTROLLER_H_ */
