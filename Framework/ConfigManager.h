/*
 * ConfigManager.h
 *
 *  Created on: Mar 1, 2013
 *      Author: timok
 */

#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#include <Arduino.h>
#include <Aquaduino.h>
#include <Framework/Actuator.h>
#include <Framework/Controller.h>
#include <Framework/Sensor.h>

static const uint16_t bufferSize = 100;

class Aquaduino;

struct configuration{
	char name[AQUADUINO_STRING_LENGTH];
	uint8_t objectType;
	int8_t controllerIdx;
	int8_t actuatorIdx;
	int8_t sensorIdx;
	char data[bufferSize];
};

class ConfigManager {
public:

	virtual int8_t writeConfig(Aquaduino* aquaduino) = 0;
	virtual int8_t writeConfig(Actuator* actuator) = 0;
	virtual int8_t writeConfig(Controller* controller) = 0;
	virtual int8_t writeConfig(Sensor* sensor) = 0;

	virtual int8_t readConfig(Aquaduino* aquaduino) = 0;
	virtual int8_t readConfig(Actuator* actuator) = 0;
	virtual int8_t readConfig(Controller* controller) = 0;
	virtual int8_t readConfig(Sensor* sensor) = 0;
};

#endif /* CONFIGMANAGER_H_ */
