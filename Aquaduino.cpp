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

#include <Aquaduino.h>
#include <Controller/TemperatureController.h>
#include <Controller/LevelController.h>
#include <Controller/ClockTimerController.h>
#include <Actuators/DigitalOutput.h>
#include <Sensors/DS18S20.h>
#include <Sensors/DigitalInput.h>

#include <Time.h>
#include <EthernetUdp.h>
#include <WebServer.h>
#include <SD.h>
#include <stdlib.h>

Aquaduino* aquaduino;

extern void defaultCmd(WebServer &server, WebServer::ConnectionType type,
                       char *, bool);
extern void controllerDispatchCommand(WebServer &server,
                                      WebServer::ConnectionType type,
                                      char **url_path, char *url_tail,
                                      bool tail_complete);

Aquaduino::Aquaduino() :
        myIP(192, 168, 1, 222),
        myNetmask(255, 255, 255, 0),
        myDNS(192, 168, 1, 1),
        myGateway(192, 168, 1, 1),
        myNTP(192, 53, 103, 108),
        ntpSyncInterval(5),
        doDHCP(0),
        doNTP(0),
        m_Controllers(MAX_CONTROLLERS),
        m_Actuators(MAX_ACTORS),
        temperatureSensor(NULL),
        levelSensor(NULL),
        myWebServer(NULL),
        m_TemplateParser(NULL),
        temp(0),
        level(0)
{
    m_Type = AQUADUINO;
    // Deselect all SPI devices!
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);

    if (!SD.begin(4))
    {
        Serial.println(F("No SD Card available"));
        while(1);
    }

    myMAC[0] = 0x00;
    myMAC[1] = 0x01;
    myMAC[2] = 0x02;
    myMAC[3] = 0x03;
    myMAC[4] = 0x04;
    myMAC[5] = 0x05;

#ifdef DEBUG
    Serial.print(F("IP: "));
    Serial.println(myIP);
    Serial.print(F("Netmask: "));
    Serial.println(myNetmask);
    Serial.print(F("Gateway: "));
    Serial.println(myGateway);
    Serial.print(F("DNS Server: "));
    Serial.println(myDNS);
    Serial.print(F("NTP Server: "));
    Serial.println(myNTP);
#endif

    m_ConfigManager = new SDConfigManager("config");

    Ethernet.begin(myMAC, myIP, myDNS, myGateway, myNetmask);
}

IPAddress* Aquaduino::getIP()
{
    return &myIP;
}
void Aquaduino::setIP(IPAddress* ip)
{
    myIP = *ip;
}

IPAddress* Aquaduino::getNetmask()
{
    return &myNetmask;
}

void Aquaduino::setNetmask(IPAddress* netmask)
{
    myNetmask = *netmask;
}

IPAddress* Aquaduino::getGateway()
{
    return &myGateway;
}

void Aquaduino::setGateway(IPAddress* gateway)
{
    myGateway = *gateway;
}

IPAddress* Aquaduino::getDNS()
{
    return &myDNS;
}

void Aquaduino::setDNS(IPAddress* dns)
{
    myDNS = *dns;
}

IPAddress* Aquaduino::getNTP()
{
    return &myNTP;
}

void Aquaduino::setNTP(IPAddress* ntp)
{
    myNTP = *ntp;
}

uint16_t Aquaduino::getNtpSyncInterval()
{
    return ntpSyncInterval;
}

void Aquaduino::setNtpSyncInterval(uint16_t syncInterval)
{
    ntpSyncInterval = syncInterval;
}

void Aquaduino::enableDHCP()
{
    doDHCP = 1;
}

void Aquaduino::disableDHCP()
{
    doDHCP = 0;
}

int8_t Aquaduino::isDHCPEnabled()
{
    return doDHCP;
}

void Aquaduino::enableNTP()
{
    doNTP = 1;
}

void Aquaduino::disableNTP()
{
    doNTP = 0;
}

int8_t Aquaduino::isNTPEnabled()
{
    return doNTP;
}

void Aquaduino::setTime(int8_t hour, int8_t minute, int8_t second, int8_t day,
                        int8_t month, int16_t year)
{
    if (!doNTP)
        ::setTime(hour, minute, second, day, month, year);
}

void Aquaduino::addController(Controller* newController)
{
    int8_t status = m_Controllers.add(newController);
#ifdef DEBUG
    Serial.print(F("Adding controller "));
    Serial.print(newController->getName());
    Serial.print(F(" @ position "));
    Serial.println(status);
#endif
}

Controller* Aquaduino::getController(unsigned int controller)
{
    return m_Controllers.get(controller);
}

int8_t Aquaduino::getControllerID(Controller* controller)
{
    return m_Controllers.findElement(controller);
}

void Aquaduino::resetControllerIterator()
{
    m_Controllers.resetIterator();
}

