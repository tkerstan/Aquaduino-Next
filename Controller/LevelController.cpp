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

#include <Controller/LevelController.h>
#include <Framework/Aquaduino.h>

#include <SD.h>

#include <limits.h>

#ifdef FEATURE_WEBIF
#include <TemplateParser.h>
#include <Framework/Flashvars.h>
#endif

enum TEMPLATE_LEVELCONTROLLER_STATES
{
    LEVELCONTROLLER_STATE_OK,
    LEVELCONTROLLER_STATE_DEBOUNCE,
    LEVELCONTROLLER_STATE_REFILL,
    LEVELCONTROLLER_STATE_OVERRUN,
    LEVELCONTROLLER_STATE_REFILL_TIMEOUT
};

/**
 * \brief Constructor
 * \param[in] name Name of the controller
 */
LevelController::LevelController(const char* name) :
        Controller(name)
{
    m_Type = CONTROLLER_LEVEL;
    m_Delayh = 3;
    m_Delayl = 10;
    m_State = LEVELCONTROLLER_STATE_OK;
    m_Timeout = 30;
    m_Sensor = -1;
}

uint16_t LevelController::serialize(Stream* s)
{
    uint16_t mySize = sizeof(m_Delayl) + sizeof(m_Delayh)
                      + sizeof(m_Timeout) + sizeof(m_Sensor);
    s->write((uint8_t*)&m_Sensor, sizeof(m_Sensor));
    s->write((uint8_t*)&m_Delayl, sizeof(m_Delayl));
    s->write((uint8_t*)&m_Delayh, sizeof(m_Delayh));
    s->write((uint8_t*)&m_Timeout, sizeof(m_Timeout));
	return mySize;
}

uint16_t LevelController::deserialize(Stream* s)
{
    uint16_t mySize = sizeof(m_Delayl) + sizeof(m_Delayh)
                      + sizeof(m_Timeout);
    if (mySize > s->available())
    	return 0;

	s->readBytes((char*) &m_Sensor, sizeof(m_Sensor));
	if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
		m_Sensor = -1;
	s->readBytes((char*) &m_Delayl, sizeof(m_Delayl));
	s->readBytes((char*) &m_Delayh, sizeof(m_Delayh));
	s->readBytes((char*) &m_Timeout, sizeof(m_Timeout));
	return mySize;
}

/**
 * \brief Run method triggered by Aquaduino::run
 *
 * This method implements a state machine for the refill process.
 *
 * When in state LEVELCONTROLLER_STATE_OK the state machine waits for
 * the reading to become HIGH and performs a transition to
 * LEVELCONTROLLER_STATE_DEBOUNCE.
 *
 * In state LEVELCONTROLLER_STATE_DEBOUNCE it is checked if the reading becomes
 * LOW within the time LevelController::debounceDelayHigh seconds. When this is
 * the case a transition back to LEVELCONTROLLER_STATE_OK is performed. This is
 * the case when the level sensor detected a wave. When the signal is HIGH for
 * at least LevelController::debounceDelayHigh seconds a transition to
 * LEVELCONTROLLER_STATE_REFILL is performed and all assigned actuators are
 * activated.
 *
 * In state LEVELCONTROLLER_STATE_REFILL it is checked whether the signal
 * remains HIGH for more than LevelController::refillTimeout seconds. If thats
 * the case it is assumed that the water reservoir ran out of water and the
 * refill process is stopped and a transition to
 * LevelController::LEVELCONTROLLER_STATE_REFILL_TIMEOUT is performed.
 * If the signal gets LOW within LevelController::refillTimeout seconds a
 * transition to LEVELCONTROLLER_STATE_OVERRUN is performed.
 *
 * In state LEVELCONTROLLER_STATE_OVERRUN it is checked whether the reading
 * remains LOW and a time of LevelController::hysteresis seconds passes. If
 * this is the case the refill process has finished and a transition to
 * LEVELCONTROLLER_STATE_OK is performed. If the reading gets HIGH again
 * a transition back to LEVELCONTROLLER_STATE_REFILL.
 *
 * In state LEVELCONTROLLER_STATE_REFILL_TIMEOUT nothing happens until
 * the state machine variables are reseted from outside this function. This
 * reset is currently implemented in the POST request processing in
 * LevelController::showWebinterface.
 *
 */
