/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
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

#ifndef SDCONFIGMANAGER_H_
#define SDCONFIGMANAGER_H_

#include "ConfigManager.h"

static const int8_t PREFIX_LENGTH = 12;
static const int8_t FILENAME_LENGTH = 12;

/**
 * \brief Configuration Manager using SD card to store and restore
 * configurations.
 *
 * This configuration manager uses the availabe SD card. The initialization
 * of the SD class is implemented in Aquaduino::Aquaduino.
 */
class SDConfigManager: public ConfigManager
{
public:
    SDConfigManager();
    SDConfigManager(const char* prefix);

    virtual int8_t writeConfig(Aquaduino* aquaduino);
    virtual int8_t writeConfig(Actuator* actuator);
    virtual int8_t writeConfig(Controller* controller);
    virtual int8_t writeConfig(Sensor* sensor);

    virtual int8_t readConfig(Aquaduino* aquaduino);
    virtual int8_t readConfig(Actuator* actuator);
    virtual int8_t readConfig(Controller* controller);
    virtual int8_t readConfig(Sensor* sensor);

protected:
    uint16_t writeStructToFile(const char* filename,
                               struct configuration* config);
    uint16_t readStructFromFile(const char* filename,
                                struct configuration* config);

private:
    SDConfigManager(SDConfigManager&);
    SDConfigManager(const SDConfigManager&);
    virtual ~SDConfigManager();
    char m_folder[PREFIX_LENGTH];
};

#endif /* SDCONFIGMANAGER_H_ */
