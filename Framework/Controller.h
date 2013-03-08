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

#ifndef AQUADUINOCONTROLLER_H_
#define AQUADUINOCONTROLLER_H_

#define WEBDUINO_NO_IMPLEMENTATION

#include "Object.h"
#include "Serializable.h"
#include "WebInterface.h"
#include "Config.h"
#include <WebServer.h>

class Controller: public Object, public Serializable, public WebInterface
{
public:
    Controller(const char* name);

    virtual int8_t run() = 0;
    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type) = 0;

protected:
    virtual ~Controller();
    void allMyActuators(int8_t on);
};

#endif /* AQUADUINOCONTROLLER_H_ */
