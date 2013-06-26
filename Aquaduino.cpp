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
#include <SD.h>
#include <TemplateParser.h>
#include <Time.h>
#include <EthernetUdp.h>
#include <WebServer.h>
#include <stdlib.h>
#include <Framework/Flashvars.h>

Aquaduino* aquaduino;

extern time_t NTPSync();

/*
 * My system has 24 Power Outlets controlled by the Pins 14-37,
 * a Level sensor @ pin 40 and a DS18S20 @ pin 42.
 */
const static uint8_t POWER_OUTLETS = 24;
const static uint8_t POWER_OUTLET_START_PIN = 14;
const static uint8_t LEVEL_SENSOR_PIN = 38;
const static uint8_t TEMPERATURE_SENSOR_PIN = 39;

/**
 * \brief Default Constructor
 *
 * Initializes Aquaduino with default values and then tries to read the
 * configuration using the SDConfigManager. When there are multiple
 * implementations of ConfigManager available this is the place to exchange
 * them. Finally the network is brought up.
 */
Aquaduino::Aquaduino() :
        m_IP(192, 168, 1, 222),
        m_Netmask(255, 255, 255, 0),
        m_DNSServer(192, 168, 1, 1),
        m_Gateway(192, 168, 1, 1),
        m_NTPServer(192, 53, 103, 108),
        m_Timezone(TIME_ZONE),
        m_NTPSyncInterval(5),
        m_DHCP(0),
        m_NTP(0),
        m_Controllers(MAX_CONTROLLERS),
        m_Actuators(MAX_ACTUATORS),
        m_Sensors(MAX_SENSORS),
        m_WebServer(NULL),
        m_TemplateParser(NULL)
{
    aquaduino = this;

    int8_t status = 0;
    m_Type = AQUADUINO;
    // Deselect all SPI devices!
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    int8_t actuatorConfig[MAX_ACTUATORS] = ACTUATOR_CONFIG;
    int8_t controllerConfig[MAX_CONTROLLERS] = CONTROLLER_CONFIG;
    int8_t sensorConfig[MAX_SENSORS] = SENSOR_CONFIG;

    if (!SD.begin(4))
    {
        Serial.println(F("No SD Card available"));
        while(1);
    }

    m_ConfigManager = new SDConfigManager("config");

    m_MAC[0] = 0xDE;
    m_MAC[1] = 0xAD;
    m_MAC[2] = 0xBE;
    m_MAC[3] = 0xEF;
    m_MAC[4] = 0xDE;
    m_MAC[5] = 0xAD;

    readConfig(this);

    if (m_DHCP)
    {
        Serial.println(F("Waiting for DHCP reply..."));
        status = Ethernet.begin(m_MAC);
    }
    if (!m_DHCP || !status)
    {
        Serial.println(F("Using static network configuration..."));
        Ethernet.begin(m_MAC, m_IP, m_DNSServer, m_Gateway, m_Netmask);
    }

    m_IP = Ethernet.localIP();
    m_DNSServer = Ethernet.dnsServerIP();
    m_Gateway = Ethernet.gatewayIP();
    m_Netmask = Ethernet.subnetMask();

    Serial.print(F("IP: "));
    Serial.println(m_IP);
    Serial.print(F("Netmask: "));
    Serial.println(m_Netmask);
    Serial.print(F("Gateway: "));
    Serial.println(m_Gateway);
    Serial.print(F("DNS Server: "));
    Serial.println(m_DNSServer);
    Serial.print(F("NTP Server: "));
    Serial.println(m_NTPServer);

    //Init Time. If NTP Sync fails this will be used.
    setTime(0, 0, 0, 1, 1, 42);

    if (isNTPEnabled())
    {
        Serial.println(F("Syncing time using NTP..."));
        enableNTP();
    }

    Serial.println(F("Starting Webserver..."));
    setWebserver(new WebServer("", 80));

    Serial.println(F("Initializing actuators..."));
    for (int i = 0; i < MAX_ACTUATORS; i++)
    {
        switch (actuatorConfig[i])
        {
        case ACTUATOR_DIGITALOUTPUT:
            addActuator(new DigitalOutput(NULL,
                                          POWER_OUTLET_START_PIN + i,
                                          HIGH,
                                          LOW));
            break;
        case ACTUATOR_PWMOUTPUT:
            break;
        }
    }

    Serial.println(F("Initializing controllers..."));
    for (int i = 0; i < MAX_CONTROLLERS; i++)
    {
        switch (controllerConfig[i])
        {
        case CONTROLLER_LEVEL:
            addController(new LevelController("Level"));
            break;
        case CONTROLLER_TEMPERATURE:
            addController(new TemperatureController("Temperature"));
            break;
        case CONTROLLER_CLOCKTIMER:
            addController(new ClockTimerController("Clock Timer"));
            break;
        }
    }

    Serial.println(F("Initializing sensors..."));
    for (int i = 0; i < MAX_SENSORS; i++)
    {
        switch (sensorConfig[i])
        {
        case SENSOR_DIGITALINPUT:
            addSensor(new DigitalInput());
            m_Sensors[i]->setName("Level");
            break;
        case SENSOR_DS18S20:
            addSensor(new DS18S20());
            m_Sensors[i]->setName("DS18S20");
            break;
        }
    }
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
        m_MAC[i] = mac[i];
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
        mac[i] = m_MAC[i];
}

