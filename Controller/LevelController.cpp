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
const static char progTemplateString1[] PROGMEM = "##STATE##";
const static char progTemplateString2[] PROGMEM = "##DELAYLOW##";
const static char progTemplateString3[] PROGMEM = "##DELAYHIGH##";
const static char progTemplateString4[] PROGMEM = "##REFILLTIMEOUT##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3,
      progTemplateString4 };

const static char progStateString1[] PROGMEM = "OK";
const static char progStateString2[] PROGMEM = "DEBOUNCE";
const static char progStateString3[] PROGMEM = "REFILL";
const static char progStateString4[] PROGMEM = "OVERRUN";
const static char progStateString5[] PROGMEM = "TIMEOUT";

const static char progInputDelayLow[] PROGMEM = "delayLow";
const static char progInputDelayHigh[] PROGMEM = "delayHigh";
const static char progInputRefillTimeout[] PROGMEM = "refillTimeout";

LevelController::LevelController(const char* name, uint8_t pin) :
        Controller(name)
{
    m_Type = CONTROLLER_LEVEL;
    myPin = pin;
    myActor = NULL;
    debounceDelayHigh = 3;
    debounceDelayLow = 10;
    lastTime = 0;
    state = LEVELCONTROLLER_STATE_OK;
    lastState = LEVELCONTROLLER_STATE_OK;
    refillTimeout = 30;
    pinMode(myPin, INPUT);
    digitalWrite(myPin, LOW);
}

uint16_t LevelController::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;
    uint8_t offset = 0;

    uint16_t mySize = sizeof(debounceDelayLow) + sizeof(debounceDelayHigh)
                      + sizeof(refillTimeout);
    if (mySize <= size)
    {
        memcpy(bPtr, &debounceDelayLow, sizeof(debounceDelayLow));
        offset += sizeof(debounceDelayLow);
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

    uint16_t mySize = sizeof(debounceDelayLow) + sizeof(debounceDelayHigh)
                      + sizeof(refillTimeout);
    if (mySize <= size)
    {
        memcpy(&debounceDelayLow, data, sizeof(debounceDelayLow));
        offset += sizeof(debounceDelayLow);
        memcpy(&debounceDelayHigh, data + offset, sizeof(debounceDelayHigh));
        offset += sizeof(debounceDelayHigh);
        memcpy(&refillTimeout, data + offset, sizeof(refillTimeout));
        return mySize;
    }
    return 0;
}

int8_t LevelController::run()
{
    long reading = round(aquaduino->getLevel());
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
        if (reading == LOW && deltaTSwitch > debounceDelayLow * 1000)
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
                                         WebServer::ConnectionType type)
{
    File templateFile;
    TemplateParser* parser;

    char* replacementStrings[4];
    char debounceLowString[4];
    char debounceHighString[4];
    char refillTimeoutString[4];
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

    itoa(debounceDelayLow, debounceLowString, 10);
    itoa(debounceDelayHigh, debounceHighString, 10);
    itoa(refillTimeout, refillTimeoutString, 10);

    states[0] = stateString1;
    states[1] = stateString2;
    states[2] = stateString3;
    states[3] = stateString4;
    states[4] = stateString5;

    replacementStrings[0] = states[state];
    replacementStrings[1] = debounceLowString;
    replacementStrings[2] = debounceHighString;
    replacementStrings[3] = refillTimeoutString;

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
                debounceDelayLow = d;
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
        lastTime = 0;
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
                                      4,
                                      server);
        templateFile.close();
    }
    return true;
}
