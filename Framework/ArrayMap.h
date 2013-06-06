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

template<class T>
class ArrayMap
{
private:
    T* m_Array;
    int8_t m_Size;
    int8_t m_Current;

protected:
    int8_t findHead();
    int8_t findTail();

public:
    ArrayMap(const int8_t size);
    virtual ~ArrayMap();

    int8_t add(const T e);
    int8_t remove(const T e);
    int8_t findElement(const T e);
    int8_t getNrOfElements();

    int8_t getCurrent(T* result);
    int8_t getNext(T* result);
    void resetIterator();
    void setIterator(T element);

    T get(const int8_t index);
    T& operator[](const int nIndex);
};

template<class T>
ArrayMap<T>::ArrayMap(const int8_t size) :
        m_Size(size), m_Current(0)
{
    m_Array = (T*) malloc(sizeof(T) * size);
    memset(m_Array, 0, sizeof(T) * size);
}

template<class T>
ArrayMap<T>::~ArrayMap()
{
    delete m_Array;
}

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

template<class T>
int8_t ArrayMap<T>::findTail()
{
    int8_t i = m_Size - 1;

    for (; i >= 0; i--)
        if (m_Array[i] != NULL)
        {
            return i;
        }
    return -1;
}

template<class T>
int8_t ArrayMap<T>::add(const T e)
{
    int8_t i = 0;

    for (; i < m_Size; i++)
    {
        if (m_Array[i] == NULL)
        {
            m_Array[i] = e;
            return i;
        }
    }

    return -1;
}

template<class T>
int8_t ArrayMap<T>::remove(const T e)
{
    int8_t pos = findElement(e);

    if (pos >= 0)
    {
        m_Array[pos] == NULL;
        return pos;

    }
    return -1;
}

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

template<class T>
int8_t ArrayMap<T>::getCurrent(T* result)
{
    *result = m_Array[m_Current];

    return m_Current;
}

template<class T>
int8_t ArrayMap<T>::getNext(T* result)
{
    for (; m_Current < m_Size; m_Current++)
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

template<class T>
void ArrayMap<T>::resetIterator()
{
    m_Current = findHead();
}

template<class T>
void ArrayMap<T>::setIterator(T element)
{
    m_Current = element;
}

template<class T>
T ArrayMap<T>::get(const int8_t index)
{
    if (0 <= index && index < m_Size)
        return m_Array[index];
    return NULL;
}

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
