/*
 * GUIServer.cpp
 *
 *  Created on: 12.03.2014
 *      Author: Timo
 */

#include <Framework/GUIServer.h>
#include <Arduino.h>

enum
{
    GETALLSENSORS=1,
    GETSENSORDATA=2,
    SETSENSORCONFIG=3,
    GETALLACTUATORS=4,
    GETACTUATORDATA=5,
    SETACTUATORDATA=6
};

GUIServer::GUIServer(uint16_t port)
{
    m_Port = port;
    m_UdpServer.begin(m_Port);
}

GUIServer::~GUIServer()
{
}

int8_t GUIServer::receiveCommand()
{
    if (!m_UdpServer.parsePacket())
    {
        return 0;
    }

    Serial.print(F("UDP Packet of "));
    Serial.print(m_UdpServer.available());
    Serial.println(F(" Bytes available"));
    Serial.print(F("Got "));
    Serial.print(m_UdpServer.read(m_Buffer, sizeof(m_Buffer)));
    Serial.println(F(" Bytes"));
    return 1;
}

void GUIServer::run()
{
    if (receiveCommand())
    {
        m_UdpServer.beginPacket(m_UdpServer.remoteIP(), m_UdpServer.remotePort());
        m_UdpServer.write(m_Buffer[1]);
        Serial.print(F("Request ID: "));
        Serial.println(m_Buffer[1]);
        Serial.print(F("Method ID: "));
        Serial.println(m_Buffer[0]);
        switch(m_Buffer[0])
        {
        case GETALLSENSORS:
            m_UdpServer.write("GETALLSENSORS");
            break;
        case GETSENSORDATA:
            m_UdpServer.write("GETSENSORDATA");
            break;
        case SETSENSORCONFIG:
            m_UdpServer.write("SETSENSORCONFIG");
            break;
        case GETALLACTUATORS:
            m_UdpServer.write("GETALLACTUATORS");
            break;
        case GETACTUATORDATA:
            m_UdpServer.write("GETACTUATORDATA");
            break;
        case SETACTUATORDATA:
            m_UdpServer.write("SETACTUATORDATA");
            break;
        default:
            break;
        }
        m_UdpServer.endPacket();
    }

}