/**
 * \brief Getter for IP address.
 *
 * \returns Configured IP address. May be different to active IP!
 */
IPAddress* Aquaduino::getIP()
{
    return &m_IP;
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
    m_IP = *ip;
}

/**
 * \brief Getter for netmask.
 *
 * \returns Configured netmask. May be different to active netmask!
 */

IPAddress* Aquaduino::getNetmask()
{
    return &m_Netmask;
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
    m_Netmask = *netmask;
}

/**
 * \brief Getter for gateway address.
 *
 * \returns Configured gateway address. May be different to active gateway
 * address!
 */

IPAddress* Aquaduino::getGateway()
{
    return &m_Gateway;
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
    m_Gateway = *gateway;
}

/**
 * \brief Getter for DNS server address.
 *
 * \returns Configured DNS server address. May be different to active DNS
 * server address!
 */
IPAddress* Aquaduino::getDNS()
{
    return &m_DNSServer;
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
    m_DNSServer = *dns;
}

/**
 * \brief Getter for NTP address.
 *
 * \returns Configured NTP server address.
 */
IPAddress* Aquaduino::getNTP()
{
    return &m_NTPServer;
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
    m_NTPServer = *ntp;
}

/**
 * \brief Getter for NTP synchronization interval.
 *
 * \returns NTP synchronization interval in minutes.
 */
uint16_t Aquaduino::getNtpSyncInterval()
{
    return m_NTPSyncInterval;
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
    m_NTPSyncInterval = syncInterval;
}

void Aquaduino::setTimezone(int8_t zone)
{
    this->m_Timezone = zone;
}

int8_t Aquaduino::getTimezone()
{
    return this->m_Timezone;
}

/**
 * \brief Enables DHCP flag.
 *
 * Enables the DHCP flag. When this flag is set during construction time
 * a DHCP request is performed.
 */

void Aquaduino::enableDHCP()
{
    m_DHCP = 1;
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
    m_DHCP = 0;
}

/**
 * \brief Checks whether DHCP is enabled or not.
 *
 * \returns Value of the DHCP flag.
 */
int8_t Aquaduino::isDHCPEnabled()
{
    return m_DHCP;
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
    m_NTP = 1;
    setSyncInterval(m_NTPSyncInterval * 60);
    setSyncProvider(&::NTPSync);
}

/**
 * \brief Disables NTP synchronization.
 *
 * Disables the NTP synchronization and leaves current time untouched.
 */
void Aquaduino::disableNTP()
{
    m_NTP = 0;
    setSyncInterval(m_NTPSyncInterval * 60);
    setSyncProvider(NULL);
}

/**
 * \brief Checks whether NTP synchronization is enabled or not.
 *
 * \returns Value of the NTP flag.
 */
