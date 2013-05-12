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

#ifndef Aquaduino_H_
#define Aquaduino_H_
#include <Arduino.h>
#include <Ethernet.h>
#include <TemplateParser.h>

#include "Framework/Config.h"
#include "Framework/Controller.h"
#include "Framework/Actuator.h"
#include "Framework/Sensor.h"
#include "Framework/ArrayList.h"
#include "Framework/SDConfigManager.h"
#include "Framework/Object.h"
#include "Framework/Serializable.h"

class WebServer;
class Controller;
class Actuator;
class Sensor;
class ConfigManager;

class Aquaduino: public Object, Serializable
{
private:
    byte myMAC[6];
    IPAddress myIP, myNetmask, myDNS, myGateway, myNTP;
    uint16_t ntpSyncInterval;
    int8_t doDHCP;
    int8_t doNTP;

    ArrayList<Controller*> m_Controllers;
    ArrayList<Actuator*> m_Actuators;
    Sensor* temperatureSensor;
    Sensor* levelSensor;
    WebServer* myWebServer;
    TemplateParser* m_TemplateParser;
    double temp;
    double level;

    ConfigManager* m_ConfigManager;

public:
    Aquaduino();

    void setMAC(uint8_t* mac);
    void getMAC(uint8_t* mac);

    IPAddress* getIP();
    void setIP(IPAddress* ip);

    IPAddress* getNetmask();
    void setNetmask(IPAddress* netmask);

    IPAddress* getGateway();
    void setGateway(IPAddress* gateway);

    IPAddress* getDNS();
    void setDNS(IPAddress* dns);

    IPAddress* getNTP();
    void setNTP(IPAddress* ntp);

    uint16_t getNtpSyncInterval();
    void setNtpSyncInterval(uint16_t syncInterval);

    void enableDHCP();
    void disableDHCP();
    int8_t isDHCPEnabled();

    void enableNTP();
    void disableNTP();
    int8_t isNTPEnabled();
    void setTime(int8_t hour, int8_t minute, int8_t second, int8_t day,
                 int8_t month, int16_t year);

    int8_t addController(Controller* newController);
    Controller* getController(unsigned int controller);
    int8_t getControllerID(Controller* controller);
    void resetControllerIterator();
    int8_t getNextController(Controller** controller);
    unsigned char getNrOfControllers();

    int8_t addActuator(Actuator* newActuator);
    Actuator* getActuator(unsigned int actuator);
    int8_t getActuatorID(Actuator* actuator);
    void resetActuatorIterator();
    int8_t getNextActuator(Actuator** actuator);
    int8_t getAssignedActuators(Controller* controller, Actuator** actuators,
                                int8_t max);
    int8_t getAssignedActuatorIDs(Controller* controller, int8_t* actuatorIDs,
                                  int8_t max);
    unsigned char getNrOfActuators();

    uint16_t serialize(void* buffer, uint16_t size);
    uint16_t deserialize(void* data, uint16_t size);

    int8_t writeConfig(Aquaduino* aquaduino);
    int8_t writeConfig(Actuator* actuator);
    int8_t writeConfig(Controller* controller);
    int8_t writeConfig(Sensor* sensor);

    int8_t readConfig(Aquaduino* aquaduino);
    int8_t readConfig(Actuator* actuator);
    int8_t readConfig(Controller* controller);
    int8_t readConfig(Sensor* sensor);

    void setWebserver(WebServer* webServer);
    WebServer* getWebserver();

    void setTemplateParser(TemplateParser* parser);
    TemplateParser* getTemplateParser();

    void setTemperatureSensor(Sensor* tempSensor);
    double getTemperature();

    void setLevelSensor(Sensor* levSensor);
    double getLevel();

    void run();

};

extern Aquaduino* aquaduino;

#endif /* Aquaduino_H_ */

