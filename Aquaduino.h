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
#include "Framework/ArrayMap.h"
#include "Framework/SDConfigManager.h"
#include "Framework/Object.h"
#include "Framework/Serializable.h"
#include "Framework/WebInterface.h"

class WebServer;
class Controller;
class Actuator;
class Sensor;
class ConfigManager;

/*! \brief Aquaduino main class.
 *
 *  The Aquaduino class contains all elements controlled by Aquaduino.
 *  - Controller
 *  - Actuator
 *  - Sensor
 *
 *  To configure Aquaduino a WebServer based on Webduino is integrated.
 *  The WebServer is used to provide a configuration page for:
 *  - Network configuration
 *  - Controller configuration
 *  - Actuator configuration
 */
class Aquaduino: public Object, Serializable, WebInterface
{
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

    void setTimezone(int8_t zone);
    int8_t getTimezone();

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

    virtual int8_t showWebinterface(WebServer* server,
                                    WebServer::ConnectionType type, char* url);

    void run();

protected:
    void printConfigWebpage(WebServer* server);
    int8_t configWebpageProcessPost(WebServer* server,
                                    WebServer::ConnectionType type);
    int8_t configWebpage(WebServer* server, WebServer::ConnectionType type);
    int8_t mainWebpageProcessPost(WebServer* server, WebServer::ConnectionType type);
    int8_t printMainActuatorTable(WebServer* server);
    int8_t printMainControllerTable(WebServer* server);
    int8_t printMainWebpage(WebServer* server);
    int8_t mainWebpage(WebServer* server, WebServer::ConnectionType type);


private:
    byte m_MAC[6];
    IPAddress m_IP, m_Netmask, m_DNSServer, m_Gateway, m_NTPServer;
    int8_t m_Timezone;
    uint16_t m_NTPSyncInterval;
    int8_t m_DHCP;
    int8_t m_NTP;

    ArrayMap<Controller*> m_Controllers;
    ArrayMap<Actuator*> m_Actuators;
    Sensor* m_TemperatureSensor;
    Sensor* m_LevelSensor;
    WebServer* m_WebServer;
    TemplateParser* m_TemplateParser;
    double m_Temperature;
    double m_Level;

    ConfigManager* m_ConfigManager;
};

extern Aquaduino* aquaduino;

#endif /* Aquaduino_H_ */

