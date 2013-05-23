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

class LevelController: public Controller
{
private:
    Actuator* myActor;
    uint8_t myPin;
    int16_t debounceDelayHigh;
    int16_t debounceDelayLow;
    int16_t refillTimeout;
    int8_t lastState;
    int8_t state;
    unsigned long lastTime;

public:
    LevelController(const char* name, uint8_t pin);

    virtual uint16_t serialize(void* buffer, uint16_t size);
    virtual uint16_t deserialize(void* data, uint16_t size);

    virtual int8_t run();
    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type,
                                    char* url);
};

#endif /* LEVELCONTROLLER_H_ */
