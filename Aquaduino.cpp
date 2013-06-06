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

/**
 * \mainpage Aquaduino
 *
 * Aquaduino
 * =========
 *
 * What is Aquaduino?
 * ------------------
 *
 * Aquaduino is an extensible open source control system framework for fish tanks
 * or other related environments. Aquaduino is published under the GPLv3 and is
 * written in C++. It is currently developed on a DFRduino Mega 2560 with DFRduino
 * Ethernet Shield. It currently supports the control of digital outputs to
 * control my relay modules, temperature readings by using a Dallas DS18S20,
 * automatic refill by using a level sensor with closing contact and a simple
 * webinterface to manage the control of the power outlets. The strength of
 * Aquaduino is its extensibility which allows developers to simply support
 * further sensors, actors or controllers related to their special needs.
 *
 * The framework provided by Aquaduino uses Webduino. To provide a seamless
 * integration of actuators like relays into controlling elements the Aquaduino
 * WebInterface automatically generates a configuration WebInterface to assign
 * the available actuators to the controlling elements of Aquaduino. Thus the
 * actuators can easily be enabled, disabled or assigned at runtime to the
 * different control elements.
 *
 * Features
 * --------
 *
 * * Network and WebInterface
 *     * Static or DHCP IP configuration
 *     * NTP synchronization
 *     * Configuration of actuators and controllers
 *
 * * Actuator / Controller / Sensor Framework
 *     * Controllers are software components that control assigned actuators
 *       like power outlets,...
 *     * Actuators can dynamically be assigned to controllers
 *     * Sensor readings are triggered by the framework and globally available
 *       in the whole framework
 *     * Controllers and actuators provide their own WebInterface
 *     * Easily extensible to support different controllers and actuators
 *     * Currently supported Controllers:
 *         * Temperature controller (TemperatureController)
 *             * Threshold based
 *             * Definition of temperature where PWM based actor shall reach
 *               its maximum (linear scaling)
 *             * Hysteresis (fixed to 0.3 degree celsius)
 *         * Level controller (LevelController)
 *             * Time based debounce mechanism to ignore waves
 *             * Time out mechanism to prevent refill pump from running dry
 *             * Hysteresis (Configurable by webinterface)
 *         * Clocktimer controller (ClockTimerController)
 *             * Configurable amount of clocktimers
 *             * Configurable amount of time intervals per clocktimer
 *     * Currently supported Actuators
 *         * Digital outputs
 *     * Currently supported Sensors
 *         * Level sensor (DigitalInput)
 *         * Temperature sensor (DS18S20)
 *         * pH/ORP/EC in progress
 *
 * * Template Parser Framework
 *     * Easy integration of HTML templates in controller webinterface code
 *     * Completely stack based (No dynamic memory consumption)
 *     * Templates stored on SD Card (No static memory consumption)
 *
 * * Configuration Management
 *     * Support for SD Card storage
 *
 * Hardware Setup
 * --------------
 *
 * Aquaduino runs on the Arduino Mega 2560 and requires an Ethernetshield.
 * For more information see https://sourceforge.net/p/aquaduino/home/Home/
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

extern time_t NTPSync();

extern void defaultCmd(WebServer &server, WebServer::ConnectionType type,
                       char *, bool);
extern void controllerDispatchCommand(WebServer &server,
                                      WebServer::ConnectionType type,
                                      char **url_path, char *url_tail,
                                      bool tail_complete);

/**
 * \brief Default Constructor
 *
 * Initializes Aquaduino with default values and then tries to read the
 * configuration using the SDConfigManager. When there are multiple
 * implementations of ConfigManager available this is the place to exchange
 * them. Finally the network is brought up.
 */
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
        m_Actuators(MAX_ACTUATORS),
        temperatureSensor(NULL),
        levelSensor(NULL),
        myWebServer(NULL),
        m_TemplateParser(NULL),
        temp(0),
        level(0)
{
    int8_t status = 0;
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

    m_ConfigManager = new SDConfigManager("config");

    myMAC[0] = 0xDE;
    myMAC[1] = 0xAD;
    myMAC[2] = 0xBE;
    myMAC[3] = 0xEF;
    myMAC[4] = 0xDE;
    myMAC[5] = 0xAD;

    readConfig(this);

    if (doDHCP)
    {
        Serial.println(F("Waiting for DHCP reply..."));
        status = Ethernet.begin(myMAC);
    }
    if (!doDHCP || !status)
    {
        Serial.println(F("Using static network configuration..."));
        Ethernet.begin(myMAC, myIP, myDNS, myGateway, myNetmask);
    }

    myIP = Ethernet.localIP();
    myDNS = Ethernet.dnsServerIP();
    myGateway = Ethernet.gatewayIP();
    myNetmask = Ethernet.subnetMask();

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

    //Init Time. If NTP Sync fails this will be used.
    setTime(0, 0, 0, 1, 1, 42);
}

