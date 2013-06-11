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

#ifndef CLOCKTIMER_H_
#define CLOCKTIMER_H_

#include <Arduino.h>
#include <Framework/Serializable.h>
#include <Framework/Config.h>

const static uint8_t max_timers = CLOCKTIMER_MAX_TIMERS;

/**
 * \brief Implementation of a clocktimer to allow for time based control of
 * actuators
 *
 * Each clocktimer has up to #CLOCKTIMER_MAX_TIMERS entries.
 */
class ClockTimer: public Serializable
{
public:
    ClockTimer();
    virtual ~ClockTimer();

    void setTimer(uint8_t index, uint8_t hOn, uint8_t mOn, uint8_t hOff,
                  uint8_t mOff);
    void getTimer(uint8_t, uint8_t* hOn, uint8_t* mOn, uint8_t* hOff,
                  uint8_t* mOff);
    uint8_t getHourOn(uint8_t index)
    {
        return hOn[index % max_timers];
    }
    uint8_t getMinuteOn(uint8_t index)
    {
        return mOn[index % max_timers];
    }
    uint8_t getHourOff(uint8_t index)
    {
        return hOff[index % max_timers];
    }
    uint8_t getMinuteOff(uint8_t index)
    {
        return mOff[index % max_timers];
    }
    void setHourOn(uint8_t index, uint8_t value)
    {
        hOn[index % max_timers] = value % 24;
    }
    void setMinuteOn(uint8_t index, uint8_t value)
    {
        mOn[index % max_timers] = value % 60;
    }
    void setHourOff(uint8_t index, uint8_t value)
    {
        hOff[index % max_timers] = value % 24;
    }
    void setMinuteOff(uint8_t index, uint8_t value)
    {
        mOff[index % max_timers] = value % 60;
    }
    void clearAll();

    int8_t check();

    virtual uint16_t serialize(void* buffer, uint16_t size);
    virtual uint16_t deserialize(void* data, uint16_t size);

private:
    ClockTimer(const ClockTimer&);
    ClockTimer(ClockTimer&);

    uint8_t hOn[max_timers];
    uint8_t hOff[max_timers];
    uint8_t mOn[max_timers];
    uint8_t mOff[max_timers];
};

#endif /* CLOCKTIMER_H_ */
