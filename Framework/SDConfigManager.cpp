/*
 * SDConfigManager.cpp
 *
 *  Created on: 04.03.2013
 *      Author: TimoK
 */

#include "SDConfigManager.h"
#include "SD.h"
#include <Aquaduino.h>

SDConfigManager::SDConfigManager()
{
	m_prefix[0] = 0;
}

SDConfigManager::SDConfigManager(const char* prefix)
{
	File f;

	strncpy(m_prefix, prefix, PREFIX_LENGTH-1);
	m_prefix[PREFIX_LENGTH-1] = 0;

	if(!SD.exists(m_prefix))
		SD.mkdir(m_prefix);
	else
	{
		f = SD.open(m_prefix, FILE_READ);
		if (!f.isDirectory())
			m_prefix[0] = 0;
	}
}

SDConfigManager::~SDConfigManager()
{
}

int8_t SDConfigManager::writeConfig(Aquaduino* aquaduino)
{
	return 0;
}

int8_t SDConfigManager::writeConfig(Actuator* actuator)
{
	return 0;
}

int8_t SDConfigManager::writeConfig(Controller* controller)
{
	struct configuration config;
	char fileName[FILENAME_LENGTH];
	uint16_t serializedBytes = 0;
	uint16_t writtenBytes = 0;
	int8_t id;

	memset(&config, 0, sizeof(config));

	id = aquaduino->getControllerID(controller);
	fileName[0] = 'C';
	itoa(id, &fileName[1], 10);
	strcat(fileName, ".cfg");

	serializedBytes = controller->serialize(&config.data, bufferSize);

	if (serializedBytes > 0)
	{
		config.controllerIdx = id;
		config.objectType = controller->getType();
		config.actuatorIdx = -1;
		config.sensorIdx = -1;
		strcpy(config.name, controller->getName());

		writtenBytes = writeStructToFile(fileName, &config);

		if (writtenBytes != sizeof(struct configuration))
			return -1;
	}
	return 0;
}

int8_t SDConfigManager::writeConfig(Sensor* sensor)
{
	return 0;
}

int8_t SDConfigManager::readConfig(Aquaduino* aquaduino)
{
	return 0;
}

int8_t SDConfigManager::readConfig(Actuator* actuator)
{
	return 0;
}

int8_t SDConfigManager::readConfig(Controller* controller)
{
	struct configuration config;
	char fileName[FILENAME_LENGTH];
	uint16_t serializedBytes = 0;
	uint16_t readBytes = 0;
	int8_t id;

	memset(&config, 0, sizeof(config));

	id = aquaduino->getControllerID(controller);
	fileName[0] = 'C';
	itoa(id, &fileName[1], 10);
	strcat(fileName, ".cfg");

	readBytes = readStructFromFile(fileName, &config);

	if (readBytes == sizeof(struct configuration))
	{
#ifdef DEBUG
		Serial.print(F("Name: "));
		Serial.println(config.name);
		Serial.print(F("ID: "));
		Serial.println(config.controllerIdx);
		Serial.print(F("Type: "));
		Serial.println(config.objectType);
#endif
		if (controller->getType() == config.objectType)
			controller->deserialize(&config.data, bufferSize);
		else
		{
#ifdef DEBUG
			Serial.println(F("Object Types do not match!"));
			Serial.print(controller->getType());
			Serial.print(" <> ");
			Serial.print(config.objectType);
#endif
		}
	}

	return 0;
}

int8_t SDConfigManager::readConfig(Sensor* sensor)
{
	return 0;
}

uint16_t SDConfigManager::writeStructToFile(const char* fileName, struct configuration* config)
{
	uint16_t writtenBytes = 0;
	File configFile;
	char path[PREFIX_LENGTH+FILENAME_LENGTH];
	memset(path,0,PREFIX_LENGTH+FILENAME_LENGTH);

	strcat(path, m_prefix);
	strcat(path, "/");
	strcat(path, fileName);

#ifdef DEBUG
	Serial.print(F("Writing Config to "));
	Serial.println(fileName);
#endif

	configFile = SD.open(path, FILE_WRITE);
	configFile.seek(SEEK_SET);
	writtenBytes = configFile.write((uint8_t*) config, sizeof(struct configuration));
	configFile.close();

	if (writtenBytes == sizeof(struct configuration))
	{
#ifdef DEBUG
		Serial.println(F("Successful!"));
#endif
	} else
	{
#ifdef DEBUG
		Serial.print(F("Failed! Wrote only "));
		Serial.print(writtenBytes);
		Serial.print(F(" Bytes"));
#endif
	}


	return writtenBytes;
}

uint16_t SDConfigManager::readStructFromFile(const char* fileName, struct configuration* config)
{
	uint16_t readBytes = 0;
	File configFile;
	char path[PREFIX_LENGTH+FILENAME_LENGTH];

	memset(path,0,PREFIX_LENGTH+FILENAME_LENGTH);

	strcat(path, m_prefix);
	strcat(path, "/");
	strcat(path, fileName);

#ifdef DEBUG
	Serial.print(F("Reading Config from "));
	Serial.println(path);
#endif

	if (SD.exists(path))
	{
		configFile = SD.open(path, FILE_READ);
		readBytes = configFile.read(config, sizeof(struct configuration));
		configFile.close();

		if (readBytes == sizeof(struct configuration))
		{
#ifdef DEBUG
			Serial.println(F("Successful!"));
#endif
		}
		else
		{
#ifdef DEBUG
			Serial.print(F("Failed! Read only "));
			Serial.print(readBytes);
			Serial.println(F(" Bytes"));
#endif
		}
	}
	else
	{
#ifdef DEBUG
		Serial.println("Configuration does not exist.");
#endif
	}

	return readBytes;
}
