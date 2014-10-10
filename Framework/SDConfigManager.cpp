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

#include "SDConfigManager.h"
#include "SD.h"
#include <Aquaduino.h>
#include <Framework/ObjectTypes.h>

extern int freeRam();

/**
 * Default constructor.
 *
 * Sets empty folder for configuration files.
 */
SDConfigManager::SDConfigManager()
{
    m_folder[0] = 0;
}

/**
 * Constructor
 * \param[in] folder folder of configuration files
 *
 * Checks if the folder already exists. If not the folder is created.
 */
SDConfigManager::SDConfigManager(const char* folder)
{
    File f;

    strncpy(m_folder, folder, PREFIX_LENGTH - 1);
    m_folder[PREFIX_LENGTH - 1] = 0;

    if (!SD.exists(m_folder))
        SD.mkdir(m_folder);
    else
    {
        f = SD.open(m_folder, FILE_READ);
        if (!f.isDirectory())
            m_folder[0] = 0;
    }
}

/**
 * \brief Destructor
 *
 * Empty.
 */
SDConfigManager::~SDConfigManager()
{
}

/**
 * \brief Copy Constructor
 *
 * Empty.
 */
SDConfigManager::SDConfigManager(SDConfigManager&)
{
}

/**
 * \brief Copy Constructor
 *
 * Empty.
 */
SDConfigManager::SDConfigManager(const SDConfigManager&)
{
}

uint16_t SDConfigManager::writeConfig(Aquaduino* aquaduino)
{
	return 0;
}

uint16_t SDConfigManager::writeConfig(Actuator* actuator)
{
	File configFile;
    char fileName[FILENAME_LENGTH];
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    int8_t id;
    int retval = 0;

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    id = __aquaduino->getActuatorID(actuator);
    fileName[0] = 'A';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Writing actuator config to "));
    Serial.print(path);
    Serial.println(F(" ..."));

	Serial.print("Serializing...");
	if (SD.exists(path))
		SD.remove(path);
	configFile = SD.open(path, FILE_WRITE);
	configFile.write((uint8_t*) actuator->getName(), AQUADUINO_STRING_LENGTH);
	configFile.write(actuator->getController());
	if (actuator->serialize(&configFile))
		Serial.println(F(" OK!"));
	else
		Serial.println(F(" Failed!"));
	configFile.close();

    return retval;
}

uint16_t SDConfigManager::writeConfig(Controller* controller)
{
	File configFile;
    char fileName[FILENAME_LENGTH];
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    int8_t id;
    int retval = 0;

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    id = __aquaduino->getControllerID(controller);
    fileName[0] = 'C';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Writing controller config to "));
    Serial.print(path);
    Serial.println(F(" ..."));

	Serial.print("Serializing...");
	if (SD.exists(path))
		SD.remove(path);
	configFile = SD.open(path, FILE_WRITE);
	configFile.write((uint8_t*) controller->getName(), AQUADUINO_STRING_LENGTH);
	if (controller->serialize(&configFile))
		Serial.println(F(" OK!"));
	else
		Serial.println(F(" Failed!"));
	configFile.close();

    return retval;
}

uint16_t SDConfigManager::writeConfig(Sensor* sensor)
{
	File configFile;
    char fileName[FILENAME_LENGTH];
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    int8_t id;
    int retval = 0;

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);
    id = __aquaduino->getSensorID(sensor);
    fileName[0] = 'S';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Writing sensor config to "));
    Serial.print(path);
    Serial.println(F(" ..."));

	Serial.print("Serializing...");
	if (SD.exists(path))
		SD.remove(path);
	configFile = SD.open(path, FILE_WRITE);
	configFile.write((uint8_t*) sensor->getName(), AQUADUINO_STRING_LENGTH);
	if (sensor->serialize(&configFile))
		Serial.println(F(" OK!"));
	else
		Serial.println(F(" Failed!"));
	configFile.close();

    return retval;
}

uint16_t SDConfigManager::readConfig(Aquaduino* aquaduino)
{
	File configFile;
	char path[PREFIX_LENGTH + FILENAME_LENGTH];
	int retval = 0;

	memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

	strcat(path, m_folder);
	strcat(path, "/");
	strcat(path, "aqua.cfg");

	if (SD.exists(path))
	{
		configFile = SD.open(path, FILE_READ);
		aquaduino->deserialize(&configFile);
		configFile.close();
	}
	else
	{
		retval = 1;
	}

    return retval;
}

uint16_t SDConfigManager::readConfig(Actuator* actuator)
{
	File configFile;
    char fileName[FILENAME_LENGTH];
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    char name[AQUADUINO_STRING_LENGTH];
    int8_t id;
    int retval = 0;

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    id = __aquaduino->getActuatorID(actuator);
    fileName[0] = 'A';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Reading actuator config from "));
    Serial.print(path);
    Serial.println(F(" ..."));

    if (SD.exists(path))
	{
    	Serial.print(" File exists! Deserializing...");
		configFile = SD.open(path, FILE_READ);
		configFile.read(name, AQUADUINO_STRING_LENGTH);
		actuator->setName(name);
		actuator->setController(configFile.read());
		if (actuator->deserialize(&configFile))
			Serial.println(F(" OK!"));
		else
			Serial.println(F(" Failed!"));
		configFile.close();
	}
	else
	{
		Serial.println(" File does not exist!");
		retval = 1;
	}

    return retval;
}

uint16_t SDConfigManager::readConfig(Controller* controller)
{
	File configFile;
    char fileName[FILENAME_LENGTH];
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    char name[AQUADUINO_STRING_LENGTH];
    int8_t id;
    int retval = 0;

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    id = __aquaduino->getControllerID(controller);
    fileName[0] = 'C';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Reading controller config from "));
    Serial.print(path);
    Serial.println(F(" ..."));

    if (SD.exists(path))
	{
    	Serial.print(" File exists! Deserializing...");
		configFile = SD.open(path, FILE_READ);
		configFile.read(name, AQUADUINO_STRING_LENGTH);
		controller->setName(name);

		if (controller->deserialize(&configFile))
			Serial.println(F(" OK!"));
		else
			Serial.println(F(" Failed!"));
		configFile.close();
	}
	else
	{
		Serial.println(" File does not exist!");
		retval = 1;
	}

    return retval;
}

uint16_t SDConfigManager::readConfig(Sensor* sensor)
{
	File configFile;
    char fileName[FILENAME_LENGTH];
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    char name[AQUADUINO_STRING_LENGTH];
    int8_t id;
    int retval = 0;

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    id = __aquaduino->getSensorID(sensor);
    fileName[0] = 'S';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Reading sensor config from "));
    Serial.print(path);
    Serial.println(F(" ..."));

    if (SD.exists(path))
	{
    	Serial.print(" File exists! Deserializing...");
		configFile = SD.open(path, FILE_READ);
		configFile.read(name, AQUADUINO_STRING_LENGTH);
		sensor->setName(name);
		if (sensor->deserialize(&configFile))
			Serial.println(F(" OK!"));
		else
			Serial.println(F(" Failed!"));
		configFile.close();
	}
	else
	{
		Serial.println(" File does not exist!");
		retval = 1;
	}

    return retval;
}
