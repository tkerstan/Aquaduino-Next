/*
 * Copyright (c) 2012 Timo Kerstan.  All right reserved.
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

#include <Controller/TemperatureController.h>
#include <Aquaduino.h>
#include <SD.h>
#include <TemplateParser.h>

const static char progTemplateString1[] PROGMEM = "##SSELECT##";
const static char progTemplateString2[] PROGMEM = "##TEMPERATURE##";
const static char progTemplateString3[] PROGMEM = "##THRESHOLD##";
const static char progTemplateString4[] PROGMEM = "##PWMMAX##";
const static char progTemplateString5[] PROGMEM = "##HYSTERESIS##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3,
      progTemplateString4, progTemplateString5 };

enum
{
    TC_SSELECT, TC_TEMPERATURE, TC_THRESHOLD, TC_PWMMAX, TC_HYSTERESIS
};

const static char progInputThreshold[] PROGMEM = "Threshold";
const static char progInputPWMMax[] PROGMEM = "PWMMax";
const static char progInputHysteresis[] PROGMEM = "Hysteresis";
const static char progInputSensor[] PROGMEM = "sensor";

const static char progTemplateFileName[] PROGMEM = "temp.htm";

/**
 * Constructor
 * \param[in] name The name of the controller
 */
TemperatureController::TemperatureController(const char* name) :
        Controller(name)
{
    m_Type = CONTROLLER_TEMPERATURE;
    m_Threshold = 25.0;
    m_MaxPWM = 28.0;
    m_Hysteresis = 0.3;
}

/**
 * \brief Destructor
 *
 * Empty.
 */
TemperatureController::~TemperatureController()
{
}

uint16_t TemperatureController::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_Threshold) + sizeof(m_MaxPWM);
    if (mySize <= size)
    {
        memcpy(bPtr, &m_Threshold, sizeof(m_Threshold));
        offset += sizeof(m_Threshold);
        memcpy(bPtr + offset, &m_MaxPWM, sizeof(m_MaxPWM));
        offset += sizeof(m_MaxPWM);
        memcpy(bPtr + offset, &m_Sensor, sizeof(m_Sensor));
        return mySize;
    }
    return 0;
}

uint16_t TemperatureController::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_Threshold) + sizeof(m_MaxPWM);
    if (mySize <= size)
    {
        memcpy(&m_Threshold, bPtr, sizeof(m_Threshold));
        offset += sizeof(m_Threshold);
        memcpy(&m_MaxPWM, bPtr + offset, sizeof(m_MaxPWM));
        offset += sizeof(m_MaxPWM);
        memcpy(&m_Sensor, bPtr + offset, sizeof(m_Sensor));
        if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
            m_Sensor = -1;

        return mySize;
    }
    return 0;
}

/**
 * \brief Run method triggered by Aquaduino::run
 *
 * Turns on all assigned actuators when temperature exceeds m_Threshold.
 * When the temperature drops below m_Threshold - HYSTERESIS all assigned
 * actuators are turned off.
 */
int8_t TemperatureController::run()
{
    float temp;
    if (m_Sensor == -1)
        return -1;

    temp = aquaduino->getSensorValue(m_Sensor);
    if (temp >= m_Threshold)
        allMyActuators(1);
    else if (m_Threshold - temp > m_Hysteresis)
        allMyActuators(0);
    return true;
}

int8_t TemperatureController::showWebinterface(WebServer* server,
                                               WebServer::ConnectionType type,
                                               char* url)
{
    File templateFile;
    TemplateParser* parser;
    int8_t matchIdx;
    const char* sensorNames[MAX_SENSORS + 1];
    char sensorValArray[MAX_SENSORS + 1][3];
    const char* sensorValuePointers[MAX_SENSORS + 1];
    int8_t i = 0, sensorIdx;
    Sensor* sensor;

    char templateFileName[sizeof(progTemplateFileName)];

    strcpy_P(templateFileName, progTemplateFileName);

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name, progInputThreshold) == 0)
                m_Threshold = atof(value);
            else if (strcmp_P(name, progInputPWMMax) == 0)
                m_MaxPWM = atof(value);
            else if (strcmp_P(name, progInputHysteresis) == 0)
                m_Hysteresis = atof(value);
            else if (strcmp_P(name, progInputSensor) == 0)
                m_Sensor = atoi(value);

        } while (repeat);

        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        aquaduino->resetSensorIterator();
        sensorNames[0] = "None";
        sensorValuePointers[0] = "-1";
        i = 1;
        while ((sensorIdx = aquaduino->getNextSensor(&sensor)) != -1)
        {
            sensorNames[i] = sensor->getName();
            itoa(sensorIdx, sensorValArray[i], 10);
            sensorValuePointers[i] = sensorValArray[i];
            i++;
        }
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      templateStrings,
                                                      sizeof(templateStrings) / sizeof(char*),
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case TC_SSELECT:
                parser->selectList("sensor",
                                   sensorNames,
                                   sensorValuePointers,
                                   this->m_Sensor+1,
                                   i,
                                   server);
                break;
            case TC_TEMPERATURE:
                server->print(aquaduino->getSensorValue(m_Sensor));
                break;
            case TC_THRESHOLD:
                server->print(m_Threshold);
                break;
            case TC_PWMMAX:
                server->print(m_MaxPWM);
                break;
            case TC_HYSTERESIS:
                server->print(m_Hysteresis);
                break;
            }
        }
        templateFile.close();
    }
    return true;
}