int8_t Aquaduino::getNextController(Controller** controller)
{
    return m_Controllers.getNext(controller);
}

unsigned char Aquaduino::getNrOfControllers()
{
    return m_Controllers.getNrOfElements();
}

void Aquaduino::addActuator(Actuator* newActor)
{
    int8_t status = m_Actuators.add(newActor);
#ifdef DEBUG
    Serial.print(F("Adding actuator "));
    Serial.print(newActor->getName());
    Serial.print(F(" @ position "));
    Serial.println(status);
#endif
}

Actuator* Aquaduino::getActuator(unsigned int actor)
{
    return m_Actuators.get(actor);
}

int8_t Aquaduino::getActuatorID(Actuator* actuator)
{
    return m_Actuators.findElement(actuator);
}

void Aquaduino::resetActuatorIterator()
{
    m_Actuators.resetIterator();
}

int8_t Aquaduino::getNextActuator(Actuator** actuator)
{
    return m_Actuators.getNext(actuator);
}

int8_t Aquaduino::getAssignedActuators(Controller* controller,
                                       Actuator** actuators, int8_t max)
{
    int8_t actorIdx = -1;
    int8_t nrOfAssignedActuators = 0;
    Actuator* currentActuator;
    int8_t controllerIdx = m_Controllers.findElement(controller);

    m_Actuators.resetIterator();
    while ((actorIdx = m_Actuators.getNext(&currentActuator)) != -1)
    {
        if (currentActuator->getController() == controllerIdx)
        {
            if (nrOfAssignedActuators < max)
                actuators[nrOfAssignedActuators] = currentActuator;
            nrOfAssignedActuators++;
        }
    }
    return nrOfAssignedActuators;
}

unsigned char Aquaduino::getNrOfActuators()
{
    return m_Actuators.getNrOfElements();
}

