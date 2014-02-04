#include <Aquaduino.h>
#include "Config.h"
#include <Controller/TemperatureController.h>
#include <Controller/LevelController.h>
#include <Controller/ClockTimerController.h>
#include <Actuators/DigitalOutput.h>
#include <Sensors/DigitalInput.h>
#include <Sensors/DS18S20.h>
#include <Sensors/SerialInput.h>

Aquaduino *aquaduino;

void setup()
{
    int8_t i;
    int8_t actuatorConfig[MAX_ACTUATORS] = ACTUATOR_CONFIG;
    int8_t controllerConfig[MAX_CONTROLLERS] = CONTROLLER_CONFIG;
    int8_t sensorConfig[MAX_SENSORS] = SENSOR_CONFIG;

    aquaduino = new Aquaduino();

    Serial.println(F("Initializing actuators..."));
    for (i = 0; i < ACTUATORS; i++)
    {
        switch (actuatorConfig[i])
        {
            case ACTUATOR_DIGITALOUTPUT:
                aquaduino->addActuator(new DigitalOutput(NULL, HIGH, LOW));
                break;
            default:
            	Serial.println("Unknown actuator type");
        }
    }

    Serial.println(F("Initializing controllers..."));
    for (int i = 0; i < CONTROLLERS; i++)
    {
        switch (controllerConfig[i])
        {
            case CONTROLLER_LEVEL:
                aquaduino->addController(new LevelController("Level"));
                break;
            case CONTROLLER_TEMPERATURE:
                aquaduino->addController(new TemperatureController("Temperature"));
                break;
            case CONTROLLER_CLOCKTIMER:
                aquaduino->addController(new ClockTimerController("Clock Timer"));
                break;
            default:
            	Serial.println("Unknown controller type");
        }
    }

    Serial.println(F("Initializing sensors..."));
    for (int i = 0; i < SENSORS; i++)
    {
        switch (sensorConfig[i])
        {
            case SENSOR_DIGITALINPUT:
                aquaduino->addSensor(new DigitalInput());
                break;
            case SENSOR_DS18S20:
                aquaduino->addSensor(new DS18S20());
                break;
            case SENSOR_SERIALINPUT:
                aquaduino->addSensor(new SerialInput());
                break;
            default:
            	Serial.println("Unknown sensor type");
        }
    }

    aquaduino->initXively();

    aquaduino->startTimer();
}

void loop()
{
    aquaduino->run();
}
