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
#include <Framework/FrameworkConfig.h>

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
        return m_HOn[index % max_timers];
    }

    uint8_t getMinuteOn(uint8_t index)
    {
        return m_MOn[index % max_timers];
    }

    uint8_t getHourOff(uint8_t index)
    {
        return m_HOff[index % max_timers];
    }

    uint8_t getMinuteOff(uint8_t index)
    {
        return m_MOff[index % max_timers];
    }

    void setHourOn(uint8_t index, uint8_t value)
    {
        m_HOn[index % max_timers] = value % 24;
    }

    void setMinuteOn(uint8_t index, uint8_t value)
    {
        m_MOn[index % max_timers] = value % 60;
    }

    void setHourOff(uint8_t index, uint8_t value)
    {
        m_HOff[index % max_timers] = value % 24;
    }

    void setMinuteOff(uint8_t index, uint8_t value)
    {
        m_MOff[index % max_timers] = value % 60;
    }

    void enableMonday()
    {
        m_DOW |= 0x1;
    }

    void enableTuesday()
    {
        m_DOW |= 0x2;
    }

    void enableWednesday()
    {
        m_DOW |= 0x4;
    }

    void enableThursday()
    {
        m_DOW |= 0x8;
    }

    void enableFriday()
    {
        m_DOW |= 0x10;
    }

    void enableSaturday()
    {
        m_DOW |= 0x20;
    }

    void enableSunday()
    {
        m_DOW |= 0x40;
    }

    void enableAllDays()
    {
        m_DOW = 0x7F;
    }

    void disableMonday()
    {
        m_DOW &= ~0x1;
    }

    void disableTuesday()
    {
        m_DOW &= ~0x2;
    }

    void disableWednesday()
    {
        m_DOW &= ~0x4;
    }

    void disableThursday()
    {
        m_DOW &= ~0x8;
    }

    void disableFriday()
    {
        m_DOW &= ~0x10;
    }

    void disableSaturday()
    {
        m_DOW &= ~0x20;
    }

    void disableSunday()
    {
        m_DOW &= ~0x40;
    }

    void disableAllDays()
    {
        m_DOW = 0;
    }

    int8_t isMondayEnabled()
    {
        return m_DOW & 0x1;
    }

    int8_t isTuesdayEnabled()
    {
        return m_DOW & 0x2;
    }

    int8_t isWednesdayEnabled()
    {
        return m_DOW & 0x4;
    }

    int8_t isThursdayEnabled()
    {
        return m_DOW & 0x8;
    }

    int8_t isFridayEnabled()
    {
        return m_DOW & 0x10;
    }

    int8_t isSaturdayEnabled()
    {
        return m_DOW & 0x20;
    }

    int8_t isSundayEnabled()
    {
        return m_DOW & 0x40;
    }

    void clearAll();

    int8_t check();

    virtual uint16_t serialize(void* buffer, uint16_t size);
    virtual uint16_t deserialize(void* data, uint16_t size);

private:
    ClockTimer(const ClockTimer&);
    ClockTimer(ClockTimer&);

    uint8_t m_HOn[max_timers];
    uint8_t m_HOff[max_timers];
    uint8_t m_MOn[max_timers];
    uint8_t m_MOff[max_timers];
    uint8_t m_DOW;
};

#endif /* CLOCKTIMER_H_ */
