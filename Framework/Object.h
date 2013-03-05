/*
 * Object.h
 *
 *  Created on: 04.03.2013
 *      Author: TimoK
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <Framework/Config.h>
#include <Framework/ObjectTypes.h>

class Object {
public:
	Object();
	Object(const char* name, const char* url);

	void setName(const char* name);
	const char* getName();

	void setURL(const char* url);
	const char* getURL();

	virtual int16_t getType();

protected:
	virtual ~Object();
	char m_Name[AQUADUINO_STRING_LENGTH];
	char m_URL[AQUADUINO_STRING_LENGTH];
	int16_t m_Type;
};

#endif /* OBJECT_H_ */
