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
 * The Aquaduino framework uses Webduino for its webinterface. To provide a seamless
 * integration of actuators like relays into controlling elements the Aquaduino
 * webinterface automatically generates a configuration webinterface to assign
 * the available actuators to the controlling elements of Aquaduino. Thus the
 * actuators can easily be enabled, disabled or assigned at runtime to the
 * different control elements.
 *
 */

#include <Aquaduino.h>
#include <Controller/TemperatureController.h>
#include <Controller/LevelController.h>
#include <Controller/ClockTimerController.h>
#include <Actuators/DigitalOutput.h>
#include <Sensors/DS18S20.h>
#include <Sensors/DigitalInput.h>
#include <Sensors/SerialAtlasPH.h>
#include <Sensors/SerialAtlasEC.h>
#include <Sensors/SerialAtlasORP.h>
#include <SD.h>
#include <Time.h>
#include <EthernetUdp.h>
#include <stdlib.h>


#ifdef FEATURE_WEBIF
#include <WebServer.h>
#include <Framework/Flashvars.h>
#include <TemplateParser.h>
#endif

Aquaduino* __aquaduino;

extern time_t NTPSync();
extern int freeRam();

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
        m_Xively(0),
        m_Controllers(MAX_CONTROLLERS),
        m_Actuators(MAX_ACTUATORS),
        m_Sensors(MAX_SENSORS),
#ifdef FEATURE_WEBIF
        m_WebServer(NULL),
        m_TemplateParser(NULL),
#endif
        m_XivelyClient(ethClient)
{
    __aquaduino = this;
    m_Type = AQUADUINO;

    initPeripherals();
	Serial.print(F("Startup Free Ram: "));
	Serial.println(freeRam());

    //ToDo: Buggy!
    m_ConfigManager = new SDConfigManager();
    readConfig(this);

    initNetwork();
    initXively();

#ifdef INTERRUPT_DRIVEN
    Serial.println("Interrupt triggered mode enabled.");
    startTimer();
#else
    Serial.println("Software triggered mode enabled.");
#endif

}

/**
 * \brief Initialize peripherals of Arduino Board
 *
 *
 */

void Aquaduino::initPeripherals(){

	//Init Serial Port
	Serial.begin(115200);

    // Deselect all SPI devices!
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);

    //Initializing SD slot
    if (!SD.begin(4))
    {
        Serial.println(F("No SD Card available"));
        while (1)
            ;
    }

    //Setting the PWM frequencies to 31.25kHz
	Serial.println(F("Initializing PWM frequency to 31.25 kHz..."));
    TCCR1A = _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(CS10);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20);
    TCCR3A = _BV(WGM31) | _BV(WGM30);
    TCCR3B = _BV(CS30);
    TCCR4A = _BV(WGM41) | _BV(WGM40);
    TCCR4B = _BV(CS40);
    TCCR5A = _BV(WGM51) | _BV(WGM50);
    TCCR5B = _BV(CS50);

    Serial.println(F("Initializing OneWire Handler..."));
    m_OneWireHandler = new OneWireHandler();
}

/**
 * \brief Initialize peripherals of Arduino Board
 *
 *
 */

