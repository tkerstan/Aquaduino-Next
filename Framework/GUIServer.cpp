/*
 * GUIServer.cpp
 *
 *  Created on: 12.03.2014
 *      Author: Timo
 */

#include <Aquaduino.h>
#include <Framework/GUIServer.h>
#include <Arduino.h>

enum
{
    GETVERSION = 0,
    GETALLSENSORS = 1,
    GETSENSORDATA = 2,
    SETSENSORCONFIG = 3,
    GETALLACTUATORS = 4,
    GETACTUATORDATA = 5,
    SETACTUATORDATA = 6
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
        m_UdpServer.beginPacket(m_UdpServer.remoteIP(),
                                m_UdpServer.remotePort());
        m_UdpServer.write(m_Buffer[0]);
        m_UdpServer.write(m_Buffer[1]);

        Serial.print(F("Request ID: "));
        Serial.println(m_Buffer[1]);
        Serial.print(F("Method ID: "));
        Serial.println(m_Buffer[0]);
        switch (m_Buffer[0])
        {
        case GETVERSION:
            m_UdpServer.write((uint8_t) 0);
            m_UdpServer.write(1);
            break;
        case GETALLSENSORS:
            getAllSensors();
            break;
        case GETSENSORDATA:
            getSensorData(m_Buffer[2]);
            break;
        case SETSENSORCONFIG:
            m_UdpServer.write("SETSENSORCONFIG");
            break;
        case GETALLACTUATORS:
            getAllActuators();
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
void GUIServer::getAllSensors()
{
    //errorcode 0
    m_UdpServer.write((uint8_t) 0);
    //num of sensors
    m_UdpServer.write((uint8_t) __aquaduino->getNrOfSensors());
    //Serial.println("NrOfSensors()");
   // Serial.println(__aquaduino->getNrOfSensors());
    //sensor information
    Sensor* sensor;
    __aquaduino->resetSensorIterator();
    while (__aquaduino->getNextSensor(&sensor) != -1)
    {
        m_UdpServer.write(__aquaduino->getSensorID(sensor));
        m_UdpServer.write(sensor->getType());
        //
        m_UdpServer.write(24);
        m_UdpServer.write("unit not implemented yet");
        //visible
        m_UdpServer.write(true);
        //calibrationInterval(days):int
        m_UdpServer.write((uint8_t) 0);
    }
}
void GUIServer::getSensorData(uint8_t sensorId)
{
    Sensor* sensor =__aquaduino->getSensor(sensorId);

    //errorcode 0
    m_UdpServer.write((uint8_t) 0);

    //
    m_UdpServer.write(__aquaduino->getSensorValue(sensorId));
    //valueMax24h:float
    m_UdpServer.write(0.04);
    //valueMax24hTime:time
    m_UdpServer.write((uint8_t) 0);
    //valueMin24h:float
    m_UdpServer.write(0.01);
    //valueMin24hTime:time
    m_UdpServer.write((uint32_t) 1395867979);
    //lastCalibration:dateTime
    m_UdpServer.write((uint32_t) 1395867979);
    //operatingHours
    m_UdpServer.write((uint8_t) 0);
    //lastOperatingHoursReset
    m_UdpServer.write((uint32_t) 1395867979);

}
void GUIServer::getAllActuators()
{
    //errorcode 0
    m_UdpServer.write((uint8_t) 0);
    //num of actuators
    m_UdpServer.write((uint8_t) __aquaduino->getNrOfActuators());
    //sensor information
    Actuator* actuator;
    __aquaduino->resetActuatorIterator();
    while (__aquaduino->getNextActuator(&actuator) != -1)
    {
        m_UdpServer.write(__aquaduino->getActuatorID(actuator));
        m_UdpServer.write(strlen(actuator->getName()));
        m_UdpServer.write(actuator->getName());
        //influencesStream:bool
        m_UdpServer.write((uint8_t) 0);
        //influencesHeat:bool
        m_UdpServer.write((uint8_t) 0);
        //ControllerSemanticValue:int
        m_UdpServer.write((uint8_t) 0);
        //calibrationInterval(days):int
        m_UdpServer.write((uint8_t) 0);
    }
}

void GUIServer::getActuatorData(uint8_t actuatorId)
{
    Actuator* actuator =__aquaduino->getActuator(actuatorId);

        //errorcode 0
        m_UdpServer.write((uint8_t) 0);

        //value:int
        m_UdpServer.write((uint8_t) 0);
        //operatingHours:int
        m_UdpServer.write((uint8_t) 4010);
        //lastOperatingHoursReset:dateTime
        m_UdpServer.write((uint32_t) 1395867979);
        //lastCalibration:dateTime
        m_UdpServer.write((uint32_t) 1395867979);
}

/*
void GUIServer::setSensorConfig
void GUIServer::setActuatorData
*/








