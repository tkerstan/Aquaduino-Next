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
ClockTimer::ClockTimer() :
        m_DOW(0)
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
    this->m_HOn[index % max_timers] = hOn;
    this->m_MOn[index % max_timers] = mOn;
    this->m_HOff[index % max_timers] = hOff;
    this->m_MOff[index % max_timers] = mOff;
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
        *hOn = this->m_HOn[index % max_timers];
    if (mOn != NULL)
        *mOn = this->m_MOn[index % max_timers];
    if (hOff != NULL)
        *hOff = this->m_HOff[index % max_timers];
    if (mOff != NULL)
        *mOff = this->m_MOff[index % max_timers];
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
    unsigned long nowInSecs;
    unsigned long clockTimerOnInSecs;
    unsigned long clockTimerOffInSecs;

    switch (weekday())
    {
    case dowMonday:
        if (!isMondayEnabled())
            return 0;
        break;
    case dowTuesday:
        if (!isTuesdayEnabled())
            return 0;
        break;
    case dowWednesday:
        if (!isWednesdayEnabled())
            return 0;
        break;
    case dowThursday:
        if (!isThursdayEnabled())
            return 0;
        break;
    case dowFriday:
        if (!isFridayEnabled())
            return 0;
        break;
    case dowSaturday:
        if (!isSaturdayEnabled())
            return 0;
        break;
    case dowSunday:
        if (!isSundayEnabled())
            return 0;
        break;
    default:
        return 0;
        break;
    }

    nowInSecs = hour() * SECS_PER_HOUR + minute() * SECS_PER_MIN + second();

    for (int i = 0; i < max_timers; i++)
    {
        clockTimerOnInSecs = this->m_HOn[i] * SECS_PER_HOUR
                + this->m_MOn[i] * SECS_PER_MIN;
        clockTimerOffInSecs = this->m_HOff[i] * SECS_PER_HOUR
                + this->m_MOff[i] * SECS_PER_MIN;

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
        this->m_HOn[i] = 0;
        this->m_HOff[i] = 0;
        this->m_MOn[i] = 0;
        this->m_MOff[i] = 0;
    }
}

uint16_t ClockTimer::serialize(void* buffer, uint16_t size)
{
    uint16_t mySize = sizeof(m_HOn) + sizeof(m_MOn) + sizeof(m_HOff)
                      + sizeof(m_MOff) + sizeof(m_DOW);
    uint16_t pos = 0;
    uint8_t* charBuffer = (uint8_t*) buffer;

    if (mySize > size)
        return 0;

    memcpy(charBuffer, m_HOn, sizeof(m_HOn));
    pos += sizeof(m_HOn);
    memcpy(charBuffer + pos, m_MOn, sizeof(m_MOn));
    pos += sizeof(m_MOn);
    memcpy(charBuffer + pos, m_HOff, sizeof(m_HOff));
    pos += sizeof(m_HOff);
    memcpy(charBuffer + pos, m_MOff, sizeof(m_MOff));
    pos += sizeof(m_MOff);
    memcpy(charBuffer + pos, &m_DOW, sizeof(m_DOW));
    pos += sizeof(m_DOW);
    return mySize;
}

uint16_t ClockTimer::deserialize(void* data, uint16_t size)
{
    uint16_t mySize = sizeof(m_HOn) + sizeof(m_MOn) + sizeof(m_HOff)
                      + sizeof(m_MOff) + sizeof(m_DOW);
    uint16_t pos = 0;
    uint8_t* charBuffer = (uint8_t*) data;

    if (size != mySize)
        return 0;

    memcpy(m_HOn, charBuffer, sizeof(m_HOn));
    pos += sizeof(m_HOn);
    memcpy(m_MOn, charBuffer + pos, sizeof(m_MOn));
    pos += sizeof(m_MOn);
    memcpy(m_HOff, charBuffer + pos, sizeof(m_HOff));
    pos += sizeof(m_HOff);
    memcpy(m_MOff, charBuffer + pos, sizeof(m_MOff));
    pos += sizeof(m_MOff);
    memcpy(&m_DOW, charBuffer + pos, sizeof(m_DOW));
    pos += sizeof(m_DOW);
    return mySize;
}