void Aquaduino::initNetwork()
{
	int8_t status = 0;

    m_MAC[0] = 0xDE;
    m_MAC[1] = 0xAD;
    m_MAC[2] = 0xBE;
    m_MAC[3] = 0xEF;
    m_MAC[4] = 0xDE;
    m_MAC[5] = 0xAD;

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
    setTime(0, 0, 0, 1, 1, 2013);

    if (isNTPEnabled())
    {
        Serial.println(F("Syncing time using NTP..."));
        enableNTP();
    }

#ifdef FEATURE_WEBIF
    Serial.println(F("Starting Webserver..."));
    setWebserver(new WebServer("", 80));
#endif

    m_GUIServer = new GUIServer(4242);
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
 * \brief Enables Xively.
 *
 * Enables the Xively flag. When this flag is set sensor data with valid
 * Xively channels will be send to Xively.
 */

void Aquaduino::initXively()
{
    Serial.print(F("Xively API Key: "));
    Serial.println(m_XivelyAPIKey);

    Serial.print(F("Xively Feed: "));
    Serial.println(m_XivelyFeedName);
    Serial.println(F("Xively Channels:"));
    for (uint8_t i = 0; i < getNrOfSensors(); i++)
    {
        Serial.print(i);
        Serial.print(":");
        Serial.println(m_XivelyChannelNames[i]);
        m_XiveleyDatastreams[i] =
                new XivelyDatastream(m_XivelyChannelNames[i],
                                     strlen(m_XivelyChannelNames[i]),
                                     DATASTREAM_FLOAT);
    }

    m_XivelyFeed = new XivelyFeed(atol(m_XivelyFeedName),
                                  m_XiveleyDatastreams,
                                  getNrOfSensors());
}

/**
 * \brief Enables Xively.
 *
 * Enables the Xively flag. When this flag is set sensor data with valid
 * Xively channels will be send to Xively.
 */

void Aquaduino::enableXively()
{
    m_Xively = 1;
}

/**
 * \brief Disables Xively flag.
 *
 */
void Aquaduino::disableXively()
{
    m_Xively = 0;
}

/**
 * \brief Checks whether Xively is enabled or not.
 *
 * \returns Value of the Xively flag.
 */
int8_t Aquaduino::isXivelyEnabled()
{
    return m_Xively;
}

void Aquaduino::setXivelyApiKey(const char* key)
{
    strcpy(m_XivelyAPIKey, key);
}
const char* Aquaduino::getXivelyApiKey()
{
    return m_XivelyAPIKey;
}

void Aquaduino::setXivelyFeed(const char* feed)
{
    strcpy(m_XivelyFeedName, feed);
}

const char* Aquaduino::getXivelyFeed()
{
    return m_XivelyFeedName;
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

    for (actuatorIdx = 0; actuatorIdx < MAX_ACTUATORS; actuatorIdx++)
    {
        currentActuator = m_Actuators.get(actuatorIdx);
        if (currentActuator && currentActuator->getController()
                == controllerIdx)
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

    //m_Actuators.resetIterator();
    for (actuatorIdx = 0; actuatorIdx < MAX_ACTUATORS; actuatorIdx++)
    {
        currentActuator = m_Actuators.get(actuatorIdx);
        if (currentActuator && currentActuator->getController()
                == controllerIdx)
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

OneWireHandler* Aquaduino::getOneWireHandler()
{
    return m_OneWireHandler;
}

/*
 * ============================================================================
 */

const uint16_t Aquaduino::m_Size = sizeof(m_MAC) + sizeof(uint32_t)
                                   + sizeof(uint32_t) + sizeof(uint32_t)
                                   + sizeof(uint32_t) + sizeof(uint32_t)
                                   + sizeof(m_NTPSyncInterval) + sizeof(m_DHCP)
                                   + sizeof(m_NTP) + sizeof(m_Timezone)
                                   + sizeof(m_Xively) + sizeof(m_XivelyAPIKey)
                                   + sizeof(m_XivelyFeed)
                                   + sizeof(m_XivelyChannelNames);

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
uint16_t Aquaduino::serialize(Stream* s)
{
    return 0;
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
uint16_t Aquaduino::deserialize(Stream* s)
{

    uint16_t stringLength = 0;
    uint16_t xivelyFeedNameLength = 0;
    uint16_t xivelyApiKeyLength = 0;
    uint16_t xivelyChannelNameLength = 0;
    uint16_t nrOfActuators = 0;
    uint16_t nrOfControllers = 0;
    uint16_t nrOfSensors = 0;
    uint16_t calculatedSize = 0;
    uint16_t i = 0;
    uint16_t size = s->available();

    memset(m_XivelyAPIKey, 0, sizeof(m_XivelyAPIKey));
    memset(m_XivelyFeedName, 0, sizeof(m_XivelyFeedName));
    memset(m_XiveleyDatastreams, 0, sizeof(m_XiveleyDatastreams));
    memset(m_XivelyChannelNames, 0, sizeof(m_XivelyChannelNames));

    if (size >= 7)
    {
        stringLength = s->read();
        xivelyFeedNameLength = s->read();
        xivelyApiKeyLength = s->read();
        xivelyChannelNameLength = s->read();
        nrOfActuators = s->read();
        nrOfControllers = s->read();
        nrOfSensors = s->read();
    }

    // See Main.java in Aquaduino-Config for calculation
    calculatedSize = 7
            + ((nrOfActuators + nrOfControllers + nrOfSensors) * stringLength)
            + (2 * nrOfActuators) + nrOfControllers + (2 * nrOfSensors)
            + (nrOfSensors * xivelyChannelNameLength) + 6 + 1 + 4 + 4 + 4 + 1
            + 4 + 1 + 1 + 1 + xivelyApiKeyLength + xivelyFeedNameLength;

    if ((calculatedSize != size) || (stringLength != AQUADUINO_STRING_LENGTH)
        || (xivelyFeedNameLength != XIVELY_FEED_NAME_LENGTH)
        || (xivelyApiKeyLength != XIVELY_API_KEY_LENGTH)
        || (xivelyChannelNameLength != XIVELY_CHANNEL_NAME_LENGTH)
        || (nrOfActuators > MAX_ACTUATORS)
        || (nrOfControllers > MAX_CONTROLLERS) || (nrOfSensors > MAX_SENSORS))
    {
        Serial.println(F("Invalid configuration file size!"));
        Serial.print(calculatedSize);
        Serial.print("?=");
        Serial.println(size);
    }

    for (i = 0; i < nrOfActuators; i++)
    {

        char name[stringLength];
        s->readBytes(name, stringLength);
        uint8_t typeId = s->read();
        uint8_t portId = s->read();
        Actuator* actuator;
        int8_t idx;

        switch(typeId){
        case 1:
            actuator = new DigitalOutput(name, 1, 0);
            ((DigitalOutput*) actuator)->setPin(portId);
            break;
        default:
            actuator = NULL;
            break;
        }

        if ( (actuator != NULL) && (idx = __aquaduino->addActuator(actuator)) != -1)
        {
            readConfig(actuator);
        }
    }

    for (i = 0; i < nrOfControllers; i++)
    {

    	char name[stringLength];
    	s->readBytes(name, stringLength);
        uint8_t typeId = s->read();
        Controller* controller;
        int8_t idx;

        switch(typeId){
        case 1:
            controller = new LevelController(name);
            break;
        case 2:
            controller = new TemperatureController(name);
            break;
        case 3:
            controller = new ClockTimerController(name);
            break;
        default:
            controller = NULL;
            break;
        }

        if ( (controller != NULL) && (idx = __aquaduino->addController(controller)) != -1)
        {
            readConfig(controller);
        }
    }

    for (i = 0; i < nrOfSensors; i++)
    {

    	char name[stringLength];
    	char xivelyChannel[xivelyChannelNameLength];
    	s->readBytes(name, stringLength);
        uint8_t typeId = s->read();
        uint8_t portId = s->read();
        s->readBytes(xivelyChannel, xivelyChannelNameLength);
        Sensor* sensor;
        int8_t idx;

        switch(typeId){
        case 1:
            sensor = new DigitalInput();
            ((DigitalInput*) sensor)->setPin(portId);
            ((DigitalInput*) sensor)->setName(name);
            break;
        case 2:
            sensor = new DS18S20();
            ((DS18S20*) sensor)->setPin(portId);
            ((DS18S20*) sensor)->setName(name);
            m_OneWireHandler->addPin(portId);
            break;
        case 3:
            sensor = new SerialAtlasPH();
            ((SerialAtlasPH*) sensor)->setName(name);
            break;
        case 4:
            sensor = new SerialAtlasEC();
            ((SerialAtlasEC*) sensor)->setName(name);
            break;
        case 5:
            sensor = new SerialAtlasORP();
            ((SerialAtlasORP*) sensor)->setName(name);
            break;
        default:
            sensor = NULL;
            break;
        }

        if ( (sensor != NULL) && (idx = __aquaduino->addSensor(sensor)) != -1)
        {
            memcpy(m_XivelyChannelNames[idx], xivelyChannel, xivelyChannelNameLength);
            readConfig(sensor);
        }
    }
    s->readBytes((char*)m_MAC, sizeof(m_MAC));
    Serial.print(F("MAC: "));
    for (uint8_t i = 0; i < sizeof(m_MAC); i++){
        Serial.print(m_MAC[i], HEX);
        if (i != sizeof(m_MAC)-1)
            Serial.print(":");
    }
    Serial.println();

    s->readBytes((char*)&m_DHCP, sizeof(m_DHCP));
    Serial.print(F("DHCP: "));
    Serial.println(m_DHCP);

    Serial.print(F("IP: "));
    for (int i = 0; i < 4; i++){
    	m_IP[i]= s->read();
        Serial.print(m_IP[i]);
        if (i != 3)
            Serial.print(".");

    }
    Serial.println();

    Serial.print(F("Netmask: "));
    for (int i = 0; i < 4; i++){
    	m_Netmask[i] = s->read();
        Serial.print(m_Netmask[i]);
        if (i != 3)
            Serial.print(".");

    }
    Serial.println();

    Serial.print(F("Gateway: "));
    for (int i = 0; i < 4; i++){
    	m_Gateway[i] = s->read();
        Serial.print(m_Gateway[i]);
        if (i != 3)
            Serial.print(".");

    }
    Serial.println();

    s->readBytes((char*)&m_NTP, 1);
    Serial.print(F("NTP: "));
    Serial.println(m_NTP);


    Serial.print(F("NTP Server: "));
    for (int i = 0; i < 4; i++){
    	m_NTPServer[i] = s->read();
        Serial.print(m_NTPServer[i]);
        if (i != 3)
            Serial.print(".");

    }
    Serial.println();

	//ToDo: Inconsistent to Aquaduino-Config
    s->readBytes((char*)&m_NTPSyncInterval, 1);
    Serial.print(F("NTP Sync Interval: "));
    Serial.println(m_NTPSyncInterval);

    s->readBytes((char*)&m_Timezone, sizeof(m_Timezone));
    Serial.print(F("Timezone: "));
    Serial.println(m_Timezone);

    s->readBytes((char*)&m_Xively, sizeof(m_Xively));
    Serial.print(F("Xively: "));
    Serial.println(m_Xively);

    s->readBytes((char*)&m_XivelyAPIKey, sizeof(m_XivelyAPIKey));
    Serial.print(F("Xively API Key: "));
    Serial.println(m_XivelyAPIKey);

    s->readBytes((char*)&m_XivelyFeedName, sizeof(m_XivelyFeedName));
    Serial.print(F("Xively Feed Name: "));
    Serial.println(m_XivelyFeedName);

    return size;
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
	if (m_ConfigManager != NULL)
	{
		//ToDo:Buggy
		m_ConfigManager->writeConfig(aquaduino);
	}
	return 0;
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
	if (m_ConfigManager != NULL)
	{
		//ToDo:Buggy
		m_ConfigManager->writeConfig(actuator);
	}
	return 0;
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
	if (m_ConfigManager != NULL)
	{
		//ToDo:Buggy
		m_ConfigManager->writeConfig(controller);
	}
	return 0;
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
	if (m_ConfigManager != NULL)
	{
		//ToDo:Buggy
		m_ConfigManager->writeConfig(sensor);
	}
	return 0;
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
	IPAddress ip(192,168,2,2);
	IPAddress nm(255,255,255,0);

	if (m_ConfigManager != NULL)
	{
		Serial.println(F("Reading aqua.cfg..."));
		m_ConfigManager->readConfig(aquaduino);
		Serial.println(F("Reading aqua.cfg finished."));
	}
	return 0;
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
	if (m_ConfigManager != NULL)
	{
		//ToDo:Buggy
		m_ConfigManager->readConfig(actuator);
	}
	return 0;
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
	if (m_ConfigManager != NULL)
	{
		m_ConfigManager->readConfig(controller);
	}
	return 0;
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
	if (m_ConfigManager != NULL)
	{
		//ToDo:Buggy
		m_ConfigManager->readConfig(sensor);
	}
	return 0;
}

#ifdef FEATURE_WEBIF
void defaultCmd(WebServer &server, WebServer::ConnectionType type, char * url,
                bool);
#endif

#ifdef FEATURE_WEBIF
void dispatchCommand(WebServer &server, WebServer::ConnectionType type,
                     char **url_path, char *url_tail, bool tail_complete);
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Setter for the Webduino webserver instance.
 * \param[in] webServer WebServer instance to be used.
 *
 * Sets the WebServer instance to be used and attaches the prepared handler
 * routines. Finally the WebServer is initialized.
 *
 * See ::defaultCmd for implementation of the main web page and
 * ::dispatchCommand for the implementation of the request
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
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Getter for the WebServer
 *
 * \returns The instance of the used WebServer
 */
WebServer* Aquaduino::getWebserver()
{
    return m_WebServer;
}
#endif

#ifdef FEATURE_WEBIF
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
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Getter for the TemplateParser
 *
 * \returns The instance of the used TemplateParser
 */
TemplateParser* Aquaduino::getTemplateParser()
{
    return m_TemplateParser;
}
#endif

/**
 * ----------------------------------------------------------------------------
 *
 */

#ifdef FEATURE_WEBIF
/**
 * \brief Prints the main web page.
 *
 * Prints the main web page specified in pMFileName.
 */
void defaultCmd(WebServer &server, WebServer::ConnectionType type, char * url,
                bool)
{
    __aquaduino->showWebinterface(&server, type, url);
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

        __aquaduino->resetControllerIterator();
        while (__aquaduino->getNextController(&controller) != -1)
        {
            if (strcmp(topLevelURL, controller->getURL()) == 0)
            {
                controller->showWebinterface(&server, type, subURL);
                if (type == WebServer::POST)
                    __aquaduino->writeConfig(controller);
                return;
            }
        }

        __aquaduino->resetSensorIterator();
        while (__aquaduino->getNextSensor(&sensor) != -1)
        {
            if (strcmp(topLevelURL, sensor->getURL()) == 0)
            {
                sensor->showWebinterface(&server, type, subURL);
                if (type == WebServer::POST)
                    __aquaduino->writeConfig(sensor);
                return;
            }
        }

        __aquaduino->resetActuatorIterator();
        while (__aquaduino->getNextActuator(&actuator) != -1)
        {
            if (strcmp(topLevelURL, actuator->getURL()) == 0)
            {
                actuator->showWebinterface(&server, type, subURL);
                if (type == WebServer::POST)
                    __aquaduino->writeConfig(actuator);
                return;
            }
        }

        __aquaduino->showWebinterface(&server, type, *url_path);
    }
}
#endif

int freeRam();

#ifdef FEATURE_WEBIF
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

    char templateFileName[sizeof(template_config_fnsize)];
    strcpy_P(templateFileName, template_config_fname);

    parser = getTemplateParser();
    templateFile = SD.open(templateFileName, FILE_READ);

    getMAC(mac);
    ip = getIP();
    netmask = getNetmask();
    gw = getGateway();
    dns = getDNS();
    ntp = getNTP();

    while ((matchIdx =
            parser->processTemplateUntilNextMatch(&templateFile,
                                                  template_config,
                                                  template_config_elements,
                                                  server))
           >= 0)
    {
        switch (matchIdx)
        {
        case CONFIG_MAC1_VAL:
            server->print(mac[0], HEX);
            break;
        case CONFIG_MAC2_VAL:
            server->print(mac[1], HEX);
            break;
        case CONFIG_MAC3_VAL:
            server->print(mac[2], HEX);
            break;
        case CONFIG_MAC4_VAL:
            server->print(mac[3], HEX);
            break;
        case CONFIG_MAC5_VAL:
            server->print(mac[4], HEX);
            break;
        case CONFIG_MAC6_VAL:
            server->print(mac[5], HEX);
            break;
        case CONFIG_MAC1_NAME:
            server->print((__FlashStringHelper*) pgm_input_mac1);
            break;
        case CONFIG_MAC2_NAME:
            server->print((__FlashStringHelper*) pgm_input_mac2);
            break;
        case CONFIG_MAC3_NAME:
            server->print((__FlashStringHelper*) pgm_input_mac3);
            break;
        case CONFIG_MAC4_NAME:
            server->print((__FlashStringHelper*) pgm_input_mac4);
            break;
        case CONFIG_MAC5_NAME:
            server->print((__FlashStringHelper*) pgm_input_mac5);
            break;
        case CONFIG_MAC6_NAME:
            server->print((__FlashStringHelper*) pgm_input_mac6);
            break;
        case CONFIG_DHCPSELECTOPTION:
            if (__aquaduino->isDHCPEnabled())
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
        case CONFIG_IP1_VAL:
            server->print((*ip)[0]);
            break;
        case CONFIG_IP2_VAL:
            server->print((*ip)[1]);
            break;
        case CONFIG_IP3_VAL:
            server->print((*ip)[2]);
            break;
        case CONFIG_IP4_VAL:
            server->print((*ip)[3]);
            break;
        case CONFIG_IP1_NAME:
            server->print((__FlashStringHelper*) pgm_input_ip1);
            break;
        case CONFIG_IP2_NAME:
            server->print((__FlashStringHelper*) pgm_input_ip2);
            break;
        case CONFIG_IP3_NAME:
            server->print((__FlashStringHelper*) pgm_input_ip3);
            break;
        case CONFIG_IP4_NAME:
            server->print((__FlashStringHelper*) pgm_input_ip4);
            break;
        case CONFIG_NETMASK1_VAL:
            server->print((*netmask)[0]);
            break;
        case CONFIG_NETMASK2_VAL:
            server->print((*netmask)[1]);
            break;
        case CONFIG_NETMASK3_VAL:
            server->print((*netmask)[2]);
            break;
        case CONFIG_NETMASK4_VAL:
            server->print((*netmask)[3]);
            break;
        case CONFIG_NETMASK1_NAME:
            server->print((__FlashStringHelper*) pgm_input_netmask1);
            break;
        case CONFIG_NETMASK2_NAME:
            server->print((__FlashStringHelper*) pgm_input_netmask2);
            break;
        case CONFIG_NETMASK3_NAME:
            server->print((__FlashStringHelper*) pgm_input_netmask3);
            break;
        case CONFIG_NETMASK4_NAME:
            server->print((__FlashStringHelper*) pgm_input_netmask4);
            break;
        case CONFIG_GATEWAY1_VAL:
            server->print((*gw)[0]);
            break;
        case CONFIG_GATEWAY2_VAL:
            server->print((*gw)[1]);
            break;
        case CONFIG_GATEWAY3_VAL:
            server->print((*gw)[2]);
            break;
        case CONFIG_GATEWAY4_VAL:
            server->print((*gw)[3]);
            break;
        case CONFIG_GATEWAY1_NAME:
            server->print((__FlashStringHelper*) pgm_input_gateway1);
            break;
        case CONFIG_GATEWAY2_NAME:
            server->print((__FlashStringHelper*) pgm_input_gateway2);
            break;
        case CONFIG_GATEWAY3_NAME:
            server->print((__FlashStringHelper*) pgm_input_gateway3);
            break;
        case CONFIG_GATEWAY4_NAME:
            server->print((__FlashStringHelper*) pgm_input_gateway4);
            break;
        case CONFIG_DNS1_VAL:
            server->print((*dns)[0]);
            break;
        case CONFIG_DNS2_VAL:
            server->print((*dns)[1]);
            break;
        case CONFIG_DNS3_VAL:
            server->print((*dns)[2]);
            break;
        case CONFIG_DNS4_VAL:
            server->print((*dns)[3]);
            break;
        case CONFIG_DNS1_NAME:
            server->print((__FlashStringHelper*) pgm_input_dns1);
            break;
        case CONFIG_DNS2_NAME:
            server->print((__FlashStringHelper*) pgm_input_dns2);
            break;
        case CONFIG_DNS3_NAME:
            server->print((__FlashStringHelper*) pgm_input_dns3);
            break;
        case CONFIG_DNS4_NAME:
            server->print((__FlashStringHelper*) pgm_input_dns4);
            break;
        case CONFIG_NTPSELECTOPTION:
            if (__aquaduino->isNTPEnabled())
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
        case CONFIG_NTP1_VAL:
            server->print((*ntp)[0]);
            break;
        case CONFIG_NTP2_VAL:
            server->print((*ntp)[1]);
            break;
        case CONFIG_NTP3_VAL:
            server->print((*ntp)[2]);
            break;
        case CONFIG_NTP4_VAL:
            server->print((*ntp)[3]);
            break;
        case CONFIG_NTP1_NAME:
            server->print((__FlashStringHelper*) pgm_input_ntp1);
            break;
        case CONFIG_NTP2_NAME:
            server->print((__FlashStringHelper*) pgm_input_ntp2);
            break;
        case CONFIG_NTP3_NAME:
            server->print((__FlashStringHelper*) pgm_input_ntp3);
            break;
        case CONFIG_NTP4_NAME:
            server->print((__FlashStringHelper*) pgm_input_ntp4);
            break;
        case CONFIG_NTPPERIODVAL:
            server->print(m_NTPSyncInterval);
            break;
        case CONFIG_TIMEZONE:
            server->print(m_Timezone);
            break;
        case CONFIG_HOUR:
            server->print(hour());
            break;
        case CONFIG_MINUTE:
            server->print(minute());
            break;
        case CONFIG_SECOND:
            server->print(second());
            break;
        case CONFIG_MONTH:
            server->print(month());
            break;
        case CONFIG_DAY:
            server->print(day());
            break;
        case CONFIG_YEAR:
            server->print(year());
            break;
        case CONFIG_XIVELYSELECT:
            if (isXivelyEnabled())
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
        case CONFIG_XIVELYSELECTNAME:
            server->print((__FlashStringHelper*) pgm_input_xively);
            break;
        case CONFIG_XIVELYAPIKEYVAL:
            server->print(getXivelyApiKey());
            break;
        case CONFIG_XIVELYAPIKEYNAME:
            server->print((__FlashStringHelper*) pgm_input_xively_api_key);
            break;
        case CONFIG_XIVELYAPIKEYSIZE:
            server->print(XIVELY_API_KEY_LENGTH+1);
            break;
        case CONFIG_XIVELYAPIKEYMAXLENGTH:
            server->print(XIVELY_API_KEY_LENGTH);
            break;
        case CONFIG_XIVELYFEEDVAL:
            server->print(getXivelyFeed());
            break;
        case CONFIG_XIVELYFEEDNAME:
            server->print((__FlashStringHelper*) pgm_input_xively_feed);
            break;
        case CONFIG_XIVELYFEEDSIZE:
            server->print(XIVELY_FEED_NAME_LENGTH+1);
            break;
        case CONFIG_XIVELYFEEDMAXLENGTH:
            server->print(XIVELY_FEED_NAME_LENGTH);
            break;
        case CONFIG_FREERAM:
            server->print(freeRam());
            server->print(F(" Bytes"));
            break;
        }
    }
    templateFile.close();
}
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief This command is trigger upon request to the URL "/config"
 * \param[in] server Webserver instance to use
 * \param[in] type Request type
 *
 * Request is redirected from ::dispatchCommand.
 */
int8_t Aquaduino::configWebpageProcessPost(WebServer* server,
                                           WebServer::ConnectionType type)
{
    int8_t repeat;
    uint8_t mac[6];
    char name[20], value[50];
    IPAddress ip, netmask, gw, dns, ntp;
    int8_t doNTP = 0, doDHCP = 0;
    uint16_t ntpperiod = 5;
    int8_t timezone = TIME_ZONE;
    int8_t hour = 0;
    int8_t minute = 0;
    int8_t second = 0;
    int8_t day = 0;
    int8_t month = 0;
    uint16_t year = 0;

    /*
     * TODO: Implement security checks when processing POST parameters
     */
    if (type == WebServer::POST)
    {
        while ((repeat = server->readPOSTparam(name, 20, value, 50)) > 0)
        {
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MAC1])))
                == 0)
                mac[0] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MAC2])))
                     == 0)
                mac[1] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MAC3])))
                     == 0)
                mac[2] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MAC4])))
                     == 0)
                mac[3] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MAC5])))
                     == 0)
                mac[4] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MAC6])))
                     == 0)
                mac[5] = strtol(value, NULL, 16);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_IP1])))
                     == 0)
                ip[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_IP2])))
                     == 0)
                ip[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_IP3])))
                     == 0)
                ip[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_IP4])))
                     == 0)
                ip[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NETMASK1])))
                == 0)
                netmask[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NETMASK2])))
                     == 0)
                netmask[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NETMASK3])))
                     == 0)
                netmask[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NETMASK4])))
                     == 0)
                netmask[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_GATEWAY1])))
                == 0)
                gw[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_GATEWAY2])))
                     == 0)
                gw[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_GATEWAY3])))
                     == 0)
                gw[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_GATEWAY4])))
                     == 0)
                gw[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_DNS1])))
                == 0)
                dns[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_DNS2])))
                     == 0)
                dns[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_DNS3])))
                     == 0)
                dns[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_DNS4])))
                     == 0)
                dns[3] = atoi(value);
            if (strcmp_P(name,
                         (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NTP1])))
                == 0)
                ntp[0] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NTP2])))
                     == 0)
                ntp[1] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NTP3])))
                     == 0)
                ntp[2] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NTP4])))
                     == 0)
                ntp[3] = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NTP])))
                     == 0)
                doNTP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_DHCP])))
                     == 0)
                doDHCP = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_NTPPERIOD])))
                     == 0)
                ntpperiod = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_TIMEZONE])))
                     == 0)
                timezone = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_HOUR])))
                     == 0)
                hour = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MINUTE])))
                     == 0)
                minute = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_SECOND])))
                     == 0)
                second = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_MONTH])))
                     == 0)
                month = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_DAY])))
                     == 0)
                day = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_YEAR])))
                     == 0)
                year = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_XIVELY])))
                     == 0)
                m_Xively = atoi(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_XIVELYAPIKEY])))
                     == 0)
                setXivelyApiKey(value);
            else if (strcmp_P(name,
                              (PGM_P) pgm_read_word(&(template_config_inputs[CONFIG_I_XIVELYFEED])))
                     == 0)
                setXivelyFeed(value);
        }

        __aquaduino->setMAC(mac);
        __aquaduino->setIP(&ip);
        __aquaduino->setNetmask(&netmask);
        __aquaduino->setGateway(&gw);
        __aquaduino->setDNS(&dns);
        __aquaduino->setNTP(&ntp);

        if (doDHCP)
            __aquaduino->enableDHCP();
        else
            __aquaduino->disableDHCP();

        if (doNTP)
            __aquaduino->enableNTP();
        else
        {
            __aquaduino->disableNTP();
            __aquaduino->setTime(hour, minute, second, day, month, year);
        }

        __aquaduino->setNtpSyncInterval(ntpperiod);

        __aquaduino->setTimezone(timezone);

        __aquaduino->writeConfig(__aquaduino);

        __aquaduino->resetActuatorIterator();

        server->httpSeeOther("/config");
    }
    return true;
}
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief This command is trigger upon request to the URL "/config"
 * \param[in] server Webserver instance to use
 * \param[in] type Request type
 *
 * Request is redirected from ::dispatchCommand.
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
    int8_t idx;
    Actuator* actuator;
    Controller* controller;
    Sensor* sensor;

    while ((repeat = server->readPOSTparam(name, 30, value, 30)) > 0)
    {

        if (name[0] == 'A' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            getActuator(idx)->setName(value);
        }
        else if (name[0] == 'C' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            getActuator(idx)->setController(atoi(value));
        }
        else if (name[0] == 'L' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            if (atoi(value) == 1)
                getActuator(idx)->lock();
            else
                getActuator(idx)->unlock();
        }
        else if (name[0] == 'E' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            if (atoi(value) == 1)
                getActuator(idx)->forceOn();
            else
                getActuator(idx)->forceOff();
        }
        else if (name[0] == 'S' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            Serial.println(idx);
            getSensor(idx)->setName(value);
        }
        else if (name[0] == 'N' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            getController(idx)->setName(value);
        }
        else if (name[0] == 'X' && name[1] >= '0' && name[1] <= '9')
        {
            idx = atoi(&name[1]);
            strcpy(m_XivelyChannelNames[idx], value);
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

    writeConfig(this);

    server->httpSeeOther("/");

    return true;
}
#endif

#ifdef FEATURE_WEBIF
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
    char aRowFileName[template_main_actuatorrow_fnsize];
    File templateARow;
    Actuator* currentActuator;
    Controller* currentController;

    parser = __aquaduino->getTemplateParser();
    strcpy_P(aRowFileName, template_main_actuatorrow_fname);

    __aquaduino->resetActuatorIterator();

    while ((i = __aquaduino->getNextActuator(&currentActuator)) != -1)
    {
        templateARow = SD.open(aRowFileName, FILE_READ);
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateARow,
                                                      template_main_actuatorrow,
                                                      template_main_actuatorrow_elements,
                                                      server))
               != -1)
        {

            switch (matchIdx)
            {
            case ACTUATORROW_COLOR:
                if (i % 2 == 0)
                {
                    server->print("#FFFFFF");
                }
                else
                {
                    server->print("#99CCFF");
                }
                break;
            case ACTUATORROW_ACTUATOR_NAME:
                actuatorID[0] = 'A';
                itoa(i, &actuatorID[1], 10);
                server->print(actuatorID);
                break;
            case ACTUATORROW_ACTUATOR_VAL:
                server->print(currentActuator->getName());
                break;
            case ACTUATORROW_ACTUATOR_SIZE:
                server->print(AQUADUINO_STRING_LENGTH+1);
                break;
            case ACTUATORROW_ACTUATOR_MAXLENGTH:
                server->print(AQUADUINO_STRING_LENGTH);
                break;

            case ACTUATORROW_CSELECT:
                controllerID[0] = 'C';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case ACTUATORROW_COPTIONS:
                __aquaduino->resetControllerIterator();
                while ((j = __aquaduino->getNextController(&currentController)) != -1)
                {
                    itoa(j, controllerID, 10);
                    parser->selectListOption(currentController->getName(),
                                             controllerID,
                                             currentActuator->getController() == j,
                                             server);
                }
                break;
            case ACTUATORROW_LSELECT:
                lockedID[0] = 'L';
                itoa(i, &lockedID[1], 10);
                server->print(lockedID);
                break;
            case ACTUATORROW_LOPTIONS:
                parser->selectListOption("Unlocked", "0", 0, server);
                parser->selectListOption("Locked",
                                         "1",
                                         currentActuator->isLocked(),
                                         server);
                break;
            case ACTUATORROW_SSELECT:
                stateID[0] = 'E';
                itoa(i, &stateID[1], 10);
                server->print(stateID);
                break;
            case ACTUATORROW_SOPTIONS:
                parser->selectListOption("Off", "0", 0, server);
                parser->selectListOption("On",
                                         "1",
                                         currentActuator->isOn(),
                                         server);
                break;
            case ACTUATORROW_LINK:
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
#endif

#ifdef FEATURE_WEBIF
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
    char cRowFileName[template_main_controllerrow_fnsize];
    char controllerID[5];
    int8_t i;

    strcpy_P(cRowFileName, template_main_controllerrow_fname);
    templateCRow = SD.open(cRowFileName, FILE_READ);

    parser = __aquaduino->getTemplateParser();

    resetControllerIterator();
    while ((i = getNextController(&controller)) != -1)
    {
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateCRow,
                                                      template_main_controllerrow,
                                                      template_main_controllerrow_elements,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case CONTROLLERROW_COLOR:
                if (i % 2 == 0)
                    server->print(COLOR_CONTROLLER_ROW1);
                else
                    server->print(COLOR_CONTROLLER_ROW2);
                break;
            case CONTROLLERROW_I_CNAME_NAME:
                controllerID[0] = 'N';
                itoa(i, &controllerID[1], 10);
                server->print(controllerID);
                break;
            case CONTROLLERROW_I_CNAME_VAL:
                server->print(controller->getName());
                break;
            case CONTROLLERROW_I_CNAME_SIZE:
                server->print(AQUADUINO_STRING_LENGTH+1);
                break;
            case CONTROLLERROW_I_CNAME_MAXLENGTH:
                server->print(AQUADUINO_STRING_LENGTH);
                break;
            case CONTROLLERROW_CLINK:
                server->print(controller->getURL());
                break;
            }
        }
        templateCRow.seek(SEEK_SET);
    }
    templateCRow.close();

    return 1;
}
#endif