uint16_t Aquaduino::serialize(void* buffer, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) buffer;

    memcpy(bPtr, myMAC, sizeof(myMAC));
    bPtr += sizeof(myMAC);
    memcpy(bPtr, &myIP[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &myNetmask[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &myDNS[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &myGateway[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &myNTP[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &ntpSyncInterval, sizeof(uint16_t));
    bPtr += sizeof(uint16_t);
    memcpy(bPtr, &doDHCP, sizeof(uint16_t));
    bPtr += sizeof(int8_t);
    memcpy(bPtr, &doNTP, sizeof(uint16_t));
    bPtr += sizeof(int8_t);

    return (uint16_t) bPtr - (uint16_t) buffer;
}

uint16_t Aquaduino::deserialize(void* data, uint16_t size)
{
    uint8_t* bPtr = (uint8_t*) data;

    memcpy(myMAC, bPtr, sizeof(myMAC));
    bPtr += sizeof(myMAC);
    memcpy(&myIP[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&myNetmask[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&myDNS[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&myGateway[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&myNTP[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&ntpSyncInterval, bPtr, sizeof(uint16_t));
    bPtr += sizeof(uint16_t);
    memcpy(&doDHCP, bPtr, sizeof(uint16_t));
    bPtr += sizeof(int8_t);
    memcpy(&doNTP, bPtr, sizeof(uint16_t));
    bPtr += sizeof(int8_t);

    return (uint16_t) bPtr - (uint16_t) data;
}
int8_t Aquaduino::writeConfig(Aquaduino* aquaduino)
{
    return m_ConfigManager->writeConfig(aquaduino);
}


int8_t Aquaduino::writeConfig(Actuator* actuator)
{
    return m_ConfigManager->writeConfig(actuator);
}

int8_t Aquaduino::writeConfig(Controller* controller)
{
    return m_ConfigManager->writeConfig(controller);
}

int8_t Aquaduino::writeConfig(Sensor* sensor)
{
    return m_ConfigManager->writeConfig(sensor);
}

int8_t Aquaduino::readConfig(Aquaduino* aquaduino)
{
    return m_ConfigManager->readConfig(aquaduino);
}


int8_t Aquaduino::readConfig(Actuator* actuator)
{
    return m_ConfigManager->readConfig(actuator);
}

int8_t Aquaduino::readConfig(Controller* controller)
{
    return m_ConfigManager->readConfig(controller);
}

int8_t Aquaduino::readConfig(Sensor* sensor)
{
    return m_ConfigManager->readConfig(sensor);
}

void Aquaduino::setTemperatureSensor(Sensor* tempSensor)
{
    this->temperatureSensor = tempSensor;
}

double Aquaduino::getTemperature()
{
    return temp;
}

void Aquaduino::setLevelSensor(Sensor* levSensor)
{
    this->levelSensor = levSensor;
}

double Aquaduino::getLevel()
{
    return level;
}

void Aquaduino::setWebserver(WebServer* webServer)
{
    myWebServer = webServer;
    webServer->setDefaultCommand(&defaultCmd);
    webServer->setUrlPathCommand(&controllerDispatchCommand);
    webServer->begin();
}

WebServer* Aquaduino::getWebserver()
{
    return myWebServer;
}

void Aquaduino::setTemplateParser(TemplateParser* parser)
{
    m_TemplateParser = parser;
}

TemplateParser* Aquaduino::getTemplateParser()
{
    return m_TemplateParser;
}

void Aquaduino::run()
{
    int8_t controllerIdx = -1;
    Controller* currentController;

    temp = temperatureSensor->read();
    level = levelSensor->read() > 0 ? 1 : 0;

    m_Controllers.resetIterator();
    while ((controllerIdx = m_Controllers.getNext(&currentController)) != -1)
    {
        currentController->run();
    }

    if (myWebServer != NULL)
    {
        myWebServer->processConnection();
    }
}

/*
 * ============================================================================
 *
 * Aquaduino specific declarations and definitions
 *
 */

extern time_t NTPSync();

/*
 * My system has 24 Power Outlets controlled by the Pins 14-37,
 * a Level sensor @ pin 40 and a DS18S20 @ pin 42.
 */
const static uint8_t POWER_OUTLETS = 24;
const static uint8_t POWER_OUTLET_START_PIN = 14;
const static uint8_t LEVEL_SENSOR_PIN = 38;
const static uint8_t TEMPERATURE_SENSOR_PIN = 39;

/*
 * Controller definitions:
 *
 * Configuration Controller is responsible for assigning the
 * available Actors to the different controllers in the system
 *
 * The other controllers are up to the developer. Feel free to
 * insert your own controllers here.
 *
 */
TemperatureController* temperatureController;
LevelController* levelController;
ClockTimerController* clockTimerController;

/*
 * Actor definitions. For me up to now 24 simple power outlets
 * controlled by a digital HIGH LOW is enough to drive the connected
 * relays. You can extend the Aquaduino Actor class to implement your
 * own actors. These actors can be assigned to the differen controllers
 * using the webinterface of the Configuration controller
 */
DigitalOutput* powerOutlets[POWER_OUTLETS];

/*
 * Sensor definitions
 */
Sensor* levelSensor;
Sensor* temperatureSensor;

/*
 * Pointer to the Webserver offering the webinterface. Webduino is used
 * with a small modification. See dispatchCommand method in Webserver.h.
 */
WebServer* webServer;

/*
 * ============================================================================
 *
 * Other declarations and definitions
 *
 */
extern int __data_start;
extern int __bss_start;

#ifdef __cplusplus
extern "C"
{
#endif
void loop();
void setup();
#ifdef __cplusplus
}
#endif

void setup()
{
    Serial.begin(9600);

    aquaduino = new Aquaduino();
    aquaduino->readConfig(aquaduino);

    //Init Time. If NTP Sync fails this will be used.
    setTime(0, 0, 0, 1, 1, 42);
    setSyncInterval(900); //Sync every 15 minutes
    setSyncProvider(&NTPSync);

    webServer = new WebServer("", 80);

    aquaduino->setWebserver(webServer);

    for (int i = 0; i < POWER_OUTLETS; i++)
    {
        char name[6] = "PO";
        itoa(i, &name[2], 10);

        powerOutlets[i] = new DigitalOutput(name,
                                            POWER_OUTLET_START_PIN + i,
                                            0);
        aquaduino->addActuator(powerOutlets[i]);
        aquaduino->readConfig(powerOutlets[i]);
    }

    temperatureController = new TemperatureController("Temperature");
    temperatureController->setURL("0");
    levelController = new LevelController("Level", LEVEL_SENSOR_PIN);
    levelController->setURL("1");
    clockTimerController = new ClockTimerController("Clock Timer");
    clockTimerController->setURL("2");

    levelSensor = new DigitalInput(LEVEL_SENSOR_PIN);
    temperatureSensor = new DS18S20(TEMPERATURE_SENSOR_PIN);

    aquaduino->addController(temperatureController);
    aquaduino->addController(levelController);
    aquaduino->addController(clockTimerController);

    aquaduino->readConfig(temperatureController);
    aquaduino->readConfig(levelController);

    aquaduino->setTemperatureSensor(temperatureSensor);
    aquaduino->setLevelSensor(levelSensor);

#ifdef DEBUG
    Serial.print(F("Data Start: 0x"));
    Serial.println((int) &__data_start, HEX);
    Serial.print(F("BSS Start: 0x"));
    Serial.println((int) &__bss_start, HEX);
    Serial.print(F("Heap Start: 0x"));
    Serial.println((int) __malloc_heap_start, HEX);

    Serial.print(F("Heap End: 0x"));
    Serial.println(((uint16_t) temperatureSensor) + sizeof(temperatureSensor)
                   + 1,
                   HEX);
#endif

}

void loop()
{
    aquaduino->run();
}
