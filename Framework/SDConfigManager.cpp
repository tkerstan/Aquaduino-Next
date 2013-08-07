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

int8_t SDConfigManager::writeConfig(Aquaduino* aquaduino)
{
    struct configuration config;
    uint16_t serializedBytes = 0;
    uint16_t writtenBytes = 0;

    memset(&config, 0, sizeof(config));

    serializedBytes = aquaduino->serialize(config.data, bufferSize);

    if (serializedBytes)
    {
        config.objectType = AQUADUINO;
        config.controllerIdx = -1;
        config.actuatorIdx = -1;
        config.sensorIdx = -1;

        writtenBytes = writeStructToFile("aqua.cfg", &config);

        if (writtenBytes != sizeof(struct configuration))
            return -1;
    }

    return 0;
}

int8_t SDConfigManager::writeConfig(Actuator* actuator)
{
    struct configuration config;
    char fileName[FILENAME_LENGTH];
    uint16_t serializedBytes = 0;
    uint16_t writtenBytes = 0;
    int8_t id;

    memset(&config, 0, sizeof(config));

    id = __aquaduino->getActuatorID(actuator);
    fileName[0] = 'A';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    serializedBytes = actuator->serialize(config.data, bufferSize);

    if (serializedBytes)
    {
        config.controllerIdx = actuator->getController();
        config.objectType = actuator->getType();
        config.actuatorIdx = id;
        config.sensorIdx = -1;
        strcpy(config.name, actuator->getName());

        writtenBytes = writeStructToFile(fileName, &config);

        if (writtenBytes != sizeof(struct configuration))
            return -1;
    }
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

    id = __aquaduino->getControllerID(controller);
    fileName[0] = 'C';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    serializedBytes = controller->serialize(config.data, bufferSize);

    if (serializedBytes)
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
    struct configuration config;
    char fileName[FILENAME_LENGTH];
    uint16_t serializedBytes = 0;
    uint16_t writtenBytes = 0;
    int8_t id;

    memset(&config, 0, sizeof(config));

    id = __aquaduino->getSensorID(sensor);
    fileName[0] = 'S';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    serializedBytes = sensor->serialize(config.data, bufferSize);

    if (serializedBytes)
    {
        config.controllerIdx = -1;
        config.objectType = sensor->getType();
        config.actuatorIdx = -1;
        config.sensorIdx = -1;
        strcpy(config.name, sensor->getName());

        writtenBytes = writeStructToFile(fileName, &config);

        if (writtenBytes != sizeof(struct configuration))
            return -1;
    }
    return 0;
}

int8_t SDConfigManager::readConfig(Aquaduino* aquaduino)
{
    struct configuration config;
    uint16_t readBytes = 0;

    memset(&config, 0, sizeof(config));

    readBytes = readStructFromFile("aqua.cfg", &config);

    if (readBytes == sizeof(struct configuration))
    {
        if (aquaduino->getType() == config.objectType)
            aquaduino->deserialize(config.data, bufferSize);
    }

    return 0;
}

int8_t SDConfigManager::readConfig(Actuator* actuator)
{
    struct configuration config;
    char fileName[FILENAME_LENGTH];
    uint16_t readBytes = 0;
    int8_t id;

    memset(&config, 0, sizeof(config));

    id = __aquaduino->getActuatorID(actuator);
    fileName[0] = 'A';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    readBytes = readStructFromFile(fileName, &config);

    if (readBytes == sizeof(struct configuration))
    {
        if (actuator->getType() == config.objectType)
            actuator->deserialize(config.data, bufferSize);
        actuator->setName(config.name);
        actuator->setController(config.controllerIdx);
    }

    return 0;
}

int8_t SDConfigManager::readConfig(Controller* controller)
{
    struct configuration config;
    char fileName[FILENAME_LENGTH];
    uint16_t readBytes = 0;
    int8_t id;

    memset(&config, 0, sizeof(config));

    id = __aquaduino->getControllerID(controller);
    fileName[0] = 'C';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    readBytes = readStructFromFile(fileName, &config);

    if (readBytes == sizeof(struct configuration))
    {
        if (controller->getType() == config.objectType)
            controller->deserialize(config.data, bufferSize);
        controller->setName(config.name);
    }

    return 0;
}

int8_t SDConfigManager::readConfig(Sensor* sensor)
{
    struct configuration config;
    char fileName[FILENAME_LENGTH];
    uint16_t readBytes = 0;
    int8_t id;

    memset(&config, 0, sizeof(config));

    id = __aquaduino->getSensorID(sensor);
    fileName[0] = 'S';
    itoa(id, &fileName[1], 10);
    strcat(fileName, ".cfg");

    readBytes = readStructFromFile(fileName, &config);

    if (readBytes == sizeof(struct configuration))
    {
        if (sensor->getType() == config.objectType)
            sensor->deserialize(config.data, bufferSize);
        sensor->setName(config.name);
    }

    return 0;
}

uint16_t SDConfigManager::writeStructToFile(const char* fileName,
                                            struct configuration* config)
{
    uint16_t writtenBytes = 0;
    File configFile;
    char path[PREFIX_LENGTH + FILENAME_LENGTH];
    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    Serial.print(F("Writing configuration to "));
    Serial.print(path);

    configFile = SD.open(path, FILE_WRITE);
    configFile.seek(SEEK_SET);
    writtenBytes = configFile.write((uint8_t*) config,
                                    sizeof(struct configuration));
    configFile.close();

    if (writtenBytes == sizeof(struct configuration))
        Serial.println(F(" : successful"));
    else
        Serial.println(F(" : failed"));

    return writtenBytes;
}

uint16_t SDConfigManager::readStructFromFile(const char* fileName,
                                             struct configuration* config)
{
    uint16_t readBytes = 0;
    File configFile;
    char path[PREFIX_LENGTH + FILENAME_LENGTH];

    memset(path, 0, PREFIX_LENGTH + FILENAME_LENGTH);

    strcat(path, m_folder);
    strcat(path, "/");
    strcat(path, fileName);

    if (SD.exists(path))
    {
        Serial.print(F("Reading configuration from "));
        Serial.print(path);

        configFile = SD.open(path, FILE_READ);
        readBytes = configFile.read(config, sizeof(struct configuration));
        configFile.close();

        if (readBytes == sizeof(struct configuration))
        {
            Serial.println(" : successful");
        }
        else
            Serial.println(" : failed");
    }
    else
    {
        Serial.print(path);
        Serial.println(F(" does not exist"));
    }

    return readBytes;
}
