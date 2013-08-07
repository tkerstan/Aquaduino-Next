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

#include "Object.h"
#include "Serializable.h"
#include "WebInterface.h"
#include "FrameworkConfig.h"

/**
 * \brief A controller realizes a control task like water level monitoring,
 * temperature monitoring,...
 *
 * The control task is implemented by overwriting the Controller::run method.
 * The Controller::run method is called by the Aquaduino main loop when
 * the controller is registered using Aqaduino::addController.
 *
 * The run methods of all controllers are called in the order they were
 * added to Aquaduino and run to completion. Thus keep in mind to keep
 * the method simple and without delays.
 *
 * When the controller shall be configurable throuh the web it needs to
 * implement the WebInterface interface.
 */
class Controller: public Object, public Serializable, public WebInterface
{
public:
    Controller(const char* name);

    /**
     * \brief Interface method for triggering the controller.
     *
     * Called by Aquaduino::run
     */
    virtual int8_t run() = 0;
    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type,
                                    char* url) = 0;

protected:
    virtual ~Controller();
    void allMyActuators(int8_t on);
    void allMyActuators(float dutyCycle);

private:
    /**
     * \brief Copy constructor
     *
     * Empty.
     */
    Controller (Controller& c){};

    /**
     * \brief Copy constructor
     *
     * Empty.
     */
    Controller (const Controller& c){};
};

#endif /* AQUADUINOCONTROLLER_H_ */
