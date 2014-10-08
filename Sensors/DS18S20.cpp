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
#include <Framework/Aquaduino.h>
#include "DigitalInput.h"
#include <Arduino.h>
#include <SD.h>
#include <Framework/util.h>
#include <Framework/OneWireHandler.h>

#ifdef FEATURE_WEBIF
#include <Framework/Flashvars.h>
#include <TemplateParser.h>
#endif


/**
 * \brief Constructor
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
    m_Idx = 0;
    m_ReadPending = 0;
    m_LastReadIssue = 0;
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
    int8_t i = 0;
    OneWireHandler* handler = __aquaduino->getOneWireHandler();

    if (!m_ReadPending)
    {
        handler->issueReadCommand(m_Idx, m_Address);
        m_LastReadIssue = millis();
        m_ReadPending = 1;
    }
    else if (millis() - m_LastReadIssue > 750)
    {
        if (handler->read(m_Idx, m_Address, data, 12))
            return 0.0;
        temp_hist[m_Runs++] = ((double) convertToRaw(data,
                                                   12,
                                                   m_Address[0] == 0x10))
                            / 16;


        m_Celsius = 0;
        for (i = 0; i < TEMP_HISTORY; i++)
            m_Celsius += temp_hist[i];
        m_Celsius /= TEMP_HISTORY;
        if (m_Runs == TEMP_HISTORY)
            m_Runs = 0;
        m_ReadPending = 0;
    }

    return m_Celsius;
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

uint16_t DS18S20::serialize(Stream* s)
{
	s->write(m_Pin);
	s->write(m_Address, sizeof(m_Address));
    return sizeof(m_Pin) + sizeof(m_Address);
}

uint16_t DS18S20::deserialize(Stream* s)
{
    OneWireHandler* handler = __aquaduino->getOneWireHandler();

    m_Pin = s->read();
	s->readBytes((char*)m_Address, sizeof(m_Address));
    if (handler != NULL)
        m_Idx = handler->addPin(m_Pin);
    return sizeof(m_Pin) + sizeof(m_Address);
}

void DS18S20::setPin(uint8_t pin)
{
    m_Pin = pin;
}

uint8_t DS18S20::getPin()
{
    return m_Pin;
}

#ifdef FEATURE_WEBIF
int8_t DS18S20::showWebinterface(WebServer* server,
                                 WebServer::ConnectionType type, char* url)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx;
    char templateFileName[template_ds18s20_fnsize];
    strcpy_P(templateFileName, template_ds18s20_fname);
    uint8_t address[4][8];
    char addressNames[5][17];
    char* names[5];
    char input_name[20];

    int8_t i = 0;
    int8_t selected = 0;
    OneWireHandler* handler = __aquaduino->getOneWireHandler();

    if (type == WebServer::POST)
    {
        int8_t repeat;
        char name[20], value[20];
        do
        {
            repeat = server->readPOSTparam(name, 20, value, 20);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_ds18s20_inputs[DS18S20_I_PIN]))) == 0)
                m_Pin = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_ds18s20_inputs[DS18S20_I_ADDRESS])))
                == 0)
            {
                sth(value, m_Address, 8);
            }
        } while (repeat);
        server->httpSeeOther(this->m_URL);
    }
    else
    {
        server->httpSuccess();

        memset(addressNames[0], 0, 17);
        names[0] = addressNames[0];

        for (i = 1; i < 5; i++)
        {
            if (!handler->findDevice(m_Idx, address[i], 8))
                break;
            hts(address[i], 8, addressNames[i], 17);
            names[i] = addressNames[i];
            if (memcmp(address[i], m_Address,8) == 0)
                selected = i;
        }

        parser = __aquaduino->getTemplateParser();

        templateFile = SD.open(templateFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateFile,
                                                      template_ds18s20,
                                                      template_ds18s20_elements,
                                                      server))
               >= 0)
        {
            switch (matchIdx)
            {
            case DS18S20_SNAME:
                server->print(getName());
                break;
            case DS18S20_PIN_NAME:
                server->print((__FlashStringHelper *) pgm_input_pin);
                break;
            case DS18S20_PIN_VAL:
                server->print(m_Pin);
                break;
            case DS18S20_PIN_SIZE:
                server->print(3);
                break;
            case DS18S20_PIN_MAXLENGTH:
                server->print(2);
                break;
            case DS18S20_ADDRESSSELECT:
                strcpy_P(input_name, pgm_input_address);
                parser->selectList(input_name, names, names, selected, i, server);
                break;
            }
        }

        templateFile.close();
    }
    return true;
}
#endif