int8_t LevelController::run()
{
    static unsigned long lastTime = 0;
    unsigned long millisNow = 0;
    long delay_low_millis = 0;
    long delay_high_millis = 0;
    long timeout_millis = 0;
    long deltaTSwitch = 0;
    int8_t sensor_val = 0;

    if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
        return -1;

    sensor_val = __aquaduino->getSensorValue(m_Sensor);
    millisNow = millis();
    deltaTSwitch = millisNow - lastTime;

    delay_high_millis = 1000 * (long) m_Delayh;
    delay_low_millis = 1000 * (long) m_Delayl;
    timeout_millis = 1000 * (long) m_Timeout;

    //Check for overflow while processing refill
    if (m_State != LEVELCONTROLLER_STATE_OK && lastTime > millisNow)
        deltaTSwitch = LONG_MAX - lastTime + millisNow;

    switch (m_State)
    {
    case LEVELCONTROLLER_STATE_OK:
        allMyActuators((int8_t) 0);
        if (sensor_val == HIGH)
        {
            m_State = LEVELCONTROLLER_STATE_DEBOUNCE;
            lastTime = millisNow;
        }
        break;
    case LEVELCONTROLLER_STATE_DEBOUNCE:
        if (sensor_val == HIGH && deltaTSwitch > delay_high_millis)
        {
            allMyActuators((int8_t) 1);
            lastTime = millisNow;
            m_State = LEVELCONTROLLER_STATE_REFILL;
        }
        else if (sensor_val == LOW)
        {
            m_State = LEVELCONTROLLER_STATE_OK;
        }
        break;
    case LEVELCONTROLLER_STATE_REFILL:
        if (sensor_val == LOW)
        {
            m_State = LEVELCONTROLLER_STATE_OVERRUN;
            lastTime = millisNow;
        }
        else if (sensor_val == HIGH && deltaTSwitch > timeout_millis)
        {
            m_State = LEVELCONTROLLER_STATE_REFILL_TIMEOUT;
            allMyActuators((int8_t) 0);
        }
        break;
    case LEVELCONTROLLER_STATE_OVERRUN:
        if (sensor_val == LOW && deltaTSwitch > delay_low_millis)
        {
            m_State = LEVELCONTROLLER_STATE_OK;
            allMyActuators((int8_t) 0);
        }
        else if (sensor_val == HIGH)
        {
            m_State = LEVELCONTROLLER_STATE_REFILL;
        }
        break;
    case LEVELCONTROLLER_STATE_REFILL_TIMEOUT:
        return 0;
    default:
        return 0;
    }
    return 1;
}

#ifdef FEATURE_WEBIF
int8_t LevelController::showWebinterface(WebServer* server,
                                         WebServer::ConnectionType type,
                                         char* url)
{
    File templateFile;
    TemplateParser* parser;
    int8_t matchIdx;
    char i_sensor_name[20];
    int16_t val;

    char templateFileName[template_levelcontroller_fnsize];
    strcpy_P(templateFileName, template_levelcontroller_fname);

    const char* sensorNames[MAX_SENSORS + 1];
    char sensorValArray[MAX_SENSORS + 1][3];
    const char* sensorValuePointers[MAX_SENSORS + 1];

    Sensor* sensor;
    int8_t sensorIdx;
    int8_t i = 0;

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];

        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_levelcontroller_inputs[LEVELCONTROLLER_I_DELAYL])))
                == 0)
            {
                val = atoi(value);
                m_Delayl = val;
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_levelcontroller_inputs[LEVELCONTROLLER_I_DELAYH])))
                     == 0)
            {
                val = atol(value);
                m_Delayh = val;
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_levelcontroller_inputs[LEVELCONTROLLER_I_TIMEOUT])))
                     == 0)
            {
                val = atol(value);
                m_Timeout = val;
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_levelcontroller_inputs[LEVELCONTROLLER_I_SENSOR])))
                     == 0)
            {
                m_Sensor = atoi(value);
            }
        } while (repeat);
        m_State = LEVELCONTROLLER_STATE_OK;
        server->httpSeeOther(this->m_URL);
    }
    else
    {

        server->httpSuccess();
        parser = __aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        __aquaduino->resetSensorIterator();
        sensorNames[0] = "None";
        sensorValuePointers[0] = "-1";
        i = 1;
        while ((sensorIdx = __aquaduino->getNextSensor(&sensor)) != -1)
        {
            sensorNames[i] = sensor->getName();
            itoa(sensorIdx, sensorValArray[i], 10);
            sensorValuePointers[i] = sensorValArray[i];
            i++;
        }
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      template_levelcontroller,
                                                      template_levelcontroller_elements,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case LEVELCONTROLLER_CNAME:
                server->print(this->getName());
                break;
            case LEVELCONTROLLER_SSELECT:
                strcpy_P(i_sensor_name, pgm_input_sensor);
                parser->selectList(i_sensor_name,
                                   sensorNames,
                                   sensorValuePointers,
                                   m_Sensor + 1,
                                   i,
                                   server);
                break;
            case LEVELCONTROLLER_STATE:
                server->print((__FlashStringHelper *) pgm_read_word(&(template_levelcontroller_states[m_State])));
                break;
            case LEVELCONTROLLER_DELAYHNAME:
                server->print((__FlashStringHelper*) pgm_input_delayh);
                break;
            case LEVELCONTROLLER_DELAYHVAL:
                server->print(m_Delayh);
                break;
            case LEVELCONTROLLER_DELAYHSIZE:
                server->print(4);
                break;
            case LEVELCONTROLLER_DELAYHMAXLENGTH:
                server->print(3);
                break;
            case LEVELCONTROLLER_DELAYLNAME:
                server->print((__FlashStringHelper*) pgm_input_delayl);
                break;
            case LEVELCONTROLLER_DELAYLVAL:
                server->print(m_Delayl);
                break;
            case LEVELCONTROLLER_DELAYLSIZE:
                server->print(4);
                break;
            case LEVELCONTROLLER_DELAYLMAXLENGTH:
                server->print(3);
                break;
            case LEVELCONTROLLER_TIMEOUTNAME:
                server->print((__FlashStringHelper*) pgm_input_timeout);
                break;
            case LEVELCONTROLLER_TIMEOUTVAL:
                server->print(m_Timeout);
                break;
            case LEVELCONTROLLER_TIMEOUTSIZE:
                server->print(4);
                break;
            case LEVELCONTROLLER_TIMEOUTMAXLENGTH:
                server->print(3);
                break;

            }
        }
        templateFile.close();
    }
    return true;
}
#endif
