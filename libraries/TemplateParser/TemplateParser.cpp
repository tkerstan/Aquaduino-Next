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

#include "TemplateParser.h"

#define pgm_read_array_byte(array,i,j) pgm_read_byte(&((char*) pgm_read_word(&array[i]))[j])

const static uint16_t bufferSize = 50;

const static char progSelectTagOpen[] PROGMEM = "<select name=\"";
const static char progSelectMultiple[] PROGMEM = "multiple";
const static char progOptionTagOpen[] PROGMEM = "<option";
const static char progOptionTagSelected[] PROGMEM = " selected";
const static char progOptionTagValueOpen[] PROGMEM = " value=\"";
const static char progOptionTagValueClose[] PROGMEM = "\">";
const static char progOptionTagClose[] PROGMEM = "</option>\n";

TemplateParser::TemplateParser()
{
}

/*
 * Determine the maximum of an array
 */
uint16_t TemplateParser::getMax(uint16_t* arr, uint8_t s)
{
    uint16_t max = 0;
    uint8_t i = 0;

    for (i = 0; i < s; i++)
    {
        max = max(arr[i], max);
    }

    return max;
}

/*
 * Search templatefile for occurences of templateStrings.
 * Print contents up to the position of the occurence to p.
 *
 * IMPORTANT: Do not seek within the file while parsing templates,
 * because the parser functions rely on the positions they leave
 * behind.
 */
int16_t TemplateParser::processTemplateUntilNextMatch(
        File* templateFile, const char* const *templateStrings,
        uint8_t nrOfStrings, Print* p)
{
    uint8_t buffer[2][bufferSize];
    uint8_t curBuffer = 0;
    uint8_t nextBuffer = 1;
    uint16_t sPos[nrOfStrings];
    uint16_t filePos;
    uint8_t i, j;
    int read;
    uint16_t oldPending = 0;
    uint16_t maxPending = 0;



    //Initialize array for keeping track of the current match positions
    //within the templateStrings
    for (i = 0; i < nrOfStrings; i++)
    {
        sPos[i] = 0;
    }

    //Where are we now?
    filePos = templateFile->position();

    //Lets read to the current buffer.
    //Skip the first maxPending bytes in the buffer when there is a match pending
    //from a previous run. This happens when a template string is read partially
    //into the buffer.
    while ((read = templateFile->read(&buffer[curBuffer][maxPending],
                                      bufferSize - maxPending))
           > 0)
    {
        //Start from maxPending, because we have processed these bytes one run before.
        for (i = maxPending; i < read; i++)
        {
            for (j = 0; j < nrOfStrings; j++)
            {
                //Does the current byte match a templateString at its currently stored position?
                if (buffer[curBuffer][i] == pgm_read_array_byte(templateStrings,j,sPos[j]))
                {
                    //Ok it does
                    sPos[j] += 1;

                    //Is the next byte to match the string terminator?
                    if (pgm_read_array_byte(templateStrings,j,sPos[j]) == '\0')
                    {
                        //Ok we have a match
                        //Print buffer to p up to 1 byte before the match started.
                        p->write(buffer[curBuffer],
                                 i - strlen_P((char*) pgm_read_word(&templateStrings[j])) + 1);
                        templateFile->seek(filePos + i + 1 - maxPending);
                        return j;
                    }
                }
                else
                {
                    //It doesn't match. Start matching from the beginning.
                    sPos[j] = 0;
                }
            }
        }

        //Is there something pending from the previous run we did not print to p?
        oldPending = maxPending;

        //Is there something pending now what we are not allowed to print to p now?
        //We need the maximum!
        maxPending = getMax(sPos, nrOfStrings);

        //Print buffer containing old Pending bytes and the bytes we read in this run
        //without the currently pending maximum match
        p->write(buffer[curBuffer], oldPending + read - maxPending);

        if (maxPending)
        {
            //Ok there is something pending now. Copy it to the beginning of the next buffer.
            memcpy(buffer[nextBuffer],
                   &buffer[curBuffer][read - maxPending],
                   maxPending);
        }

        //Switch Buffers
        curBuffer = nextBuffer;
        nextBuffer = (nextBuffer + 1) & 0x1;

        //Keep track of position in file.
        filePos += read;
    }

    return -1;

}

void TemplateParser::processSingleTemplate(
        File* templateFile, const char* const *templateStrings,
        const char* const * replacementStrings, uint8_t nrOfStrings, Print* p)
{
    int16_t matchIdx = 0;

    templateFile->seek(0);

    while ((matchIdx = processTemplateUntilNextMatch(templateFile,
                                                     templateStrings,
                                                     nrOfStrings,
                                                     p))
           != -1)
    {
        if (replacementStrings[matchIdx] != NULL)
        {
            p->print(replacementStrings[matchIdx]);
        }
    }

}

void TemplateParser::selectList(const char* selectName,
                                const char* const * names,
                                const char* const * values,
                                uint8_t selected,
                                uint8_t size,
                                Print* p,
                                uint8_t multiple,
                                uint8_t* multiple_select,
                                uint8_t multiple_size)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t s = 0;

    p->print((__FlashStringHelper*) progSelectTagOpen);
    p->print(selectName);
    p->print("\" ");
    if (multiple)
    {
        p->print("size=\"");
        p->print(size);
        p->print("\" ");
        p->print((__FlashStringHelper*) progSelectMultiple);
    }
    p->print(">");

    for (; i < size; i++)
    {
        s = 0;
        if (multiple)
        {
            for (j = 0; j < multiple_size; j++)
                if (multiple_select[j] == i)
                {
                    selectListOption(names[i], values[i], 1, p);
                    s = 1;
                    break;
                }
            if (!s)
                selectListOption(names[i], values[i], 0, p);
        }
        else
        {
            if (i == selected)
                selectListOption(names[i], values[i], 1, p);
            else
                selectListOption(names[i], values[i], 0, p);
        }
    }
}

void TemplateParser::selectListOption(const char* name, const char* value,
                                      uint8_t selected, Print* p)
{
    char optionTagOpen[sizeof(progOptionTagOpen)];
    char optionTagSelected[sizeof(progOptionTagSelected)];
    char optionTagValueOpen[sizeof(progOptionTagValueOpen)];
    char optionTagValueClose[sizeof(progOptionTagValueClose)];
    char optionTagClose[sizeof(progOptionTagClose)];

    strcpy_P(optionTagOpen, progOptionTagOpen);
    strcpy_P(optionTagSelected, progOptionTagSelected);
    strcpy_P(optionTagValueOpen, progOptionTagValueOpen);
    strcpy_P(optionTagValueClose, progOptionTagValueClose);
    strcpy_P(optionTagClose, progOptionTagClose);

    p->print(optionTagOpen);
    if (selected)
    {
        p->print(optionTagSelected);
    }
    p->print(optionTagValueOpen);
    p->print(value);
    p->print(optionTagValueClose);
    p->print(name);
    p->print(optionTagClose);
}
