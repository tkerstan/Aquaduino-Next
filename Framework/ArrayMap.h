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

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * \brief Map implementation using a fixed array
 *
 * Simple map implementation. Keys are returned when adding the element to
 * the map. The value is then accessible by this key. Also provides an iterator
 * over the map.
 */
template<class T>
class ArrayMap
{
private:
    T* m_Array;
    int8_t m_Size;
    int8_t m_Current;
    int8_t m_Last;
    ArrayMap();
    ArrayMap(const ArrayMap<T>& m);

protected:
    int8_t findHead();

public:
    ArrayMap(const int8_t size);
    virtual ~ArrayMap();

    int8_t add(const T e);
    int8_t set(int8_t idx, const T e);
    int8_t remove(const T e);
    int8_t findElement(const T e);
    int8_t getNrOfElements();

    int8_t getNext(T* result);
    void resetIterator();

    T get(const int8_t index);
    T& operator[](const int nIndex);
};

/**
 * \brief Default constructor
 *
 * Private & Empty.
 */
template<class T>
ArrayMap<T>::ArrayMap() :
        m_Array(NULL), m_Size(0), m_Current(0), m_Last(0)
{
}

/**
 * \brief Copy constructor
 *
 * Private & Empty.
 */
template<class T>
ArrayMap<T>::ArrayMap(const ArrayMap<T>& m)
{
}

/**
 * \brief Constructor
 * \param[in] size Number of elements the map can hold.
 *
 * Allocates the map using malloc and initializes it to zero.
 */
template<class T>
ArrayMap<T>::ArrayMap(const int8_t size) :
        m_Size(size), m_Current(0), m_Last(0)
{
    m_Array = (T*) malloc(sizeof(T) * size);
    memset(m_Array, 0, sizeof(T) * size);
}

/**
 * \brief Destructor
 *
 * Deletes the array allocated by malloc.
 */
template<class T>
ArrayMap<T>::~ArrayMap()
{
    delete m_Array;
}

/*
 * \brief Finds the first non empty element in the array.
 *
 * \returns Index if the first non empty element in the array.
 */
template<class T>
int8_t ArrayMap<T>::findHead()
{
    int8_t i = 0;
    for (; i < m_Size; i++)
        if (m_Array[i] != NULL)
        {
            return i;
        }
    return -1;
}

/**
 * \brief Adds an element to the map.
 *
 * \returns Index of the element in the array.
 */
template<class T>
int8_t ArrayMap<T>::add(const T e)
{
    int8_t i = 0;
    int8_t idx = m_Last;

    for (; i < m_Size; i++)
    {
        if (m_Array[idx] == NULL)
        {
            m_Array[idx] = e;
            return idx;
        }
        idx++;
        if (idx >= m_Size)
            idx = 0;
    }

    return -1;
}

/**
 * \brief Adds an element to the map.
 *
 * \returns Index of the element in the array.
 */
template<class T>
int8_t ArrayMap<T>::set(int8_t idx, const T e)
{
    if (m_Array[idx] == NULL)
    {
        m_Array[idx] = e;
        return idx;
    }

    return -1;
}

/**
 * \brief Deletes an element from the map.
 *
 * \returns The index where the element was stored. -1 when the element did
 * not exist in the map.
 */
template<class T>
int8_t ArrayMap<T>::remove(const T e)
{
    int8_t pos = findElement(e);

    if (pos >= 0)
    {
        m_Array[pos] = NULL;
        m_Last = pos;
        return pos;

    }
    return -1;
}

/*
 * \brief Finds an element in the map.
 *
 * \returns Index of the element in the map. -1 if it is not found.
 */
template<class T>
int8_t ArrayMap<T>::findElement(const T e)
{
    int8_t i = 0;

    for (; i < m_Size; i++)
    {
        if (m_Array[i] == e)
            return i;
    }

    return -1;
}

/**
 * \brief Gets the number of stored elements in the map.
 *
 * \return The number of elements stored in the map.
 */
template<class T>
int8_t ArrayMap<T>::getNrOfElements()
{
    int8_t i = 0;
    int8_t elements = 0;

    for (; i < m_Size; i++)
    {
        if (m_Array[i] != NULL)
            elements++;
    }

    return elements;
}

/**
 * \brief Gets the next element using the internal iterator.
 *
 * \returns Next element in the map.
 */
template<class T>
int8_t ArrayMap<T>::getNext(T* result)
{
    for (; m_Current >= 0 && m_Current < m_Size; m_Current++)
    {
        if (m_Array[m_Current] != NULL)
        {
            *result = m_Array[m_Current];
            return m_Current++;
        }
    }

    *result = NULL;
    return -1;
}

/**
 * \brief Resets the internal iterator.
 *
 * Resets the internal iterator to the first element in the map.
 */
template<class T>
void ArrayMap<T>::resetIterator()
{
    m_Current = findHead();
}

/**
 * \brief Gets an element from the map.
 * \param index Index of the element to get.
 *
 * \returns The element at the specified index.
 */
template<class T>
T ArrayMap<T>::get(const int8_t index)
{
    if (0 <= index && index < m_Size)
        return m_Array[index];
    return NULL;
}

/**
 * \brief Gets an element from the map.
 * \param nIndex Index of the element to get.
 *
 * \returns The element at the specified index.
 */
template<class T>
T& ArrayMap<T>::operator[](const int nIndex)
{
    if (0 <= nIndex && nIndex < m_Size)
        return m_Array[nIndex];
    if ( nIndex < 0)
        return m_Array[0];
    if ( nIndex >= m_Size )
        return m_Array[m_Size-1];
    return m_Array[0];
}

#endif /* ARRAYLIST_H_ */
