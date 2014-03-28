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

#ifndef WEBINTERFACE_H_
#define WEBINTERFACE_H_

#include <Arduino.h>

#ifdef FEATURE_WEBIF
#define WEBDUINO_NO_IMPLEMENTATION
#include <WebServer.h>
#undef WEBDUINO_NO_IMPLEMENTATION
#endif

/**
 * \brief Interface to access the Webduino webserver
 *
 * This interface provides access to the webserver when a class needs to
 * present its webpage.
 */
class WebInterface
{
public:
#ifdef FEATURE_WEBIF
    /**
     * \brief Callback for webpage presentation
     * \param[in] server Instance of the Webduino webserver
     * \param[in] type Type of the request (GET, POST,...)
     * \param[in] url URL used in request. Allows for subURL decoding.
     *
     * This method needs to be implemented by the class providing this
     * interface to present its webpage using the Webduino webserver.
     *
     */
    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type,
                                    char* url) = 0;
#endif
};

#endif /* WEBINTERFACE_H_ */
