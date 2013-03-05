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

#include <Aquaduino.h>
#include "ClockTimerController.h"
#include <Time.h>
#include <SD.h>
#include <TemplateParser.h>

ClockTimerController::ClockTimerController(const char* name) :
		Controller(name)
{
	m_Type = CONTROLLER_CLOCKTIMER;
}

uint16_t ClockTimerController::serialize(void* buffer, uint16_t size)
{
}

uint16_t ClockTimerController::deserialize(void* data, uint16_t size)
{
}

int8_t ClockTimerController::run()
{
	return 0;
}

int8_t ClockTimerController::showWebinterface(WebServer* server,
		WebServer::ConnectionType type)
{
	return 0;
}