/**
 * \brief Setter for MAC address.
 * \param[in] mac pointer to MAC address.
 *
 * This method only stores the value in the object. It does not
 * configure the network.
 */
void Aquaduino::setMAC(uint8_t* mac)
{
    for (int i = 0; i < 6; i++)
        myMAC[i] = mac[i];
}

/**
 * \brief Getter for MAC address.
 * \param mac buffer to store the mac. Needs to be at least 6 bytes!
 *
 * \returns Configured MAC address. May be different to active MAC!
 */
void Aquaduino::getMAC(uint8_t* mac)
{
    for (int i = 0; i < 6; i++)
        mac[i] = myMAC[i];
}

/**
 * \brief Getter for IP address.
 *
 * \returns Configured IP address. May be different to active IP!
 */
IPAddress* Aquaduino::getIP()
{
    return &myIP;
}

/**
 * \brief Setter for IP address.
 * \param[in] ip pointer to IP address.
 *
 * This method only stores the value in the object. It does not
 * configure the network.
 */

void Aquaduino::setIP(IPAddress* ip)
{
    myIP = *ip;
}

/**
 * \brief Getter for netmask.
 *
 * \returns Configured netmask. May be different to active netmask!
 */

IPAddress* Aquaduino::getNetmask()
{
    return &myNetmask;
}

/**
 * \brief Setter for netmask.
 * \param[in] netmask pointer to netmask address.
 *
 * \brief This method only stores the value in the object. It does not
 * configure the network.
 */
void Aquaduino::setNetmask(IPAddress* netmask)
{
    myNetmask = *netmask;
}

/**
 * \brief Getter for gateway address.
 *
 * \returns Configured gateway address. May be different to active gateway
 * address!
 */

IPAddress* Aquaduino::getGateway()
{
    return &myGateway;
}

/**
 * \brief Setter for gateway address.
 * \param[in] gateway pointer to gateway address.
 *
 * This method only stores the value in the object. It does not
 * configure the network.
 */

void Aquaduino::setGateway(IPAddress* gateway)
{
    myGateway = *gateway;
}

/**
 * \brief Getter for DNS server address.
 *
 * \returns Configured DNS server address. May be different to active DNS
 * server address!
 */
IPAddress* Aquaduino::getDNS()
{
    return &myDNS;
}

/**
 * \brief Setter for DNS address
 * \param[in] dns pointer to DNS server address.
 *
 * This method only stores the value in the object. It does not
 * configure the network.
 */
void Aquaduino::setDNS(IPAddress* dns)
{
    myDNS = *dns;
}

/**
 * \brief Getter for NTP address.
 *
 * \returns Configured NTP server address.
 */
IPAddress* Aquaduino::getNTP()
{
    return &myNTP;
}

/**
 * \brief Setter for NTP address.
 * \param[in] ntp pointer to NTP addresss
 *
 * This method only stores the value in the object. It does not
 * trigger a NTP update.
 */
void Aquaduino::setNTP(IPAddress* ntp)
{
    myNTP = *ntp;
}

/**
 * \brief Getter for NTP synchronization interval.
 *
 * \returns NTP synchronization interval in minutes.
 */
uint16_t Aquaduino::getNtpSyncInterval()
{
    return ntpSyncInterval;
}

/**
 * \brief Setter for NTP synchronization interval.
 * \param[in] syncInterval NTP synchronization interval in minutes
 *
 * This method only stores the value in the object. It does not
 * trigger a NTP update.
 */

void Aquaduino::setNtpSyncInterval(uint16_t syncInterval)
{
    ntpSyncInterval = syncInterval;
}

/**
 * \brief Enables DHCP flag.
 *
 * Enables the DHCP flag. When this flag is set during construction time
 * a DHCP request is performed.
 */

void Aquaduino::enableDHCP()
{
    doDHCP = 1;
}

/**
 * \brief Disables DHCP flag.
 *
 * Disables the DHCP flag. When this flag is not set during construction time
 * no DHCP request is performed. Instead the IP configuration read by the
 * configuration manager is used as static configuration. The configuration
 * manager stores the values set by setIP, setNetmask, setGateway and
 * setDNS when they are updated using the configuration WebInterface.
 */