int8_t Aquaduino::isNTPEnabled()
{
    return m_NTP;
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
    if (!m_NTP)
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
    char buffer[5] =
        { 0 };

    int8_t idx = m_Controllers.add(newController);
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
    char buffer[5] =
        { 0 };

    int8_t idx = m_Actuators.add(newActuator);
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

int8_t Aquaduino::addSensor(Sensor* newSensor)
{
    char buffer[5] =
        { 0 };

    int8_t idx = m_Sensors.add(newSensor);
    if (idx != -1)
    {
        buffer[0] = 'S';
        itoa(idx, &buffer[1], 10);
        newSensor->setURL(buffer);
        readConfig(newSensor);
    }
    return idx;
}

Sensor* Aquaduino::getSensor(unsigned int sensor)
{
    return m_Sensors[sensor];
}

int8_t Aquaduino::getSensorID(Sensor* sensor)
{
    return m_Sensors.findElement(sensor);
}

void Aquaduino::resetSensorIterator()
{
    m_Sensors.resetIterator();
}

int8_t Aquaduino::getNextSensor(Sensor** sensor)
{
    return m_Sensors.getNext(sensor);
}

unsigned char Aquaduino::getNrOfSensors()
{
    return m_Sensors.getNrOfElements();
}

double Aquaduino::getSensorValue(int8_t idx)
{
    if (idx >= 0 && idx < MAX_SENSORS)
        return m_SensorReadings[idx];
    return 0;
}

/*
 * ============================================================================
 */

const uint16_t Aquaduino::m_Size = sizeof(m_MAC) + sizeof(uint32_t)
                                   + sizeof(uint32_t) + sizeof(uint32_t)
                                   + sizeof(uint32_t) + sizeof(uint32_t)
                                   + sizeof(m_NTPSyncInterval) + sizeof(m_DHCP)
                                   + sizeof(m_NTP) + sizeof(m_Timezone);

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

    if (size > m_Size || buffer == NULL)
        return 0;

    memcpy(bPtr, m_MAC, sizeof(m_MAC));
    bPtr += sizeof(m_MAC);
    memcpy(bPtr, &m_IP[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &m_Netmask[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &m_DNSServer[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &m_Gateway[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &m_NTPServer[0], sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(bPtr, &m_NTPSyncInterval, sizeof(m_NTPSyncInterval));
    bPtr += sizeof(m_NTPSyncInterval);
    memcpy(bPtr, &m_DHCP, sizeof(m_DHCP));
    bPtr += sizeof(m_DHCP);
    memcpy(bPtr, &m_NTP, sizeof(m_NTP));
    bPtr += sizeof(m_NTP);
    memcpy(bPtr, &m_Timezone, sizeof(m_Timezone));
    bPtr += sizeof(m_Timezone);

    return m_Size;
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

    if (m_Size > size || data == NULL)
        return 0;

    memcpy(m_MAC, bPtr, sizeof(m_MAC));
    bPtr += sizeof(m_MAC);
    memcpy(&m_IP[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&m_Netmask[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&m_DNSServer[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&m_Gateway[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&m_NTPServer[0], bPtr, sizeof(uint32_t));
    bPtr += sizeof(uint32_t);
    memcpy(&m_NTPSyncInterval, bPtr, sizeof(m_NTPSyncInterval));
    bPtr += sizeof(m_NTPSyncInterval);
    memcpy(&m_DHCP, bPtr, sizeof(m_DHCP));
    bPtr += sizeof(m_DHCP);
    memcpy(&m_NTP, bPtr, sizeof(m_NTP));
    bPtr += sizeof(m_NTP);
    memcpy(&m_Timezone, bPtr, sizeof(m_Timezone));
    bPtr += sizeof(m_Timezone);

    return m_Size;
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

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char * url,
                bool);
void dispatchCommand(WebServer &server, WebServer::ConnectionType type,
                     char **url_path, char *url_tail, bool tail_complete);

/**
 * \brief Setter for the Webduino webserver instance.
 * \param[in] webServer WebServer instance to be used.
 *
 * Sets the WebServer instance to be used and attaches the prepared handler
 * routines. Finally the WebServer is initialized.
 *
 * See ::defaultCmd for implementation of the main web page and
 * ::controllerDispatchCommand for the implementation of the request
 * dispatching to the registered controllers and actuators.
 *
 */
void Aquaduino::setWebserver(WebServer* webServer)
{
    m_WebServer = webServer;

    /**
     */
    webServer->setDefaultCommand(&defaultCmd);
    webServer->setUrlPathCommand(&dispatchCommand);
    webServer->begin();
}

/**
 * \brief Getter for the WebServer
 *
 * \returns The instance of the used WebServer
 */
WebServer* Aquaduino::getWebserver()
{
    return m_WebServer;
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
 * ----------------------------------------------------------------------------
 *
 */

/**
 * \brief Prints the main web page.
 *
 * Prints the main web page specified in pMFileName.
 */
void defaultCmd(WebServer &server, WebServer::ConnectionType type, char * url,
                bool)
{
    aquaduino->showWebinterface(&server, type, url);
}

/**
 * \brief This command is triggered when the request is not the default page.
 * \param [in] server Webserver instance to use.
 * \param[in] type Request type
 * \param[in] url_path URL of the request
 * \param[in] url_tail contains the part of the URL that wasn't matched against
 *            the registered command table.
 * \param[in] tail_complete is true if the complete URL fit in url_tail,  false if
 *            part of it was lost because the buffer was too small.
 *
 * This command dispatches the request based on its URL.
 *
 * For the Aquaduino configuration webpage the URL "config" is hardcoded here
 * and the configCmd is called when the url_path matches this string.
 * Actuator and Controller URLs are compared to url_path. When they match
 * the showWebinterface of the Actuator or Controller is called.
 *
 */
void dispatchCommand(WebServer &server, WebServer::ConnectionType type,
                     char **url_path, char *url_tail, bool tail_complete)
{
    Controller* controller;
    Actuator* actuator;
    Sensor* sensor;

    char* topLevelURL = NULL;
    char* subURL = NULL;
    uint8_t pos = 0;

    if (type != WebServer::HEAD)
    {
        //SubURL decoding.
        pos = strcspn(*url_path, URL_DELIMITER);
        if (pos != strlen(*url_path))
            subURL = &((*url_path)[pos + 1]);
        topLevelURL = strtok(*url_path, URL_DELIMITER);

        aquaduino->resetControllerIterator();
        while (aquaduino->getNextController(&controller) != -1)
        {
            if (strcmp(topLevelURL, controller->getURL()) == 0)
            {
                controller->showWebinterface(&server, type, subURL);
                if (type == WebServer::POST)
                    aquaduino->writeConfig(controller);
                return;
            }
        }

        aquaduino->resetSensorIterator();
        while (aquaduino->getNextSensor(&sensor) != -1)
        {
            if (strcmp(topLevelURL, sensor->getURL()) == 0)
            {
                sensor->showWebinterface(&server, type, subURL);
                if (type == WebServer::POST)
                    aquaduino->writeConfig(sensor);
                return;
            }
        }

        aquaduino->resetActuatorIterator();
        while (aquaduino->getNextActuator(&actuator) != -1)
        {
            if (strcmp(topLevelURL, actuator->getURL()) == 0)
            {
                actuator->showWebinterface(&server, type, subURL);
                if (type == WebServer::POST)
                    aquaduino->writeConfig(actuator);
                return;
            }
        }

        aquaduino->showWebinterface(&server, type, *url_path);
    }
}

int freeRam();

/**
 * \brief Prints the configuration webpage.
 *
 * Prints the configuation webpage using the template in
 * progConfigTemplateFileName.
 */
void Aquaduino::printConfigWebpage(WebServer* server)
{
    File templateFile;
    TemplateParser* parser;
    int16_t matchIdx = 0;
    uint8_t mac[6];
    IPAddress* ip, *netmask, *dns, *gw, *ntp;

    char templateFileName[sizeof(size_progConfigTemplateFileName)];
    strcpy_P(templateFileName, progConfigTemplateFileName);

    parser = aquaduino->getTemplateParser();
    templateFile = SD.open(templateFileName, FILE_READ);

    aquaduino->getMAC(mac);
    ip = aquaduino->getIP();
    netmask = aquaduino->getNetmask();
    gw = aquaduino->getGateway();
    dns = aquaduino->getDNS();
    ntp = aquaduino->getNTP();

    while ((matchIdx =
            parser->processTemplateUntilNextMatch(&templateFile,
                                                  configTemplateStrings,
                                                  nr_configTemplateStrings,
                                                  server))
           >= 0)
    {
        switch (matchIdx)
        {
        case T_ACTORROW:
            //printActuatorTable(server);
            break;
        case T_MAC1:
            server->print(mac[0], HEX);
            break;
        case T_MAC2:
            server->print(mac[1], HEX);
            break;
        case T_MAC3:
            server->print(mac[2], HEX);
            break;
        case T_MAC4:
            server->print(mac[3], HEX);
            break;
        case T_MAC5:
            server->print(mac[4], HEX);
            break;
        case T_MAC6:
            server->print(mac[5], HEX);
            break;
        case T_DHCPSELECTOPTION:
            if (aquaduino->isDHCPEnabled())
            {
                parser->selectListOption("Yes", "1", 1, server);
                parser->selectListOption("No", "0", 0, server);
            }
            else
            {
                parser->selectListOption("Yes", "1", 0, server);
                parser->selectListOption("No", "0", 1, server);
            }
            break;
        case T_IP1:
            server->print((*ip)[0]);
            break;
        case T_IP2:
            server->print((*ip)[1]);
            break;
        case T_IP3:
            server->print((*ip)[2]);
            break;
        case T_IP4:
            server->print((*ip)[3]);
            break;
        case T_NETMASK1:
            server->print((*netmask)[0]);
            break;
        case T_NETMASK2:
            server->print((*netmask)[1]);
            break;
        case T_NETMASK3:
            server->print((*netmask)[2]);
            break;
        case T_NETMASK4:
            server->print((*netmask)[3]);
            break;
        case T_GATEWAY1:
            server->print((*gw)[0]);
            break;
        case T_GATEWAY2:
            server->print((*gw)[1]);
            break;
        case T_GATEWAY3:
            server->print((*gw)[2]);
            break;
        case T_GATEWAY4:
            server->print((*gw)[3]);
            break;
        case T_DNS1:
            server->print((*dns)[0]);
            break;
        case T_DNS2:
            server->print((*dns)[1]);
            break;
        case T_DNS3:
            server->print((*dns)[2]);
            break;
        case T_DNS4:
            server->print((*dns)[3]);
            break;
        case T_NTPSELECTOPTION:
            if (aquaduino->isNTPEnabled())
            {
                parser->selectListOption("Yes", "1", 1, server);
                parser->selectListOption("No", "0", 0, server);
            }
            else
            {
                parser->selectListOption("Yes", "1", 0, server);
                parser->selectListOption("No", "0", 1, server);
            }
            break;
        case T_NTP1:
            server->print((*ntp)[0]);
            break;
        case T_NTP2:
            server->print((*ntp)[1]);
            break;
        case T_NTP3:
            server->print((*ntp)[2]);
            break;
        case T_NTP4:
            server->print((*ntp)[3]);
            break;
        case T_NTPPERIOD:
            server->print(aquaduino->getNtpSyncInterval());
            break;
        case T_TIMEZONE:
            server->print(aquaduino->getTimezone());
            break;
        case T_HOUR:
            server->print(hour());
            break;
        case T_MINUTE:
            server->print(minute());
            break;
        case T_SECOND:
            server->print(second());
            break;
        case T_FREERAM:
            server->print(freeRam());
            server->print(F(" Bytes"));
            break;
        }
    }
    templateFile.close();
}

/**
 * \brief This command is trigger upon request to the URL "/config"
 * \param[in] server Webserver instance to use
 * \param[in] type Request type
 *
 * Request is redirected from ::controllerDispatchCommand.
 */
int8_t Aquaduino::configWebpageProcessPost(WebServer* server,
                                           WebServer::ConnectionType type)
{
    int8_t repeat;
    uint8_t mac[6];
    char name[30], value[30];
    IPAddress ip, netmask, gw, dns, ntp;
    int8_t doNTP = 0, doDHCP = 0;
    uint16_t ntpperiod = 5;
    int8_t timezone = TIME_ZONE;
    int8_t hour = 0;
    int8_t minute = 0;
    int8_t second = 0;
    Actuator* actuator;

    /*
     * TODO: Implement security checks when processing POST parameters
     */
    if (type == WebServer::POST)
    {
        while ((repeat = server->readPOSTparam(name, 30, value, 30)) > 0)
        {
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_MAC1])))
                == 0)
                mac[0] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MAC2])))
                     == 0)
                mac[1] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MAC3])))
                     == 0)
                mac[2] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MAC4])))
                     == 0)
                mac[3] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MAC5])))
                     == 0)
                mac[4] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MAC6])))
                     == 0)
                mac[5] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP1])))
                     == 0)
                ip[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP2])))
                     == 0)
                ip[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP3])))
                     == 0)
                ip[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_IP4])))
                     == 0)
                ip[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK1])))
                == 0)
                netmask[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK2])))
                     == 0)
                netmask[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK3])))
                     == 0)
                netmask[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NETMASK4])))
                     == 0)
                netmask[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY1])))
                == 0)
                gw[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY2])))
                     == 0)
                gw[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY3])))
                     == 0)
                gw[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_GATEWAY4])))
                     == 0)
                gw[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_DNS1])))
                == 0)
                dns[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DNS2])))
                     == 0)
                dns[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DNS3])))
                     == 0)
                dns[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DNS4])))
                     == 0)
                dns[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(configInputStrings[I_NTP1])))
                == 0)
                ntp[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP2])))
                     == 0)
                ntp[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP3])))
                     == 0)
                ntp[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP4])))
                     == 0)
                ntp[3] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTP])))
                     == 0)
                doNTP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_DHCP])))
                     == 0)
                doDHCP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_NTPPERIOD])))
                     == 0)
                ntpperiod = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_TIMEZONE])))
                     == 0)
                timezone = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_HOUR])))
                     == 0)
                hour = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_MINUTE])))
                     == 0)
                minute = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(configInputStrings[I_SECOND])))
                     == 0)
                second = atoi(value);

        }

        aquaduino->setMAC(mac);
        aquaduino->setIP(&ip);
        aquaduino->setNetmask(&netmask);
        aquaduino->setGateway(&gw);
        aquaduino->setDNS(&dns);
        aquaduino->setNTP(&ntp);

        if (doDHCP)
            aquaduino->enableDHCP();
        else
            aquaduino->disableDHCP();

        if (doNTP)
            aquaduino->enableNTP();
        else
        {
            aquaduino->disableNTP();
            aquaduino->setTime(hour, minute, second, 0, 0, 0);
        }

        aquaduino->setNtpSyncInterval(ntpperiod);

        aquaduino->setTimezone(timezone);

        aquaduino->writeConfig(aquaduino);

        aquaduino->resetActuatorIterator();
        while (aquaduino->getNextActuator(&actuator) != -1)
        {
            aquaduino->writeConfig(actuator);
        }

        server->httpSeeOther("/config");
    }
    return true;
}

