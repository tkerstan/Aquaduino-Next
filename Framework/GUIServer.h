/*
 * GUIServer.h
 *
 *  Created on: 12.03.2014
 *      Author: Timo
 */

#ifndef GUISERVER_H_
#define GUISERVER_H_

#include <EthernetUdp.h>

class GUIServer
{
public:
    GUIServer(uint16_t port);

    void run();

protected:
    virtual ~GUIServer();
private:
    int8_t receiveCommand();
    uint8_t m_Buffer[50];
    uint16_t m_Port;
    EthernetUDP m_UdpServer;
};

#endif /* GUISERVER_H_ */