#ifdef FEATURE_WEBIF
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
    char sRowFileName[template_main_sensorrow_fnsize];
    char sensorID[5];
    char xivelyID[5];
    int8_t i;

    strcpy_P(sRowFileName, template_main_sensorrow_fname);
    templateSRow = SD.open(sRowFileName, FILE_READ);

    parser = __aquaduino->getTemplateParser();

    resetSensorIterator();
    while ((i = getNextSensor(&sensor)) != -1)
    {
        while ((matchIdx =
                parser->processTemplateUntilNextMatch(&templateSRow,
                                                      template_main_sensorrow,
                                                      template_main_sensorrow_elements,
                                                      server))
               != -1)
        {
            switch (matchIdx)
            {
            case SENSORROW_COLOR:
                if (i % 2 == 0)
                    server->print(COLOR_SENSOR_ROW1);
                else
                    server->print(COLOR_SENSOR_ROW2);
                break;
            case SENSORROW_I_SNAME_NAME:
                sensorID[0] = 'S';
                itoa(i, &sensorID[1], 10);
                server->print(sensorID);
                break;
            case SENSORROW_I_SNAME_VAL:
                server->print(sensor->getName());
                break;
            case SENSORROW_I_SNAME_SIZE:
                server->print(AQUADUINO_STRING_LENGTH+1);
                break;
            case SENSORROW_I_SNAME_MAXLENGTH:
                server->print(AQUADUINO_STRING_LENGTH);
                break;
            case SENSORROW_SVALUE:
                server->print(m_SensorReadings[i]);
                break;
            case SENSORROW_I_XCHANNEL_VAL:
                server->print(m_XivelyChannelNames[i]);
                break;
            case SENSORROW_I_XCHANNEL_NAME:
                xivelyID[0] = 'X';
                itoa(i, &xivelyID[1], 10);
                server->print(xivelyID);
                break;
                break;
            case SENSORROW_I_XCHANNEL_SIZE:
                server->print(XIVELY_CHANNEL_NAME_LENGTH+1);
                break;
            case SENSORROW_I_XCHANNEL_MAXLENGTH:
                server->print(XIVELY_CHANNEL_NAME_LENGTH);
                break;
            case SENSORROW_SLINK:
                server->print(sensor->getURL());
                break;
            }
        }
        templateSRow.seek(SEEK_SET);
    }
    templateSRow.close();

    return 1;
}
#endif