/**
 * \brief This command is trigger upon request to the URL "/config"
 * \param[in] server Webserver instance to use
 * \param[in] type Request type
 *
 * Request is redirected from ::controllerDispatchCommand.
 */
int8_t Aquaduino::configWebpage(WebServer* server,
                                WebServer::ConnectionType type)
{
    if (type == WebServer::POST)
    {
        configWebpageProcessPost(server, type);
    }
    else
    {
        server->httpSuccess();
        printConfigWebpage(server);
    }

    return true;
}

/**
 * \brief Processes the POST request of the main web page.
 * \param[in] server Webserver instance to use
 * \param[in] type Request type
 *
 * \return true if successful.
 */
int8_t Aquaduino::mainWebpageProcessPost(WebServer* server,
                                         WebServer::ConnectionType type)
{
    int8_t repeat = 0;
    char name[30], value[30];
    int8_t actuatorIdx;
    int8_t controllerIdx;
    int8_t sensorIdx;
    Actuator* actuator;
    Controller* controller;
    Sensor* sensor;

    while ((repeat = server->readPOSTparam(name, 30, value, 30)) > 0)
    {

        if (name[0] == 'A' && name[1] >= '0' && name[1] <= '9')
        {
            actuatorIdx = atoi(&name[1]);
            getActuator(actuatorIdx)->setName(value);
        }
        else if (name[0] == 'C' && name[1] >= '0' && name[1] <= '9')
        {
            actuatorIdx = atoi(&name[1]);
            controllerIdx = atoi(value);
            getActuator(actuatorIdx)->setController(controllerIdx);
        }
        else if (name[0] == 'L' && name[1] >= '0' && name[1] <= '9')
        {
            actuatorIdx = atoi(&name[1]);
            if (atoi(value) == 1)
                getActuator(actuatorIdx)->lock();
            else
                getActuator(actuatorIdx)->unlock();
        }
        else if (name[0] == 'E' && name[1] >= '0' && name[1] <= '9')
        {
            actuatorIdx = atoi(&name[1]);
            if (atoi(value) == 1)
                getActuator(actuatorIdx)->forceOn();
            else
                getActuator(actuatorIdx)->forceOff();
        }
        else if (name[0] == 'S' && name[1] >= '0' && name[1] <= '9')
        {
            sensorIdx = atoi(&name[1]);
            getSensor(sensorIdx)->setName(value);
        }
        else if (name[0] == 'N' && name[1] >= '0' && name[1] <= '9')
        {
            controllerIdx = atoi(&name[1]);
            getController(controllerIdx)->setName(value);
        }
    }

    resetActuatorIterator();
    while (getNextActuator(&actuator) != -1)
        writeConfig(actuator);

    resetControllerIterator();
    while (getNextController(&controller) != -1)
        writeConfig(controller);

    resetSensorIterator();
    while (getNextSensor(&sensor) != -1)
        writeConfig(sensor);

    server->httpSeeOther("/");

    return true;
}