void Aquaduino::disableDHCP()
{
    doDHCP = 0;
}

/**
 * \brief Checks whether DHCP is enabled or not.
 *
 * \returns Value of the DHCP flag.
 */
int8_t Aquaduino::isDHCPEnabled()
{
    return doDHCP;
}

/**
 * \brief Enables NTP synchronization.
 *
 * Enables the NTP flag and directly performs a NTP synchronization request.
 * The NTP synchronization interval is set to the value set by
 * setNtpSyncInterval.
 */
void Aquaduino::enableNTP()
{
    doNTP = 1;
    setSyncInterval(ntpSyncInterval * 60);
    setSyncProvider(&::NTPSync);
}

/**
 * \brief Disables NTP synchronization.
 *
 * Disables the NTP synchronization and leaves current time untouched.
 */
void Aquaduino::disableNTP()
{
    doNTP = 0;
    setSyncInterval(ntpSyncInterval * 60);
    setSyncProvider(NULL);
}

/**
 * \brief Checks whether NTP synchronization is enabled or not.
 *
 * \returns Value of the NTP flag.
 */
int8_t Aquaduino::isNTPEnabled()
{
    return doNTP;
}

/**
 * \brief Sets the current time.
 * \param[in] hour
 * \param[in] minute
 * \param[in] second
 * \param[in] day
 * \param[in] month
 * \param[in] year
 *
 * Sets the current time when NTP synchronization is disabled. Otherwise no
 * update will be performed.
 */
void Aquaduino::setTime(int8_t hour, int8_t minute, int8_t second, int8_t day,
                        int8_t month, int16_t year)
{
    if (!doNTP)
        ::setTime(hour, minute, second, day, month, year);
}

/**
 * \brief Adds a controller to Aquaduino.
 * \param[in] newController The controller to be added.
 *
 * Adds the controller specified by newController. The controllers are stored
 * in an ArrayList and can later be identified by their index in this
 * ArrayList. If the store operation was successful the controllers URL
 * is set to "C" followed by its index. Thus a controller stored at index 1
 * will receive the URL "C1". After the URL was set the configuration manager
 * is triggered to read the configuration of the controller.
 *
 * \returns Index of the controller in the ArrayList m_Controllers. When the
 * operation fails -1 is returned.
 */
int8_t Aquaduino::addController(Controller* newController)
{
    char buffer[5];
    memset(buffer, 0, 5);
    int8_t idx = m_Controllers.add(newController);
#ifdef DEBUG
    Serial.print(F("Added controller "));
    Serial.print(newController->getName());
    Serial.print(F(" @ position "));
    Serial.println(idx);
#endif
    if (idx != -1)
    {
        buffer[0] = 'C';
        itoa(idx, &buffer[1], 10);
        m_Controllers[idx]->setURL(buffer);
        aquaduino->readConfig(newController);
    }
    return idx;
}

/**
 * \brief Getter for controllers assigned to Aquaduino.
 * \param[in] idx index location.
 *
 * \returns controller object stored at position idx. Can be NULL.
 */
Controller* Aquaduino::getController(unsigned int idx)
{
    return m_Controllers.get(idx);
}

/**
 * \brief Gets the index of a controller object.
 * \param[in] controller to be identified.
 *
 * \returns the index in m_Controllers if the object is stored in there. If
 * that is not the case -1 is returned.
 */
int8_t Aquaduino::getControllerID(Controller* controller)
{
    return m_Controllers.findElement(controller);
}

/**
 * \brief Resets the iterator for the controllers stored in m_Controllers.
 *
 * The iterator is placed to the first slot in m_Controllers.
 */
void Aquaduino::resetControllerIterator()
{
    m_Controllers.resetIterator();
}

/**
 * \brief Returns the next controller in m_Controllers.
 * \param[out] controller stores the pointer to the next controller in here.
 *
 * Since the ArrayList m_Controllers may get fragmented the ArrayList
 * provides the functionality to iterate over all available elements
 * in the ArrayList. This method delegates the call to the method of the
 * ArrayList.
 */
int8_t Aquaduino::getNextController(Controller** controller)
{
    return m_Controllers.getNext(controller);
}

/**
 * \brief Getter for the number of assigned controllers.
 *
 * \returns the number of assigned controllers.
 */
unsigned char Aquaduino::getNrOfControllers()
{
    return m_Controllers.getNrOfElements();
}

