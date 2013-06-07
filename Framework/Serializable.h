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

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <Arduino.h>

/**
 * \brief Interface for serialization methods.
 *
 * This interface provides the serialization methods to be used for storing
 * and restoring the configuration of objects.
 */
class Serializable
{
public:
    /**
     * \brief Callback for serialization of an object.
     * \param[out] buffer The buffer where the serialized data is going to be
     *                    stored.
     * \param[in] size The size of the buffer.
     *
     * This method needs to be implemented to serialize the desired data of the
     * class providing this interface.
     *
     * \returns Number of bytes serialized.
     */
    virtual uint16_t serialize(void* buffer, uint16_t size) = 0;

   /**
     * \brief Callback for deserialization of an object.
     * \param[out] data The buffer where the serialized data is going to be
     *                    stored.
     * \param[in] size The size of the buffer.
     *
     * This method needs to be implemented to deserialize the desired data of
     * the class providing this interface.
     *
     * \returns Number of bytes deserialized.
     */
    virtual uint16_t deserialize(void* data, uint16_t size) = 0;
};

#endif /* SERIALIZABLE_H_ */
