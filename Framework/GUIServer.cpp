/*
 * GUIServer.cpp
 *
 *  Created on: 12.03.2014
 *      Author: Timo
 */

#include <Aquaduino.h>
#include <Framework/GUIServer.h>
#include <Arduino.h>

enum {
	GETVERSION = 0,
	GETALLSENSORS = 1,
	GETSENSORDATA = 2,
	SETSENSORCONFIG = 3,
	GETALLACTUATORS = 4,
	GETACTUATORDATA = 5,
	SETACTUATORDATA = 6
};

GUIServer::GUIServer(uint16_t port) {
	m_Port = port;
	m_UdpServer.begin(m_Port);
}

GUIServer::~GUIServer() {
}

int8_t GUIServer::receiveCommand() {
	if (!m_UdpServer.parsePacket()) {
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

void GUIServer::run() {
	if (receiveCommand()) {
		m_UdpServer.beginPacket(m_UdpServer.remoteIP(),
				m_UdpServer.remotePort());
		m_UdpServer.write(m_Buffer[0]);
		m_UdpServer.write(m_Buffer[1]);

		Serial.print(F("Request ID: "));
		Serial.println(m_Buffer[1]);
		Serial.print(F("Method ID: "));
		Serial.println(m_Buffer[0]);
		switch (m_Buffer[0]) {
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
			switch (m_Buffer[3]) {
			case 1:
				break;
			case 2:
				setSensorConfig(m_Buffer[2], m_Buffer[3], (char*) m_Buffer[4]);
				break;
			case 3:
				setSensorConfig(m_Buffer[2], m_Buffer[3], (char*) m_Buffer[4]);
				break;
			case 4:
				setSensorConfig(m_Buffer[2], m_Buffer[3], (uint8_t) m_Buffer[4]);
				break;
			case 5:
				setSensorConfig(m_Buffer[2], m_Buffer[3], (uint8_t) m_Buffer[4]);
				break;
			}
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
void GUIServer::getAllSensors() {
	//errorcode 0
	m_UdpServer.write((uint8_t) 0);
	//num of sensors
	m_UdpServer.write((uint8_t) __aquaduino->getNrOfSensors());
	//Serial.println("NrOfSensors()");
	// Serial.println(__aquaduino->getNrOfSensors());
	//sensor information
	Sensor* sensor;
	__aquaduino->resetSensorIterator();
	while (__aquaduino->getNextSensor(&sensor) != -1) {
		m_UdpServer.write(__aquaduino->getSensorID(sensor));
		m_UdpServer.write(sensor->getType());
		//
		m_UdpServer.write(strlen(sensor->getName()));
		m_UdpServer.write(sensor->getName());
		//visible
		m_UdpServer.write(true);
		//calibrationInterval(days):int
		m_UdpServer.write((uint8_t) 0);
	}
}
void GUIServer::getSensorData(uint8_t sensorId) {
	Sensor* sensor = __aquaduino->getSensor(sensorId);

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
void GUIServer::getAllActuators() {
	//errorcode 0
	m_UdpServer.write((uint8_t) 0);
	//num of actuators
	m_UdpServer.write((uint8_t) __aquaduino->getNrOfActuators());
	//sensor information
	Actuator* actuator;
	__aquaduino->resetActuatorIterator();
	while (__aquaduino->getNextActuator(&actuator) != -1) {
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

void GUIServer::getActuatorData(uint8_t actuatorId) {
	Actuator* actuator = __aquaduino->getActuator(actuatorId);

	//errorcode 0
	m_UdpServer.write((uint8_t) 0);

	//value:int
	m_UdpServer.write(actuator->isOn());
	//operatingHours:int
	m_UdpServer.write((uint8_t) 0);
	//lastOperatingHoursReset:dateTime
	m_UdpServer.write((uint32_t) 1395867979);
	//lastCalibration:dateTime
	m_UdpServer.write((uint32_t) 1395867979);
}

void GUIServer::setSensorConfig(uint8_t sensorId, uint8_t type, char* value) {
	Sensor* sensor = __aquaduino->getSensor(sensorId);
	if (type == 2) {
		sensor->setName(value);
	}
	if (type == 3) {
		//sensorUnitÊÊÊÊnot implemented yet
	}
	__aquaduino->writeConfig(sensor);
}
void GUIServer::setSensorConfig(uint8_t sensorId, uint8_t type, uint8_t value) {
	Sensor* sensor = __aquaduino->getSensor(sensorId);
	if (type == 1) {
		//sensor->resetOperatinHours();
	}
	if (type == 4) {
			//sensor->setVisible(visible)
		}
	if (type == 5) {
		//sensor->setCalibratioInterval(value)
	}
	//__aquaduino->writeConfig(sensor);
}
/*
 void GUIServer::setSensorConfig
 void GUIServer::setActuatorData
 */

