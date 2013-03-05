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

const static uint8_t CLOCKTIMER_MAX_TIMERS = 4;

class ClockTimer
{
	uint8_t hOn[CLOCKTIMER_MAX_TIMERS];
	uint8_t hOff[CLOCKTIMER_MAX_TIMERS];
	uint8_t mOn[CLOCKTIMER_MAX_TIMERS];
	uint8_t mOff[CLOCKTIMER_MAX_TIMERS];
public:
	ClockTimer();

	void setTimer(uint8_t index, uint8_t hOn, uint8_t mOn, uint8_t hOff,
			uint8_t mOff);
	void getTimer(uint8_t, uint8_t* hOn, uint8_t* mOn, uint8_t* hOff,
			uint8_t* mOff);
	uint8_t getHourOn(uint8_t index)
	{
		return hOn[index % CLOCKTIMER_MAX_TIMERS];
	}
	uint8_t getMinuteOn(uint8_t index)
	{
		return mOn[index % CLOCKTIMER_MAX_TIMERS];
	}
	uint8_t getHourOff(uint8_t index)
	{
		return hOff[index % CLOCKTIMER_MAX_TIMERS];
	}
	uint8_t getMinuteOff(uint8_t index)
	{
		return mOff[index % CLOCKTIMER_MAX_TIMERS];
	}
	void setHourOn(uint8_t index, uint8_t value)
	{
		hOn[index % CLOCKTIMER_MAX_TIMERS] = value % 24;
	}
	void setMinuteOn(uint8_t index, uint8_t value)
	{
		mOn[index % CLOCKTIMER_MAX_TIMERS] = value % 60;
	}
	void setHourOff(uint8_t index, uint8_t value)
	{
		hOff[index % CLOCKTIMER_MAX_TIMERS] = value % 24;
	}
	void setMinuteOff(uint8_t index, uint8_t value)
	{
		mOff[index % CLOCKTIMER_MAX_TIMERS] = value % 60;
	}
	void clearAll();

	int8_t check();
};

#endif /* CLOCKTIMER_H_ */
