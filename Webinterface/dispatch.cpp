/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
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

#include <Framework/WebInterface.h>
#include <SD.h>
#include <Time.h>
#include <Aquaduino.h>
#include "TemplateParser.h"

extern int8_t configCmd(WebServer* server, WebServer::ConnectionType type);

/**
 * \brief This command is triggered when the request is not the default page.
 * \param [in] server Webserver instance to use.
 * \param[in] type Request type
 * \param[in] url_path URL of the request
 * \param[in] url_tail contains the part of the URL that wasn't matched against
 *            the registered command table.
 * \param[in] tail_complete is true if the complete URL fit in url_tail,  false if
 *            part of it was lost because the buffer was too small.
 *
 * This command dispatches the request based on its URL.
 *
 * For the Aquaduino configuration webpage the URL "config" is hardcoded here
 * and the configCmd is called when the url_path matches this string.
 * Actuator and Controller URLs are compared to url_path. When they match
 * the showWebinterface of the Actuator or Controller is called.
 *
 */
void controllerDispatchCommand(WebServer &server,
                               WebServer::ConnectionType type, char **url_path,
                               char *url_tail, bool tail_complete)
{
    Controller* controller;
    Actuator* actuator;

    if (type != WebServer::HEAD)
    {
        if (strcmp("config", *url_path) == 0)
        {
            configCmd(&server, type);
            return;
        }

        aquaduino->resetControllerIterator();
        while (aquaduino->getNextController(&controller) != -1)
        {
            if (strstr(*url_path, controller->getURL()) == *url_path)
            {
                controller->showWebinterface(&server, type, *url_path);
                if (type == WebServer::POST)
                {
                    aquaduino->writeConfig(controller);
                }
            }
        }

        aquaduino->resetActuatorIterator();
        while (aquaduino->getNextActuator(&actuator) != -1)
        {
            if (strstr(*url_path, actuator->getURL()) == *url_path)
            {
                actuator->showWebinterface(&server, type, *url_path);
                if (type == WebServer::POST)
                {
                    aquaduino->writeConfig(actuator);
                }
            }
        }
    }
}