/**
 * \brief Prints the actuator table below the main information.
 * \param[in] server Webserver instance to use
 *
 * Prints the actuator table using the template specified in pARowFileName.
 */
int8_t Aquaduino::printMainActuatorTable(WebServer* server)
{
    int i, j;
    int16_t matchIdx = 0;
    TemplateParser* parser;
    char actuatorID[5], controllerID[5], lockedID[5], stateID[5];
    char aRowFileName[size_pARowFileName];
    File templateARow;
    Actuator* currentActuator;
    Controller* currentController;

    parser = aquaduino->getTemplateParser();
    strcpy_P(aRowFileName, pARowFileName);

    aquaduino->resetActuatorIterator();

    while ((i = aquaduino->getNextActuator(&currentActuator)) != -1)
    {
        templateARow = SD.open(aRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateARow,
                                                      actuatorTemplate,
                                                      nr_actuatorTemplate,
                                                      server))
               != -1)
        {

            switch (matchIdx)
            {
            case A_COLOR:
                if (i % 2 == 0)
                {
                    server->print("#FFFFFF");
                }
                else
                {
                    server->print("#99CCFF");
                }
                break;
            case A_IACTUATOR:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            case A_ACTUATORNAME:
                server->print(currentActuator->getName());
                break;
            case A_CSELECT:
                controllerID[0] = 'C';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case A_COPTIONS:
                aquaduino->resetControllerIterator();
                while ((j = aquaduino->getNextController(&currentController)) != -1)
                {
                    itoa(j, controllerID, 10);
                    parser->selectListOption(currentController->getName(),
                                             controllerID,
                                             currentActuator->getController() == j,
                                             server);
                }
                break;
            case A_LSELECT:
                lockedID[0] = 'L';
                itoa(i, &lockedID[1], 10);
                server->print(lockedID);
                break;
            case A_LOPTIONS:
                parser->selectListOption("Unlocked", "0", 0, server);
                parser->selectListOption("Locked",
                                         "1",
                                         currentActuator->isLocked(),
                                         server);
                break;
            case A_SSELECT:
                stateID[0] = 'E';
                itoa(i, &stateID[1], 10);
                server->print(stateID);
                break;
            case A_SOPTIONS:
                parser->selectListOption("Off", "0", 0, server);
                parser->selectListOption("On",
                                         "1",
                                         currentActuator->isOn(),
                                         server);
                break;
            case A_LINK:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            }
        }
        templateARow.close();
    }

    return 1;
}

