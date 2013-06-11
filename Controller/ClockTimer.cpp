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

/**
 * \brief Default constructor
 *
 * Clears all timers.
 */
ClockTimer::ClockTimer()
{
    clearAll();
}

/**
 * \brief Destructor
 *
 * Empty
 */
ClockTimer::~ClockTimer()
{

}

/**
 * \brief Sets a clocktimer
 * \param[in] index entry of the clocktimer
 * \param[in] hOn hour this entry shall get active
 * \param[in] mOn minute this entry shall get active
 * \param[in] hOff hour this entry shall get inactive
 * \param[in] mOff minute this entry shall get inactive
 */
void ClockTimer::setTimer(uint8_t index, uint8_t hOn, uint8_t mOn, uint8_t hOff,
                          uint8_t mOff)
{
    this->hOn[index % max_timers] = hOn;
    this->mOn[index % max_timers] = mOn;
    this->hOff[index % max_timers] = hOff;
    this->mOff[index % max_timers] = mOff;
}

/**
 * \brief Returns a clocktimer entry
 * \param[in] index entry of the clocktimer
 * \param[out] hOn Pointer to an uint8_t to hold the hour this entry shall
 * get active
 * \param[out] mOn Pointer to an uint8_t to hold the minute this entry shall
 * get active
 * \param[out] hOff Pointer to an uint8_t to hold the hour this entry shall
 * get inactive
 * \param[out] mOff Pointer to an uint8_t to hold the minute this entry shall
 * get inactive
 */
void ClockTimer::getTimer(uint8_t index, uint8_t* hOn, uint8_t* mOn,
                          uint8_t* hOff, uint8_t* mOff)
{
    if (hOn != NULL)
        *hOn = this->hOn[index % max_timers];
    if (mOn != NULL)
        *mOn = this->mOn[index % max_timers];
    if (hOff != NULL)
        *hOff = this->hOff[index % max_timers];
    if (mOff != NULL)
        *mOff = this->mOff[index % max_timers];
}

/**
 * \brief Checks the clocktimer
 *
 * Checks the clocktimer for active entries.
 *
 * \returns 1 if at least on entry is active. 0 otherwise.
 */
int8_t ClockTimer::check()
{
    uint8_t on = 0;
    unsigned long nowInSecs = hour() * SECS_PER_HOUR + minute() * SECS_PER_MIN
                              + second();

    for (int i = 0; i < max_timers; i++)
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

/**
 * \brief Clear all entries
 */
void ClockTimer::clearAll()
{
    for (int i = 0; i < max_timers; i++)
    {
        this->hOn[i] = 0;
        this->hOff[i] = 0;
        this->mOn[i] = 0;
        this->mOff[i] = 0;
    }
}

uint16_t ClockTimer::serialize(void* buffer, uint16_t size)
{
    uint16_t mySize = sizeof(hOn) + sizeof(mOn) + sizeof(hOff) + sizeof(mOff);
    uint16_t pos = 0;
    uint8_t* charBuffer = (uint8_t*) buffer;

    if (mySize > size)
        return 0;

    memcpy(charBuffer, hOn, sizeof(hOn));
    pos += sizeof(hOn);
    memcpy(charBuffer + pos, mOn, sizeof(mOn));
    pos += sizeof(mOn);
    memcpy(charBuffer + pos, hOff, sizeof(hOff));
    pos += sizeof(hOff);
    memcpy(charBuffer + pos, mOff, sizeof(mOff));

    return mySize;
}

uint16_t ClockTimer::deserialize(void* data, uint16_t size)
{
    uint16_t mySize = sizeof(hOn) + sizeof(mOn) + sizeof(hOff) + sizeof(mOff);
    uint16_t pos = 0;
    uint8_t* charBuffer = (uint8_t*) data;

    if (size != mySize)
        return 0;

    memcpy(hOn, charBuffer, sizeof(hOn));
    pos += sizeof(hOn);
    memcpy(mOn, charBuffer + pos, sizeof(mOn));
    pos += sizeof(mOn);
    memcpy(hOff, charBuffer + pos, sizeof(hOff));
    pos += sizeof(hOff);
    memcpy(mOff, charBuffer + pos, sizeof(mOff));

    return mySize;
}
