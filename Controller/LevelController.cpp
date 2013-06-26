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

const static uint8_t LEVELCONTROLLER_STATE_OK = 0;
const static uint8_t LEVELCONTROLLER_STATE_DEBOUNCE = 1;
const static uint8_t LEVELCONTROLLER_STATE_REFILL = 2;
const static uint8_t LEVELCONTROLLER_STATE_OVERRUN = 3;
const static uint8_t LEVELCONTROLLER_STATE_REFILL_TIMEOUT = 4;

const static char progTemplateFileName[] PROGMEM = "level.htm";
const static char progTemplateString1[] PROGMEM = "##SSELECT##";
const static char progTemplateString2[] PROGMEM = "##STATE##";
const static char progTemplateString3[] PROGMEM = "##DELAYLOW##";
const static char progTemplateString4[] PROGMEM = "##DELAYHIGH##";
const static char progTemplateString5[] PROGMEM = "##REFILLTIMEOUT##";

enum
{
    LC_SSELECT,
    LC_STATE,
    LC_DELAYLOW,
    LC_DELAYHIGH,
    LC_REFILLTIMEOUT
};

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3,
      progTemplateString4, progTemplateString5 };

const static char progStateString1[] PROGMEM = "OK";
const static char progStateString2[] PROGMEM = "DEBOUNCE";
const static char progStateString3[] PROGMEM = "REFILL";
const static char progStateString4[] PROGMEM = "OVERRUN";
const static char progStateString5[] PROGMEM = "TIMEOUT";

const static char progInputDelayLow[] PROGMEM = "delayLow";
const static char progInputDelayHigh[] PROGMEM = "delayHigh";
const static char progInputRefillTimeout[] PROGMEM = "refillTimeout";

/**
 * \brief Constructor
 * \param[in] name Name of the controller
 */
LevelController::LevelController(const char* name) :
        Controller(name)
{
    m_Type = CONTROLLER_LEVEL;
    debounceDelayHigh = 3;
    hysteresis = 10;
    state = LEVELCONTROLLER_STATE_OK;
    refillTimeout = 30;
}

uint16_t LevelController::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(hysteresis) + sizeof(debounceDelayHigh)
                      + sizeof(refillTimeout);
    if (mySize <= size)
    {
        memcpy(bPtr, &hysteresis, sizeof(hysteresis));
        offset += sizeof(hysteresis);
        memcpy(bPtr + offset, &debounceDelayHigh, sizeof(debounceDelayHigh));
        offset += sizeof(debounceDelayHigh);
        memcpy(bPtr + offset, &refillTimeout, sizeof(refillTimeout));
        return mySize;
    }
    return 0;
}

uint16_t LevelController::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(hysteresis) + sizeof(debounceDelayHigh)
                      + sizeof(refillTimeout);
    if (mySize <= size)
    {
        memcpy(&hysteresis, bPtr, sizeof(hysteresis));
        offset += sizeof(hysteresis);
        memcpy(&debounceDelayHigh, bPtr + offset, sizeof(debounceDelayHigh));
        offset += sizeof(debounceDelayHigh);
        memcpy(&refillTimeout, bPtr + offset, sizeof(refillTimeout));
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

    long reading = 0;
    unsigned long millisNow = millis();
    long deltaTSwitch = millisNow - lastTime;

    //Check for overflow while processing refill
    //May double the debounce delays in case of overflow
    if (state != LEVELCONTROLLER_STATE_OK && lastTime > millisNow)
    {
        lastTime = 0;
    }

    switch (state)
    {
    case LEVELCONTROLLER_STATE_OK:
        allMyActuators(0);
        if (reading == HIGH)
        {
            state = LEVELCONTROLLER_STATE_DEBOUNCE;
            lastTime = millisNow;
        }
        break;
    case LEVELCONTROLLER_STATE_DEBOUNCE:
        if (reading == HIGH && deltaTSwitch > 1000 * debounceDelayHigh)
        {
            allMyActuators(1);
            lastTime = millisNow;
            state = LEVELCONTROLLER_STATE_REFILL;
        }
        else if (reading == LOW)
        {
            state = LEVELCONTROLLER_STATE_OK;
        }
        break;
    case LEVELCONTROLLER_STATE_REFILL:
        if (reading == LOW)
        {
            state = LEVELCONTROLLER_STATE_OVERRUN;
            lastTime = millisNow;
        }
        else if (reading == HIGH && deltaTSwitch > 1000 * refillTimeout)
        {
            state = LEVELCONTROLLER_STATE_REFILL_TIMEOUT;
            allMyActuators(0);
        }
        break;
    case LEVELCONTROLLER_STATE_OVERRUN:
        if (reading == LOW && deltaTSwitch > hysteresis * 1000)
        {
            state = LEVELCONTROLLER_STATE_OK;
            allMyActuators(0);
        }
        else if (reading == HIGH)
        {
            state = LEVELCONTROLLER_STATE_REFILL;
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

    char* states[5];

    char templateFileName[sizeof(progTemplateFileName)];

    char stateString1[sizeof(progStateString1)];
    char stateString2[sizeof(progStateString2)];
    char stateString3[sizeof(progStateString3)];
    char stateString4[sizeof(progStateString4)];
    char stateString5[sizeof(progStateString5)];

    strcpy_P(templateFileName, progTemplateFileName);
    strcpy_P(stateString1, progStateString1);
    strcpy_P(stateString2, progStateString2);
    strcpy_P(stateString3, progStateString3);
    strcpy_P(stateString4, progStateString4);
    strcpy_P(stateString5, progStateString5);

    itoa(hysteresis, debounceLowString, 10);
    itoa(debounceDelayHigh, debounceHighString, 10);
    itoa(refillTimeout, refillTimeoutString, 10);

    states[0] = stateString1;
    states[1] = stateString2;
    states[2] = stateString3;
    states[3] = stateString4;
    states[4] = stateString5;


    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name, progInputDelayLow) == 0)
            {
                uint8_t d = atoi(value);
                hysteresis = d;
            }
            else if (strcmp_P(name, progInputDelayHigh) == 0)
            {
                uint8_t d = atoi(value);
                debounceDelayHigh = d;
            }
            else if (strcmp_P(name, progInputRefillTimeout) == 0)
            {
                uint8_t d = atoi(value);
                refillTimeout = d;
            }
        } while (repeat);
        state = LEVELCONTROLLER_STATE_OK;
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();
        parser = aquaduino->getTemplateParser();
        templateFile = SD.open(templateFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      templateStrings,
                                                      sizeof(templateStrings)/
                                                      sizeof(char*),
                                                      server))
               != -1)
        {
            switch(matchIdx)
            {
            case LC_SSELECT:
                break;
            case LC_STATE:
                server->print((__FlashStringHelper*) &states[state]);
                break;
            case LC_DELAYLOW:
                server->print((__FlashStringHelper*) progInputDelayLow);
                break;
            case LC_DELAYHIGH:
                server->print((__FlashStringHelper*) &progInputDelayHigh);
                break;
            case LC_REFILLTIMEOUT:
                server->print((__FlashStringHelper*) progInputRefillTimeout);
                break;
            }
        }
        templateFile.close();
    }
    return true;
}