/**
 * \brief Prints the controller table below the main information.
 * \param[in] server Webserver instance to use
 *
 * Prints the controller table using the template specified in pCRowFileName.
 */
int8_t Aquaduino::printMainControllerTable(WebServer* server)
{
    TemplateParser* parser;
    Controller* controller;
    int16_t matchIdx;
    File templateCRow;
    char cRowFileName[size_pCRowFileName];
    char controllerID[5];
    int8_t i;

    strcpy_P(cRowFileName, pCRowFileName);
    templateCRow = SD.open(cRowFileName, FILE_READ);

    parser = aquaduino->getTemplateParser();

    resetControllerIterator();
    while ((i = getNextController(&controller)) != -1)
    {
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateCRow,
                                                      controllerTemplate,
                                                      nr_controllerTemplate,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case C_COLOR:
                if (i % 2 == 0)
                    server->print("#FFFFFF");
                else
                    server->print("#99CCFF");
                break;
            case C_INAME:
                controllerID[0] = 'N';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case C_NAME:
                server->print(controller->getName());
                break;
            case C_LINK:
                server->print(controller->getURL());
                break;
            }
        }
        templateCRow.seek(SEEK_SET);
    }
    templateCRow.close();

    return 1;
}

/**
 * \brief Prints the sensor table below the main information.
 * \param[in] server Webserver instance to use
 *
 * Prints the sensor table using the template specified in pCRowFileName.
 */