#ifdef FEATURE_WEBIF
/**
 * \brief Prints the main web page
 * \param[in] server Webserver instance to use
 *
 * \returns true if successful.
 */

int8_t Aquaduino::printMainWebpage(WebServer* server)
{
    TemplateParser* parser;
    char fileName[template_main_fnsize];
    File templateFile;
    int16_t matchIdx;

    parser = __aquaduino->getTemplateParser();

    strcpy_P(fileName, template_main_fname);
    templateFile = SD.open(fileName, FILE_READ);

    server->httpSuccess();

    matchIdx = 0;

    while ((matchIdx = parser->processTemplateUntilNextMatch(&templateFile,
                                                             template_main,
                                                             template_main_elements,
                                                             server))
           >= 0)
    {
        switch (matchIdx)
        {
        case MAIN_HOUR:
            server->print(hour());
            break;
        case MAIN_MINUTE:
            server->print(minute());
            break;
        case MAIN_SECOND:
            server->print(second());
            break;
        case MAIN_DOW:
            switch (day() + 1)
            {
            case dowMonday:
                server->print(F("Monday"));
                break;
                case dowTuesday:
                server->print(F("Tuesday"));
                break;
                case dowWednesday:
                server->print(F("Wednesday"));
                break;
                case dowThursday:
                server->print(F("Thursday"));
                break;
                case dowFriday:
                server->print(F("Friday"));
                break;
                case dowSaturday:
                server->print(F("Saturday"));
                break;
                case dowSunday:
                server->print(F("Sunday"));
                break;
            }
            break;

            case MAIN_MONTH:
            server->print(month());
            break;
            case MAIN_DAY:
            server->print(day());
            break;
            case MAIN_YEAR:
            server->print(year());
            break;
            case MAIN_CONTROLLER:
            printMainControllerTable(server);
            break;
            case MAIN_SENSOR:
            printMainSensorTable(server);
            break;
            case MAIN_ACTUATOR:
            printMainActuatorTable(server);
            break;
        }
    }

    templateFile.close();

    return 1;
}
#endif

