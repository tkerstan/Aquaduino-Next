/*
 * Serializable.h
 *
 *  Created on: 04.03.2013
 *      Author: TimoK
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <Arduino.h>

class Serializable {
public:
	virtual uint16_t serialize(void* buffer, uint16_t size) = 0;
	virtual uint16_t deserialize(void* data, uint16_t size) = 0;
};

#endif /* SERIALIZABLE_H_ */