int8_t Aquaduino::printMainSensorTable(WebServer* server)
{
    TemplateParser* parser;
    Sensor* sensor;
    int16_t matchIdx;
    File templateSRow;
    char sRowFileName[size_pSRowFileName];
    char sensorID[5];
    int8_t i;

    strcpy_P(sRowFileName, pSRowFileName);
    templateSRow = SD.open(sRowFileName, FILE_READ);

    parser = aquaduino->getTemplateParser();

    resetSensorIterator();
    while ((i = getNextSensor(&sensor)) != -1)
    {
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateSRow,
                                                      sensorTemplate,
                                                      nr_sensorTemplate,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case S_COLOR:
                if (i % 2 == 0)
                    server->print("#FFFFFF");
                else
                    server->print("#99CCFF");
                break;
            case S_INAME:
                sensorID[0] = 'S';
                itoa(i, &sensorID[1], 10);
                server->print(sensorID);
                break;
            case S_NAME:
                server->print(sensor->getName());
                break;
            case S_VALUE:
                server->print(m_SensorReadings[i]);
                break;

            case S_LINK:
                server->print(sensor->getURL());
                break;
            }
        }
        templateSRow.seek(SEEK_SET);
    }
    templateSRow.close();

    return 1;
}

/**
 * \brief Prints the main web page
 * \param[in] server Webserver instance to use
 *
 * \returns true if successful.
 */

