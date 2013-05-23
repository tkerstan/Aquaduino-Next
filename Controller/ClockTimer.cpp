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

#include "ClockTimer.h"
#include <Time.h>

ClockTimer::ClockTimer()
{
    clearAll();
}

void ClockTimer::setTimer(uint8_t index, uint8_t hOn, uint8_t mOn, uint8_t hOff,
                          uint8_t mOff)
{
    this->hOn[index % CLOCKTIMER_MAX_TIMERS] = hOn;
    this->mOn[index % CLOCKTIMER_MAX_TIMERS] = mOn;
    this->hOff[index % CLOCKTIMER_MAX_TIMERS] = hOff;
    this->mOff[index % CLOCKTIMER_MAX_TIMERS] = mOff;
}

void ClockTimer::getTimer(uint8_t index, uint8_t* hOn, uint8_t* mOn,
                          uint8_t* hOff, uint8_t* mOff)
{
    if (hOn != NULL)
        *hOn = this->hOn[index % CLOCKTIMER_MAX_TIMERS];
    if (mOn != NULL)
        *mOn = this->mOn[index % CLOCKTIMER_MAX_TIMERS];
    if (hOff != NULL)
        *hOff = this->hOff[index % CLOCKTIMER_MAX_TIMERS];
    if (mOff != NULL)
        *mOff = this->mOff[index % CLOCKTIMER_MAX_TIMERS];
}

int8_t ClockTimer::check()
{
    uint8_t on = 0;
    unsigned long nowInSecs = hour() * SECS_PER_HOUR + minute() * SECS_PER_MIN
                              + second();

    for (int i = 0; i < CLOCKTIMER_MAX_TIMERS; i++)
    {
        unsigned long clockTimerOnInSecs = this->hOn[i] * SECS_PER_HOUR
                + this->mOn[i] * SECS_PER_MIN;
        unsigned long clockTimerOffInSecs = this->hOff[i] * SECS_PER_HOUR
                + this->mOff[i] * SECS_PER_MIN;

        // Check whether entry is not hh:mm - hh:mm
        if (clockTimerOnInSecs - clockTimerOffInSecs != 0)
        {
            // e.g. 08:00 - 15:00
            if (clockTimerOnInSecs < clockTimerOffInSecs)
            {
                on |= (nowInSecs >= clockTimerOnInSecs && nowInSecs
                        < clockTimerOffInSecs) ? 1 : 0;
                // e.g. 21:00 - 06:00
            }
            else
            {
                on |= (nowInSecs >= clockTimerOnInSecs || nowInSecs
                        < clockTimerOffInSecs) ? 1 : 0;
            }
        }
    }

    return on;

}

void ClockTimer::clearAll()
{
    for (int i = 0; i < CLOCKTIMER_MAX_TIMERS; i++)
    {
        this->hOn[i] = 0;
        this->hOff[i] = 0;
        this->mOn[i] = 0;
        this->mOff[i] = 0;
    }
}

