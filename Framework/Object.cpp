/*
 * Object.cpp
 *
 *  Created on: 04.03.2013
 *      Author: TimoK
 */

#include "Object.h"
#include "string.h"

Object::Object()
{
}

Object::Object(const char* name, const char* url)
{
	setName(name);
	setURL(url);
	m_Type = 0;
}

Object::~Object()
{
}

void Object::setName(const char* name)
{
	strncpy(m_Name, name, sizeof(m_Name));
	m_Name[sizeof(m_Name) - 1] = 0;
}

const char* Object::getName()
{
	return m_Name;
}

void Object::setURL(const char* url)
{
	strncpy(m_URL, url, sizeof(m_URL));
	m_URL[sizeof(m_URL) - 1] = 0;
}

const char* Object::getURL()
{
	return m_URL;
}

int16_t Object::getType()
{
	return m_Type;
}
