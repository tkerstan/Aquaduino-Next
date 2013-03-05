/*
 * SDConfigManager.h
 *
 *  Created on: 04.03.2013
 *      Author: TimoK
 */

#ifndef SDCONFIGMANAGER_H_
#define SDCONFIGMANAGER_H_

#include "ConfigManager.h"

static const int8_t PREFIX_LENGTH = 12;
static const int8_t FILENAME_LENGTH = 12;

class SDConfigManager : public ConfigManager {
public:
	SDConfigManager();
	SDConfigManager(const char* prefix);
	virtual ~SDConfigManager();

	virtual int8_t writeConfig(Aquaduino* aquaduino);
	virtual int8_t writeConfig(Actuator* actuator);
	virtual int8_t writeConfig(Controller* controller);
	virtual int8_t writeConfig(Sensor* sensor);

	virtual int8_t readConfig(Aquaduino* aquaduino);
	virtual int8_t readConfig(Actuator* actuator);
	virtual int8_t readConfig(Controller* controller);
	virtual int8_t readConfig(Sensor* sensor);

protected:
	uint16_t writeStructToFile(const char* filename, struct configuration* config);
	uint16_t readStructFromFile(const char* filename, struct configuration* config);

private:
	char m_prefix[PREFIX_LENGTH];
};

#endif /* SDCONFIGMANAGER_H_ */