#ifdef FEATURE_WEBIF
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
#endif

#ifdef FEATURE_WEBIF
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
#endif

/**
 * \brief Starts triggering of sensors and controller by timer interrupt
 */
void Aquaduino::startTimer()
{
    TCCR5A = 0;
    TCCR5B = 0;
    TCNT5 = 0;
    TIMSK5 = 0;

    OCR5A = 25000;
    TCCR5A = _BV(WGM51) | _BV(WGM50);
    TCCR5B = _BV(CS51) | _BV(CS50) | _BV(WGM53) | _BV(WGM52);

    TIMSK5 = _BV(TOIE5);
}

void Aquaduino::readSensors()
{
    int8_t sensorIdx;
    Sensor* currentSensor;

    for (sensorIdx = 0; sensorIdx < MAX_SENSORS; sensorIdx++)
    {
        currentSensor = m_Sensors.get(sensorIdx);
        if (currentSensor)
        {
            m_SensorReadings[sensorIdx] = currentSensor->read();
            m_XiveleyDatastreams[sensorIdx]->setFloat(m_SensorReadings[sensorIdx]);
        }
        else
        {
            m_SensorReadings[sensorIdx] = 0.0;
            m_XiveleyDatastreams[sensorIdx]->setFloat(0.0);
        }
    }
}

void Aquaduino::executeControllers()
{
    int8_t controllerIdx;
    Controller* currentController;

    for (controllerIdx = 0; controllerIdx < MAX_CONTROLLERS; controllerIdx++)
    {
        currentController = m_Controllers.get(controllerIdx);
        if (currentController)
            currentController->run();
    }
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
    static int8_t curMin = minute();

#ifndef INTERRUPT_DRIVEN
    readSensors();
    executeControllers();
#endif

    if (isXivelyEnabled() && minute() != curMin)
    {
        curMin = minute();
        Serial.print(F("Sending data to Xively... "));
        Serial.println(m_XivelyClient.put(*m_XivelyFeed, m_XivelyAPIKey));
    }
    
    if (m_GUIServer != NULL)
    {
    	m_GUIServer->run();
    }

#ifdef FEATURE_WEBIF
    if (m_WebServer != NULL)
    {
        m_WebServer->processConnection();
    }
#endif
}

ISR(TIMER5_OVF_vect)
{
#ifdef INTERRUPT_DRIVEN
    __aquaduino->readSensors();
    __aquaduino->executeControllers();
#endif
}

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