/**
 * \brief Adds an actuators to Aquaduino.
 * \param[in] newActuator pointer to the actuator object to be added.
 *
 * Adds the actuator specified by newActuator. The actuators are stored
 * in an ArrayList and can later be identified by their index in this
 * ArrayList. If the store operation was successful the actuators URL
 * is set to "A" followed by its index. Thus an actuator stored at index 1
 * will receive the URL "A1". After the URL was set the configuration manager
 * is triggered to read the configuration of the actuator.
 *
 * \returns Index of the actuator in the ArrayList m_Actuators. When the
 * operation fails -1 is returned.
 */
int8_t Aquaduino::addActuator(Actuator* newActuator)
{
    char buffer[5];
    memset(buffer, 0, 5);
    int8_t idx = m_Actuators.add(newActuator);
#ifdef DEBUG
    Serial.print(F("Added actuator "));
    Serial.print(newActuator->getName());
    Serial.print(F(" @ position "));
    Serial.println(idx);
#endif
    if (idx != -1)
    {
        buffer[0] = 'A';
        itoa(idx, &buffer[1], 10);
        newActuator->setURL(buffer);
        readConfig(newActuator);
    }
    return idx;
}

/**
 * \brief Getter for actuators assigned to Aquaduino.
 * \param[in] idx index within the ArrayList m_Actuators.
 *
 * \returns actuator object stored at position idx. Can be NULL.
 */
Actuator* Aquaduino::getActuator(unsigned int idx)
{
    return m_Actuators.get(idx);
}

/**
 * \brief Gets the index of an actuator object.
 * \param[in] actuator pointer to the actuator object to be found.
 *
 * \returns the index in m_Actuators if the object is stored in there. If
 * that is not the case -1 is returned.
 */
int8_t Aquaduino::getActuatorID(Actuator* actuator)
{
    return m_Actuators.findElement(actuator);
}

/**
 * \brief Resets the iterator for the actuators stored in m_Actuators.
 *
 * The iterator is placed to the first slot in m_Actuators.
 */
void Aquaduino::resetActuatorIterator()
{
    m_Actuators.resetIterator();
}

/**
 * \brief Returns the next actuator in m_Actuators.
 * \param[out] actuator the pointer to the actuator is stored in here.
 *
 * Since the ArrayList m_Actuators may get fragmented the ArrayList
 * provides the functionality to iterate over all available elements
 * in the ArrayList. This method delegates the call to the method of the
 * ArrayList.
 *
 * \returns the index of the next actuator.
 */
int8_t Aquaduino::getNextActuator(Actuator** actuator)
{
    return m_Actuators.getNext(actuator);
}

/**
 * \brief Identifies the actuators assigned to a specific controller.
 *
 * This method iterates over all actuators and checks which actuators
 * are assigned to the controller specified by controller. The resulting
 * objects are stored in the passed array of actuator pointers with size max.
 *
 * returns the number of assigned actuators.
 */
