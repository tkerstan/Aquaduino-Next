/*
 * Copyright (c) 2013 Timo Kerstan.  All right reserved.
 *
 * This file is part of the Arduino Template Parser Library.
 *
 * Arduino Template Parser Library is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Arduino Template Parser Library is distributed in the hope that it will
 * be useful but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arduino Template Parser Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TEMPLATEPARSER_H_
#define TEMPLATEPARSER_H_

#include <SD.h>
#include <Print.h>

class TemplateParser
{
public:
    TemplateParser();
    int16_t processTemplateUntilNextMatch(File* templateFile,
                                          const char* const *templateStrings,
                                          uint8_t nrOfStrings, Print* p);

    void processSingleTemplate(File* templateFile,
                               const char* const *templateStrings,
                               const char* const * replacementStrings,
                               uint8_t nrOfStrings, Print* p);

    void selectList(const char* selectName, const char* const * names,
                    const char* const * values, uint8_t selected, uint8_t size,
                    Print* p, uint8_t multiple = 0,
                    uint8_t* multiple_select = 0, uint8_t multiple_size = 0);
    void selectListOption(const char* name, const char* value, uint8_t selected,
                          Print* p);

private:
    uint16_t getMax(uint16_t* arr, uint8_t s);
};

#endif /* TEMPLATEPARSER_H_ */
