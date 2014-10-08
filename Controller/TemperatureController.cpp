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
#include <Framework/Aquaduino.h>
#include <SD.h>

#ifdef FEATURE_WEBIF
#include <TemplateParser.h>
#include <Framework/Flashvars.h>
#endif

/**
 * Constructor
 * \param[in] name The name of the controller
 */
TemperatureController::TemperatureController(const char* name) :
        Controller(name)
{
    m_Type = CONTROLLER_TEMPERATURE;
    m_Sensor = -1;
    m_RefTemp1 = 24.0;
    m_Hysteresis1 = 0.3;
    m_Actuator1 = -1;
    m_RefTemp2 = 25.0;
    m_Hysteresis2 = 0.3;
    m_Actuator2 = -1;
    m_Cooling = 0;
    m_Heating = 0;
}

/**
 * \brief Destructor
 *
 * Empty.
 */
TemperatureController::~TemperatureController()
{
}

uint16_t TemperatureController::serialize(Stream* s)
{
    uint16_t mySize = sizeof(m_RefTemp1) + sizeof(m_Hysteresis1)
                      + sizeof(m_RefTemp2) + sizeof(m_Hysteresis2);

    s->write((uint8_t*) &m_Sensor, sizeof(m_Sensor));
    s->write((uint8_t*) &m_RefTemp1, sizeof(m_RefTemp1));
    s->write((uint8_t*) &m_Hysteresis1, sizeof(m_Hysteresis1));
    s->write((uint8_t*) &m_Actuator1, sizeof(m_Actuator1));
    s->write((uint8_t*) &m_RefTemp2, sizeof(m_RefTemp2));
    s->write((uint8_t*) &m_Hysteresis2, sizeof(m_Hysteresis2));
    s->write((uint8_t*) &m_Actuator2, sizeof(m_Actuator2));

    return mySize;
}

