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

const static double HYSTERESIS = 0.3;

const static char progTemplateString1[] PROGMEM = "##TEMPERATURE##";
const static char progTemplateString2[] PROGMEM = "##THRESHOLD##";
const static char progTemplateString3[] PROGMEM = "##PWMMAX##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3 };

const static char progInputThreshold[] PROGMEM = "Threshold";
const static char progInputPWMMax[] PROGMEM = "PWMMax";

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
    maxPWM = 28.0;
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

    uint16_t mySize = sizeof(m_Threshold) + sizeof(maxPWM);
    if (mySize <= size)
    {
        memcpy(bPtr, &m_Threshold, sizeof(m_Threshold));
        offset += sizeof(m_Threshold);
        memcpy(bPtr + offset, &maxPWM, sizeof(maxPWM));
        return mySize;
    }
    return 0;
}

uint16_t TemperatureController::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_Threshold) + sizeof(maxPWM);
    if (mySize <= size)
    {
        memcpy(&m_Threshold, bPtr, sizeof(m_Threshold));
        offset += sizeof(m_Threshold);
        memcpy(&maxPWM, bPtr + offset, sizeof(maxPWM));
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

    temp = aquaduino->getTemperature();
    if (temp >= m_Threshold)
        allMyActuators(1);
    else if (m_Threshold - temp > HYSTERESIS)
        allMyActuators(0);
    return true;
}

int8_t TemperatureController::showWebinterface(WebServer* server,
                                               WebServer::ConnectionType type,
                                               char* url)
{
    File templateFile;
    TemplateParser* parser;

    char templateFileName[sizeof(progTemplateFileName)];
    char* replacementStrings[3];

    char temperature[10];
    char threshold[4];
    char pwmmax[4];

    strcpy_P(templateFileName, progTemplateFileName);

    dtostrf(aquaduino->getTemperature(), 5, 2, temperature);
    itoa(m_Threshold, threshold, 10);
    itoa(maxPWM, pwmmax, 10);

    replacementStrings[0] = temperature;
    replacementStrings[1] = threshold;
    replacementStrings[2] = pwmmax;

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name, progInputThreshold) == 0)
            {
                uint8_t d = atoi(value);
                m_Threshold = d;
            }
            else if (strcmp_P(name, progInputPWMMax) == 0)
            {
                uint8_t d = atoi(value);
                maxPWM = d;
            }
        } while (repeat);

        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        parser->processSingleTemplate(&templateFile,
                                      templateStrings,
                                      replacementStrings,
                                      3,
                                      server);
        templateFile.close();
    }
    return true;
}
