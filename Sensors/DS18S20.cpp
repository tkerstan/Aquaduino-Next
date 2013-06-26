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

#include "DS18S20.h"
#include <Aquaduino.h>
#include "DigitalInput.h"
#include <Arduino.h>
#include <SD.h>
#include <TemplateParser.h>

#define TEMP_HISTORY	10

static float temp_hist[TEMP_HISTORY];

static byte runs = 0;

/**
 * \brief Constructor
 * \param[in] pin Pin where the DS18S20 is attached.
 */
DS18S20::DS18S20()
{
    int i;
    m_Type = SENSOR_DS18S20;
    for (i = 0; i < TEMP_HISTORY; i++)
    {
        temp_hist[i] = 0.0;
    }
    myPin = 0;
    myOneWire = NULL;
}

/**
 * \brief Reads the sensor
 *
 * Currently only supports one DS18S20.
 * Reading is done on two phases to remove delay calls.
 * First phase (readPending = 0) searches for attached DS18S20 and issues a
 * conversion.
 *
 * Second phase (readPending = 1) reads the value when more than one second
 * has passed since the read was issued.
 */
double DS18S20::read()
{
    byte i;
    static byte type_s;
    static byte data[12];
    static byte addr[8];
    static int readPending = false;
    static unsigned long lastReadCommand = 0;

    if (myOneWire == NULL)
        return 0;

    if (readPending == 0)
    {
        if (!myOneWire->search(addr))
        {
            myOneWire->reset_search();
            return celsius;
        }

        if (OneWire::crc8(addr, 7) != addr[7])
        {
            return 0.0;
        }

        // the first ROM byte indicates which chip
        switch (addr[0])
        {
        case 0x10:
            type_s = 1;
            break;
        case 0x28:
            type_s = 0;
            break;
        case 0x22:
            type_s = 0;
            break;
        default:
            return 0.0;
        }
        myOneWire->reset();
        myOneWire->select(addr);
        myOneWire->write(0x44, 1); // start conversion, with parasite power on at the end
        lastReadCommand = millis();
        readPending = 1;

    }
    else if ((readPending == 1) && (millis() - lastReadCommand > 750))
    {
        myOneWire->reset();
        myOneWire->select(addr);
        myOneWire->write(0xBE);         // Read Scratchpad

        for (i = 0; i < 9; i++)
        {           // we need 9 bytes
            data[i] = myOneWire->read();
        }

        // convert the data to actual temperature

        unsigned int raw = (data[1] << 8) | data[0];
        if (type_s)
        {
            raw = raw << 3; // 9 bit resolution default
            if (data[7] == 0x10)
            {
                // count remain gives full 12 bit resolution
                raw = (raw & 0xFFF0) + 12 - data[6];
            }
        }
        else
        {
            byte cfg = (data[4] & 0x60);
            if (cfg == 0x00)
                raw = raw << 3;  // 9 bit resolution, 93.75 ms
            else if (cfg == 0x20)
                raw = raw << 2; // 10 bit res, 187.5 ms
            else if (cfg == 0x40)
                raw = raw << 1; // 11 bit res, 375 ms
            // default is 12 bit resolution, 750 ms conversion time
        }
        celsius = (float) raw / 16.0;
        fahrenheit = celsius * 1.8 + 32.0;
        readPending = 0;
        temp_hist[runs] = celsius;
        celsius = 0;
        runs = (runs + 1) % TEMP_HISTORY;
        for (i = 0; i < TEMP_HISTORY; i++)
        {
            celsius += temp_hist[i];
        }
        celsius /= TEMP_HISTORY;
    }
    return celsius;
}

uint16_t DS18S20::serialize(void* buffer, uint16_t size)
{
    memcpy(buffer, &myPin, sizeof(myPin));
    return sizeof(myPin);
}

uint16_t DS18S20::deserialize(void* data, uint16_t size)
{
    memcpy(&myPin, data, sizeof(myPin));
    if (myOneWire == NULL)
        myOneWire = new OneWire(myPin);
    return sizeof(myPin);
}

const static char progTemplateFileName[] PROGMEM = "DS18S20.htm";
const static char progTemplateString1[] PROGMEM = "##INAME##";
const static char progTemplateString2[] PROGMEM = "##NAME##";
const static char progTemplateString3[] PROGMEM = "##PIN##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3};

static const char progInput[] PROGMEM = "ipin";
static const char* const inputStrings[] PROGMEM =
    { progInput };


enum
{
    S_INAME,
    S_NAME,
    S_PIN
};

enum
{
    I_TYPE
};

int8_t DS18S20::showWebinterface(WebServer* server, WebServer::ConnectionType type,
                        char* url)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[sizeof(progTemplateFileName)];
    strcpy_P(templateFileName, progTemplateFileName);

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[16], value[16];
        do
        {
            repeat = server->readPOSTparam(name, 16, value, 16);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_TYPE]))) == 0)
            {
                myPin = atoi(value);
            }
        } while (repeat);
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
                                                      sizeof(templateStrings) / sizeof(char*),
                                                      server))
               >= 0)
        {
            switch (matchIdx)
            {
            case S_INAME:
                server->print((__FlashStringHelper*) &inputStrings[0][0]);
                break;
            case S_NAME:
                server->print(getName());
                break;
            case S_PIN:
                server->print(myPin);
                break;
            }
        }

        templateFile.close();
    }
    return true;
}