int8_t Aquaduino::getAssignedActuators(Controller* controller,
                                       Actuator** actuators, int8_t max)
{
    int8_t actuatorIdx = -1;
    int8_t nrOfAssignedActuators = 0;
    Actuator* currentActuator;
    int8_t controllerIdx = m_Controllers.findElement(controller);

    m_Actuators.resetIterator();
    while ((actuatorIdx = m_Actuators.getNext(&currentActuator)) != -1)
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

/**
 * \brief Identifies the actuators assigned to a specific controller.
 * @param[in] controller The controller for which the assigned actuators shall
 *                       be identified.
 * @param[out] actuatorIDs Array to store the identified actuators.
 * @param[in] max size of the array.
 *
 * This method iterates over all actuators and checks which actuators
 * are assigned to the specified controller. The resulting
 * indices are stored in the passed array of indices with size max.
 *
 * returns the number of assigned actuators.
 */
int8_t Aquaduino::getAssignedActuatorIDs(Controller* controller,
                                         int8_t* actuatorIDs, int8_t max)
{
    int8_t actuatorIdx = -1;
    int8_t nrOfAssignedActuators = 0;
    Actuator* currentActuator;
    int8_t controllerIdx = m_Controllers.findElement(controller);

    m_Actuators.resetIterator();
    while ((actuatorIdx = m_Actuators.getNext(&currentActuator)) != -1)
    {
        if (currentActuator->getController() == controllerIdx)
        {
            if (nrOfAssignedActuators < max)
                actuatorIDs[nrOfAssignedActuators] = actuatorIdx;
            nrOfAssignedActuators++;
        }
    }
    return nrOfAssignedActuators;
}

/**
 * \brief Getter for the number of assigned actuators.
 *
 * \returns the number of assigned actuators.
 */
unsigned char Aquaduino::getNrOfActuators()
{
    return m_Actuators.getNrOfElements();
}

/**
 * \brief Serializes the Aquaduino configuration
 * \param[out] buffer pointer to the buffer where the serialized data is going
 *                    to be stored.
 * \param[in] size Size of the buffer.
 *
 * \implements Serializable
 *
 * \returns amount of data serialized in bytes. Returns 0 if serialization
 * failed.
 */
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

/**
 * \brief Deserializes the Aquaduino configuration
 * \param[in] data pointer to the data where the serialized data is stored.
 * \param[in] size Size of the buffer.
 *
 * \implements Serializable
 *
 * \returns amount of data deserialized in bytes. Returns 0 if deserialization
 * failed.
 */
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

/**
 * \brief Write Aquaduino configuration
 * \param[in] aquaduino The aquaduino instance of which the configuration
 *                      shall be written.
 *
 * Delegates the call to the ConfigurationManager to write the configuration.
 *
 * \returns The number of written bytes. -1 if writing failed.
 */
int8_t Aquaduino::writeConfig(Aquaduino* aquaduino)
{
    return m_ConfigManager->writeConfig(aquaduino);
}

/**
 * \brief Write Actuator configuration
 * \param[in] actuator The actuator instance of which the configuration
 *                     shall be written.
 *
 * Delegates the call to the ConfigurationManager to write the configuration.
 *
 * \returns The number of written bytes. -1 if writing failed.
 */
int8_t Aquaduino::writeConfig(Actuator* actuator)
{
    return m_ConfigManager->writeConfig(actuator);
}

/**
 * \brief Write Controller configuration
 * \param[in] controller The controller instance of which the configuration
 *                       shall be written.
 *
 * Delegates the call to the ConfigurationManager to write the configuration.
 *
 * \returns The number of written bytes. -1 if writing failed.
 */
int8_t Aquaduino::writeConfig(Controller* controller)
{
    return m_ConfigManager->writeConfig(controller);
}

/**
 * \brief Write Sensor configuration
 * \param[in] sensor The sensor instance of which the configuration
 *                   shall be written.
 *
 * Delegates the call to the ConfigurationManager to write the configuration.
 *
 * \returns The number of written bytes. -1 if writing failed.
 */
int8_t Aquaduino::writeConfig(Sensor* sensor)
{
    return m_ConfigManager->writeConfig(sensor);
}

/**
 * \brief Reads the Aquaduino configuration
 * \param[in] aquaduino The aquaduino instance of which the configuration
 *                     shall be read.
 *
 * \returns amount of data read in bytes. -1 if reading failed.
 */
int8_t Aquaduino::readConfig(Aquaduino* aquaduino)
{
    return m_ConfigManager->readConfig(aquaduino);
}

/**
 * \brief Reads Actuator configuration
 * \param[in] actuator The actuator instance of which the configuration
 *                     shall be read.
 *
 * Delegates the call to the ConfigurationManager to read the configuration.
 *
 * \returns The number of read bytes. -1 if reading failed.
 */
int8_t Aquaduino::readConfig(Actuator* actuator)
{
    return m_ConfigManager->readConfig(actuator);
}

/**
 * \brief Reads Controller configuration
 * \param[in] controller The controller instance of which the configuration
 *                       shall be read.
 *
 * Delegates the call to the ConfigurationManager to read the configuration.
 *
 * \returns The number of read bytes. -1 if reading failed.
 */
int8_t Aquaduino::readConfig(Controller* controller)
{
    return m_ConfigManager->readConfig(controller);
}

/**
 * \brief Reads Sensor configuration
 * \param[in] sensor The sensor instance of which the configuration
 *                   shall be read.
 *
 * Delegates the call to the ConfigurationManager to read the configuration.
 *
 * \returns The number of read bytes. -1 if reading failed.
 */
int8_t Aquaduino::readConfig(Sensor* sensor)
{
    return m_ConfigManager->readConfig(sensor);
}

/**
 * \brief Setter for the temperature sensor.
 * \param[in] tempSensor instance of the temperature sensor
 *
 * Sets the temperature sensor to be used.
 */
void Aquaduino::setTemperatureSensor(Sensor* tempSensor)
{
    this->temperatureSensor = tempSensor;
}

/**
 * \brief Getter for the temperature.
 *
 * Accesses the member variable. No read on the sensor is triggered.
 *
 * \returns The last read temperature.
 *
 */
double Aquaduino::getTemperature()
{
    return temp;
}

/**
 * \brief Setter for the level sensor.
 * \param[in] levSensor instance of the temperature sensor
 *
 * Sets the level sensor to be used.
 */
void Aquaduino::setLevelSensor(Sensor* levSensor)
{
    this->levelSensor = levSensor;
}

/**
 * \brief Getter for the water level.
 *
 * Accesses the member variable. No read on the sensor is triggered.
 *
 * \returns The last read water level.
 *
 */
double Aquaduino::getLevel()
{
    return level;
}

/**
 * \brief Setter for the Webduino webserver instance.
 * \param[in] webServer WebServer instance to be used.
 *
 * Sets athe WebServer instance to be used and attaches the prepared handler
 * routines. Finally the WebServer is initialized.
 */
void Aquaduino::setWebserver(WebServer* webServer)
{
    myWebServer = webServer;
    //
    webServer->setDefaultCommand(&defaultCmd);
    webServer->setUrlPathCommand(&controllerDispatchCommand);
    webServer->begin();
}

/**
 * \brief Getter for the WebServer
 *
 * \returns The instance of the used WebServer
 */
WebServer* Aquaduino::getWebserver()
{
    return myWebServer;
}

/**
 * \brief Setter for the TemplateParser to be used for HTML template parsing.
 * \param[in] parser Instance of the used TemplateParser.
 *
 * Sets the instance of the used TemplateParser
 *
 * \returns The instance of the used WebServer
 */
void Aquaduino::setTemplateParser(TemplateParser* parser)
{
    m_TemplateParser = parser;
}

/**
 * \brief Getter for the TemplateParser
 *
 * \returns The instance of the used TemplateParser
 */
TemplateParser* Aquaduino::getTemplateParser()
{
    return m_TemplateParser;
}

/**
 * \brief Top level run method.
 *
 * This is the top level run method. It triggers the sensor readings,
 * controller run methods and the WebServer processing. Needs to be called
 * periodically i.e. within the loop() function of the Arduino environment.
 */
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

/*
 * My system has 24 Power Outlets controlled by the Pins 14-37,
 * a Level sensor @ pin 40 and a DS18S20 @ pin 42.
 */
const static uint8_t POWER_OUTLETS = 8;
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

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup()
{
    Serial.begin(9600);
    aquaduino = new Aquaduino();

    if (aquaduino->isNTPEnabled())
    {
        Serial.println(F("Syncing time using NTP..."));
        aquaduino->enableNTP();
    }

    webServer = new WebServer("", 80);
    aquaduino->setWebserver(webServer);

    for (int i = 0; i < POWER_OUTLETS; i++)
    {
        char name[6] = "PO";
        itoa(i, &name[2], 10);
        powerOutlets[i] = new DigitalOutput(name,
                                            POWER_OUTLET_START_PIN + i,
                                            HIGH,
                                            LOW);
        powerOutlets[i]->on();
        aquaduino->addActuator(powerOutlets[i]);
    }

    temperatureController = new TemperatureController("Temperature");
    levelController = new LevelController("Level", LEVEL_SENSOR_PIN);
    clockTimerController = new ClockTimerController("Clock Timer");

    levelSensor = new DigitalInput(LEVEL_SENSOR_PIN);
    temperatureSensor = new DS18S20(TEMPERATURE_SENSOR_PIN);

    aquaduino->addController(temperatureController);
    aquaduino->addController(levelController);
    aquaduino->addController(clockTimerController);

    aquaduino->setTemperatureSensor(temperatureSensor);
    aquaduino->setLevelSensor(levelSensor);

#ifdef DEBUG
    Serial.print(F("Data Start: 0x"));
    Serial.println((int) &__data_start, HEX);
    Serial.print(F("BSS Start: 0x"));
    Serial.println((int) &__bss_start, HEX);
    Serial.print(F("Heap Start: 0x"));
    Serial.println((int) __malloc_heap_start, HEX);

    Serial.print(F("Last allocated Element ends: 0x"));
    Serial.println(((uint16_t) temperatureSensor) + sizeof(temperatureSensor)
                   + 1,
                   HEX);

    Serial.print(F("Free Memory:"));
    Serial.println(freeRam());
#endif

}

void loop()
{
    aquaduino->run();
}