uint16_t TemperatureController::deserialize(Stream* s)
{
    uint16_t mySize = sizeof(m_RefTemp1) + sizeof(m_Hysteresis1)
                      + sizeof(m_RefTemp2) + sizeof(m_Hysteresis2);
    if (mySize != s->available())
    {
    	s->readBytes((char*) &m_Sensor, sizeof(m_Sensor));
        if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
            m_Sensor = -1;
    	s->readBytes((char*) &m_RefTemp1, sizeof(m_RefTemp1));
    	s->readBytes((char*) &m_Hysteresis1, sizeof(m_Hysteresis1));
    	s->readBytes((char*) &m_Actuator1, sizeof(m_Actuator1));
    	s->readBytes((char*) &m_RefTemp2, sizeof(m_RefTemp2));
    	s->readBytes((char*) &m_Hysteresis2, sizeof(m_Hysteresis2));
    	s->readBytes((char*) &m_Actuator2, sizeof(m_Actuator2));
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
    Actuator *actuator1, *actuator2;

    if (m_Sensor == -1 || (m_Actuator1 == -1 && m_Actuator2 == -1))
        return -1;

    temp = __aquaduino->getSensorValue(m_Sensor);
    actuator1 = __aquaduino->getActuator(m_Actuator1);
    actuator2 = __aquaduino->getActuator(m_Actuator2);

    if (actuator1)
    {
        if (temp < m_RefTemp1)
        {
            actuator1->on();
            m_Heating = 1;
        }
        else if (m_Heating && (temp > m_RefTemp1 + m_Hysteresis1))
        {
            actuator1->off();
            m_Heating = 0;
        }
    }

    if (actuator2)
    {
        if (temp > m_RefTemp2)
        {
            actuator2->on();
            m_Cooling = 1;
        }
        else if (actuator2 && m_Cooling && temp < m_RefTemp2 - m_Hysteresis1)
        {
            actuator2->off();
            m_Cooling = 0;
        }
    }

    return true;
}

#ifdef FEATURE_WEBIF
int8_t TemperatureController::showWebinterface(WebServer* server,
                                               WebServer::ConnectionType type,
                                               char* url)
{
    File templateFile;
    TemplateParser* parser;
    int8_t matchIdx;

    Sensor* sensor;
    const char* sensorNames[MAX_SENSORS + 1];
    char sensorValArray[MAX_SENSORS + 1][3];
    const char* sensorValuePointers[MAX_SENSORS + 1];
    int8_t i = 0, sensorIdx;

    Actuator* actuator;
    const char* actuatorNames[MAX_ACTUATORS + 1];
    char actuatorValArray[MAX_ACTUATORS + 1][3];
    const char* actuatorValuePointers[MAX_ACTUATORS + 1];
    int8_t j = 0, actuatorIdx;
    int8_t select1 = 0, select2 = 0;

    char input_name[20];

    char templateFileName[template_temperaturecontroller_fnsize];

    strcpy_P(templateFileName, template_temperaturecontroller_fname);

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_SENSOR])))
                == 0)
                m_Sensor = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_REFTEMP1])))
                     == 0)
                m_RefTemp1 = atof(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_HYST1])))
                     == 0)
                m_Hysteresis1 = atof(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_ACTUATOR1])))
                     == 0)
                m_Actuator1 = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_REFTEMP2])))
                     == 0)
                m_RefTemp2 = atof(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_HYST2])))
                     == 0)
                m_Hysteresis2 = atof(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_temperaturecontroller_inputs[TEMPERATURECONTROLLER_I_ACTUATOR2])))
                     == 0)
                m_Actuator2 = atoi(value);
        } while (repeat);
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = __aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        __aquaduino->resetSensorIterator();
        __aquaduino->resetActuatorIterator();
        sensorNames[0] = "None";
        actuatorNames[0] = sensorNames[0];

        sensorValuePointers[0] = "-1";
        actuatorValuePointers[0] = sensorValuePointers[0];
        i = 1;
        j = 1;

        while ((sensorIdx = __aquaduino->getNextSensor(&sensor)) != -1)
        {
            sensorNames[i] = sensor->getName();
            itoa(sensorIdx, sensorValArray[i], 10);
            sensorValuePointers[i] = sensorValArray[i];
            i++;
        }

        while ((actuatorIdx = __aquaduino->getNextActuator(&actuator)) != -1)
        {
            if (__aquaduino->getController(actuator->getController()) == this)
            {
                if (m_Actuator1 == actuatorIdx)
                    select1 = j;
                if (m_Actuator2 == actuatorIdx)
                    select2 = j;
                actuatorNames[j] = actuator->getName();
                itoa(actuatorIdx, actuatorValArray[j], 10);
                actuatorValuePointers[j] = actuatorValArray[j];
                j++;
            }
        }

        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      template_temperaturecontroller,
                                                      template_temperaturecontroller_elements,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case TEMPERATURECONTROLLER_CNAME:
                server->print(this->getName());
                break;
            case TEMPERATURECONTROLLER_SSELECT:
                strcpy_P(input_name, pgm_input_sensor);
                parser->selectList(input_name,
                                   sensorNames,
                                   sensorValuePointers,
                                   this->m_Sensor + 1,
                                   i,
                                   server);
                break;
            case TEMPERATURECONTROLLER_TEMPERATURE:
                server->print(__aquaduino->getSensorValue(m_Sensor));
                break;
            case TEMPERATURECONTROLLER_REFTEMP1_NAME:
                server->print((__FlashStringHelper *) pgm_input_reftemp1);
                break;
            case TEMPERATURECONTROLLER_REFTEMP1_VAL:
                server->print(m_RefTemp1);
                break;
            case TEMPERATURECONTROLLER_REFTEMP1_SIZE:
                server->print(6);
                break;
            case TEMPERATURECONTROLLER_REFTEMP1_MAXLENGTH:
                server->print(5);
                break;
            case TEMPERATURECONTROLLER_HYST1_NAME:
                server->print((__FlashStringHelper *) pgm_input_hyst1);
                break;
            case TEMPERATURECONTROLLER_HYST1_VAL:
                server->print(m_Hysteresis1);
                break;
            case TEMPERATURECONTROLLER_HYST1_SIZE:
                server->print(6);
                break;
            case TEMPERATURECONTROLLER_HYST1_MAXLENGTH:
                server->print(5);
                break;
            case TEMPERATURECONTROLLER_ASELECT1:
                strcpy_P(input_name, pgm_input_actuator1);
                parser->selectList(input_name,
                                   actuatorNames,
                                   actuatorValuePointers,
                                   select1,
                                   j,
                                   server);
                break;
            case TEMPERATURECONTROLLER_REFTEMP2_NAME:
                server->print((__FlashStringHelper *) pgm_input_reftemp2);
                break;
            case TEMPERATURECONTROLLER_REFTEMP2_VAL:
                server->print(m_RefTemp2);
                break;
            case TEMPERATURECONTROLLER_REFTEMP2_SIZE:
                server->print(6);
                break;
            case TEMPERATURECONTROLLER_REFTEMP2_MAXLENGTH:
                server->print(5);
                break;
            case TEMPERATURECONTROLLER_HYST2_NAME:
                server->print((__FlashStringHelper *) pgm_input_hyst2);
                break;
            case TEMPERATURECONTROLLER_HYST2_VAL:
                server->print(m_Hysteresis2);
                break;
            case TEMPERATURECONTROLLER_HYST2_SIZE:
                server->print(6);
                break;
            case TEMPERATURECONTROLLER_HYST2_MAXLENGTH:
                server->print(5);
                break;
            case TEMPERATURECONTROLLER_ASELECT2:
                strcpy_P(input_name, pgm_input_actuator2);
                parser->selectList(input_name,
                                   actuatorNames,
                                   actuatorValuePointers,
                                   select2,
                                   j,
                                   server);
                break;
            }
        }
        templateFile.close();
    }
    return true;
}
#endif
