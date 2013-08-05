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
#include <Aquaduino.h>
#include <TemplateParser.h>
#include <SD.h>
#include <Framework/Flashvars.h>

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

uint16_t LevelController::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_Delayl) + sizeof(m_Delayh)
                      + sizeof(m_Timeout) + sizeof(m_Sensor);
    if (mySize <= size)
    {
        memcpy(bPtr, &m_Sensor, sizeof(m_Sensor));
        offset += sizeof(m_Sensor);
        memcpy(bPtr + offset, &m_Delayl, sizeof(m_Delayl));
        offset += sizeof(m_Delayl);
        memcpy(bPtr + offset, &m_Delayh, sizeof(m_Delayh));
        offset += sizeof(m_Delayh);
        memcpy(bPtr + offset, &m_Timeout, sizeof(m_Timeout));
        offset += sizeof(m_Timeout);
        return mySize;
    }
    return 0;
}

uint16_t LevelController::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(m_Delayl) + sizeof(m_Delayh)
                      + sizeof(m_Timeout);
    if (mySize <= size)
    {
        memcpy(&m_Sensor, bPtr, sizeof(m_Sensor));
        offset += sizeof(m_Sensor);
        if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
            m_Sensor = -1;
        memcpy(&m_Delayl, bPtr + offset, sizeof(m_Delayl));
        offset += sizeof(m_Delayl);
        memcpy(&m_Delayh, bPtr + offset, sizeof(m_Delayh));
        offset += sizeof(m_Delayh);
        memcpy(&m_Timeout, bPtr + offset, sizeof(m_Timeout));
        offset += sizeof(m_Timeout);
        return mySize;
    }
    return 0;
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

    if (m_Sensor < 0 || m_Sensor >= MAX_SENSORS)
        return -1;

    long reading = aquaduino->getSensorValue(m_Sensor);
    unsigned long millisNow = millis();
    long deltaTSwitch = millisNow - lastTime;

    //Check for overflow while processing refill
    //May double the debounce delays in case of overflow
    if (m_State != LEVELCONTROLLER_STATE_OK && lastTime > millisNow)
    {
        lastTime = 0;
    }

    switch (m_State)
    {
    case LEVELCONTROLLER_STATE_OK:
        allMyActuators((int8_t) 0);
        if (reading == HIGH)
        {
            m_State = LEVELCONTROLLER_STATE_DEBOUNCE;
            lastTime = millisNow;
        }
        break;
    case LEVELCONTROLLER_STATE_DEBOUNCE:
        if (reading == HIGH && deltaTSwitch > 1000 * m_Delayh)
        {
            allMyActuators((int8_t) 1);
            lastTime = millisNow;
            m_State = LEVELCONTROLLER_STATE_REFILL;
        }
        else if (reading == LOW)
        {
            m_State = LEVELCONTROLLER_STATE_OK;
        }
        break;
    case LEVELCONTROLLER_STATE_REFILL:
        if (reading == LOW)
        {
            m_State = LEVELCONTROLLER_STATE_OVERRUN;
            lastTime = millisNow;
        }
        else if (reading == HIGH && deltaTSwitch > 1000 * m_Timeout)
        {
            m_State = LEVELCONTROLLER_STATE_REFILL_TIMEOUT;
            allMyActuators((int8_t) 0);
        }
        break;
    case LEVELCONTROLLER_STATE_OVERRUN:
        if (reading == LOW && deltaTSwitch > m_Delayl * 1000)
        {
            m_State = LEVELCONTROLLER_STATE_OK;
            allMyActuators((int8_t) 0);
        }
        else if (reading == HIGH)
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

int8_t LevelController::showWebinterface(WebServer* server,
                                         WebServer::ConnectionType type,
                                         char* url)
{
    File templateFile;
    TemplateParser* parser;
    int8_t matchIdx;
    char i_sensor_name[20];

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
                uint8_t d = atoi(value);
                m_Delayl = d;
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_levelcontroller_inputs[LEVELCONTROLLER_I_DELAYH])))
                     == 0)
            {
                uint8_t d = atoi(value);
                m_Delayh = d;
            }
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_levelcontroller_inputs[LEVELCONTROLLER_I_TIMEOUT])))
                     == 0)
            {
                uint8_t d = atoi(value);
                m_Timeout = d;
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