int8_t Aquaduino::printMainWebpage(WebServer* server)
{
    TemplateParser* parser;
    char fileName[size_pMFileName];
    File templateFile;
    int16_t matchIdx;

    parser = aquaduino->getTemplateParser();

    strcpy_P(fileName, pMFileName);
    templateFile = SD.open(fileName, FILE_READ);

    server->httpSuccess();

    matchIdx = 0;

    while ((matchIdx = parser->processTemplateUntilNextMatch(&templateFile,
                                                             mainTemplate,
                                                             nr_mainTemplate,
                                                             server))
           >= 0)
    {
        switch (matchIdx)
        {
        case M_HOUR:
            server->print(hour());
            break;
        case M_MINUTE:
            server->print(minute());
            break;
        case M_SECOND:
            server->print(second());
            break;
        case M_CONTROLLER:
            printMainControllerTable(server);
            break;
        case M_SENSOR:
            printMainSensorTable(server);
            break;
        case M_ACTUATOR:
            printMainActuatorTable(server);
            break;
        }
    }

    templateFile.close();

    return 1;
}

/**
 * \brief Handler for main web page
 * \param[in] server Webserver instance to use
 * \param[in] type Request type
 *
 * Dispatches the request to the appropriate submethod.
 *
 * \returns true if successful.
 */
int8_t Aquaduino::mainWebpage(WebServer* server, WebServer::ConnectionType type)
{
    if (type == WebServer::POST)
        return mainWebpageProcessPost(server, type);
    else if (type != WebServer::HEAD)
        printMainWebpage(server);
    return 1;
}

/**
 * \brief Handles all request not covered by actuators, controllers and sensors
 *
 * Provides man web page and configuration web page.
 *
 * \returns true if successful.
 */
int8_t Aquaduino::showWebinterface(WebServer* server,
                                   WebServer::ConnectionType type, char* url)
{
    if (strcmp("config", url) == 0)
        return configWebpage(server, type);

    return mainWebpage(server, type);
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
    int8_t sensorIdx = -1;
    Controller* currentController;
    Sensor* currentSensor;

    m_Sensors.resetIterator();
    while ((sensorIdx = m_Sensors.getNext(&currentSensor)) != -1)
    {
        m_SensorReadings[sensorIdx] = currentSensor->read();
    }

    m_Controllers.resetIterator();
    while ((controllerIdx = m_Controllers.getNext(&currentController)) != -1)
    {
        currentController->run();
    }

    if (m_WebServer != NULL)
    {
        m_WebServer->processConnection();
    }
}

/*
 * ============================================================================
 */

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup()
{
    Serial.begin(9600);
    Serial.println(F("Starting Aquaduino..."));
    new Aquaduino();

    Serial.println(F("Starting main loop..."));
}

void loop()
{
    aquaduino->run();
}
