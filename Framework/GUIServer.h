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
    void getAllSensors();
    void getSensorData(uint8_t sensorId);
    void getAllActuators();
    void getActuatorData(uint8_t actuatorId);
    void setSensorConfig(uint8_t sensorId, uint8_t type, char* value);
    void setSensorConfig(uint8_t sensorId, uint8_t type, uint8_t value);
    uint8_t m_Buffer[50];
    uint16_t m_Port;
    EthernetUDP m_UdpServer;
};

#endif /* GUISERVER_H_ */
