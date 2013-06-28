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
#include <Framework/util.h>

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
    m_Pin = 0;
    m_OneWire = NULL;
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
    uint8_t data[12];
    static unsigned long lastIssue = 0;
    static uint8_t readPending = 0;
    int8_t i = 0;

    if (!readPending)
    {
        issueReadCommand(m_Address);
        lastIssue = millis();
        readPending = 1;
    }
    else if (millis() - lastIssue > 750)
    {
        read(m_Address, data, 12);
        temp_hist[runs++] = ((double) convertToRaw(data,
                                                   12,
                                                   m_Address[0] == 0x10))
                            / 16;


        m_Celsius = 0;
        for (i = 0; i < TEMP_HISTORY; i++)
            m_Celsius += temp_hist[i];
        m_Celsius /= TEMP_HISTORY;
        if (runs == TEMP_HISTORY)
            runs = 0;
        readPending = 0;
    }

    return m_Celsius;
}

/**
 * \brief Scans for OneWire devices
 * \param[out] address Buffer where this method stores the address of the
 * device found
 * \param[in] size Size of the buffer. Needs to be at least 8 bytes.
 *
 * \returns 1 when device found. 0 otherwise
 */
int8_t DS18S20::findDevice(uint8_t *address, uint8_t size)
{
    if (size < 8)
        return 0;
    if (!m_OneWire->search(address))
    {
        m_OneWire->reset_search();
        return 0;
    }

    if (OneWire::crc8(address, 7) != address[7])
    {
        return 0;
    }
    return 1;
}

/**
 * \brief Issue OneWire read command to given address
 * \param[in] addr The address of the OneWire device to read from
 */
void DS18S20::issueReadCommand(uint8_t* addr)
{
    if (m_OneWire == NULL)
        return;

    m_OneWire->reset();
    m_OneWire->select(addr);
    m_OneWire->write(0x44, 1); // start conversion, with parasite power on at the end
}

/**
 * \brief Reads the scratchpad of the OneWire device.
 * \param[in] addr Address of the OneWire device
 * \param[out] data Buffer for the data
 * \param[in] size Size of the buffer. Needs to be at least 12 Bytes.
 */
int8_t DS18S20::read(uint8_t* addr, uint8_t* data, uint8_t size)
{
    int8_t i = 0;

    if (size < 12)
        return 0;

    m_OneWire->reset();
    m_OneWire->select(addr);
    m_OneWire->write(0xBE);         // Read Scratchpad

    for (i = 0; i < 9; i++)
    {           // we need 9 bytes
        data[i] = m_OneWire->read();
    }

    return 1;
}

/**
 * \brief Converts the data to a raw 16 bit value
 * \param[in] data The data to be converted
 * \param[in] size Size of the data.
 * \param[in] type Device type. Held in first byte of the address.
 */
uint16_t DS18S20::convertToRaw(uint8_t* data, uint8_t size, int8_t type)
{
    unsigned int raw = (data[1] << 8) | data[0];
    if (type == 1)
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
    return raw;
}

uint16_t DS18S20::serialize(void* buffer, uint16_t size)
{
    memcpy(buffer, &m_Pin, sizeof(m_Pin));
    memcpy(((char*) buffer) + sizeof(m_Pin), &m_Address, sizeof(m_Address));
    return sizeof(m_Pin) + sizeof(m_Address);
}

uint16_t DS18S20::deserialize(void* data, uint16_t size)
{
    memcpy(&m_Pin, data, sizeof(m_Pin));
    memcpy(&m_Address, ((char*) data) + sizeof(m_Pin), sizeof(m_Address));
    if (m_OneWire == NULL)
        m_OneWire = new OneWire(m_Pin);
    return sizeof(m_Pin);
}

const static char progTemplateFileName[] PROGMEM = "DS18S20.htm";
const static char progTemplateString1[] PROGMEM = "##INAME##";
const static char progTemplateString2[] PROGMEM = "##NAME##";
const static char progTemplateString3[] PROGMEM = "##PIN##";
const static char progTemplateString4[] PROGMEM = "##ADDRESSSELECT##";

const static char* const templateStrings[] PROGMEM =
    { progTemplateString1, progTemplateString2, progTemplateString3,
      progTemplateString4 };

static const char progInputPin[] PROGMEM = "ipin";
static const char progInputAddress[] PROGMEM = "address";
static const char* const inputStrings[] PROGMEM =
    { progInputPin, progInputAddress };

enum
{
    S_INAME, S_NAME, S_PIN, S_ADDRESSELECT
};

enum
{
    I_PIN, I_ADDRESS
};

int8_t DS18S20::showWebinterface(WebServer* server,
                                 WebServer::ConnectionType type, char* url)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[sizeof(progTemplateFileName)];
    strcpy_P(templateFileName, progTemplateFileName);
    uint8_t address[4][8];
    char addressNames[4][17];
    char* names[4];
    int8_t i = 0;

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[20], value[20];
        do
        {
            repeat = server->readPOSTparam(name, 20, value, 20);
            if (strcmp_P(name, (PGM_P) pgm_read_word(&(inputStrings[I_PIN]))) == 0)
                m_Pin = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(inputStrings[I_ADDRESS])))
                == 0)
                sth(value, m_Address, 8);
        } while (repeat);
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();

        for (i = 0; i < 4; i++)
        {
            if (!findDevice(address[i], 8))
                break;
            hts(address[i], 8, addressNames[i], 17);
            names[i] = addressNames[i];
        }

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
                server->print((__FlashStringHelper *) &inputStrings[0][0]);
                break;
            case S_NAME:
                server->print(getName());
                break;
            case S_PIN:
                server->print(m_Pin);
                break;
            case S_ADDRESSELECT:
                parser->selectList("address", names, names, 0, i, server);
                break;
            }
        }

        templateFile.close();
    }
    return true;
}

