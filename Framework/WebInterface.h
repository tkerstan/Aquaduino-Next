/*
 * WebInterface.h
 *
 *  Created on: 04.03.2013
 *      Author: TimoK
 */

#ifndef WEBINTERFACE_H_
#define WEBINTERFACE_H_

#include <WebServer.h>

class WebInterface{
public:
	virtual int8_t showWebinterface(WebServer* server,
				WebServer::ConnectionType type) = 0;
};


#endif /* WEBINTERFACE_H_ */
