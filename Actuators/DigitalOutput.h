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

#ifndef DIGITALOUTPUT_H_
#define DIGITALOUTPUT_H_

#include <Framework/Actuator.h>

/**
 * \brief Class for the digital outputs of the Arduino
 *
 * Implements only the functionality for pure digital outputs.
 */
class DigitalOutput: public Actuator
{
private:
    int8_t m_Pin;
    uint8_t m_OnValue;
    uint8_t m_OffValue;
    uint8_t m_On;
    float m_DutyCycle;
public:
    DigitalOutput(const char* name, uint8_t onValue, uint8_t offValue);

    virtual uint16_t serialize(void* buffer, uint16_t size);
    virtual uint16_t deserialize(void* data, uint16_t size);

    virtual void on();
    virtual void off();
    virtual void forceOn();
    virtual void forceOff();

    virtual int8_t isOn();
    virtual int8_t supportsPWM();

    virtual void setPWM(float dutyCycle);
    virtual float getPWM();

    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type, char* url);

};

#endif /* DIGITALOUTPUT_H_ */
