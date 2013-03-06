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
class ArrayList
{
private:
    T* m_Array;
    int8_t m_Size;
    int8_t m_Head;
    int8_t m_Current;
    int8_t m_Tail;

protected:
    int8_t findHead();
    int8_t findTail();

public:
    ArrayList(const int8_t size);
    virtual ~ArrayList();

    int8_t add(const T e);
    int8_t remove(const T e);
    int8_t findElement(const T e);
    int8_t getNrOfElements();

    int8_t getHead(T* result);
    int8_t getTail(T* result);
    int8_t getCurrent(T* result);
    int8_t getNext(T* result);
    void resetIterator();
    void setIterator(T element);

    T get(const int8_t index);
    T& operator[](const int nIndex);
};

template<class T>
ArrayList<T>::ArrayList(const int8_t size) :
        m_Size(size), m_Head(0), m_Current(0), m_Tail(0)
{
    m_Array = (T*) malloc(sizeof(T) * size);
    memset(m_Array, 0, sizeof(T) * size);
}

template<class T>
ArrayList<T>::~ArrayList()
{
    delete m_Array;
}

template<class T>
int8_t ArrayList<T>::findHead()
{
    int8_t i = 0;
    for (; i < m_Size; i++)
        if (m_Array[i] != NULL)
        {
            m_Head = i;
            return i;
        }
    m_Head = 0;
    return -1;
}

template<class T>
int8_t ArrayList<T>::findTail()
{
    int8_t i = m_Size - 1;

    for (; i >= 0; i--)
        if (m_Array[i] != NULL)
        {
            m_Tail = i;
            return i;
        }
    m_Tail = 0;
    return -1;
}

template<class T>
int8_t ArrayList<T>::add(const T e)
{
    int8_t i = 0;

    for (; i < m_Size; i++)
    {
        m_Tail = (m_Tail + i) % m_Size;
        if (m_Array[m_Tail] == NULL)
        {
            m_Array[m_Tail] = e;
            return m_Tail;
        }
    }

    return -1;
}

template<class T>
int8_t ArrayList<T>::remove(const T e)
{
    int8_t pos = findElement(e);

    if (pos >= 0)
    {
        m_Array[pos] == NULL;
        if (pos == m_Head)
            findHead();
        if (pos == m_Tail)
            findTail();
        return pos;

    }
    return -1;
}

template<class T>
int8_t ArrayList<T>::findElement(const T e)
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
int8_t ArrayList<T>::getNrOfElements()
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
int8_t ArrayList<T>::getHead(T* result)
{
    *result = m_Array[m_Head];

    return m_Head;
}

template<class T>
int8_t ArrayList<T>::getTail(T* result)
{
    *result = m_Array[m_Tail];

    return m_Tail;
}

template<class T>
int8_t ArrayList<T>::getCurrent(T* result)
{
    *result = m_Array[m_Current];

    return m_Current;
}

template<class T>
int8_t ArrayList<T>::getNext(T* result)
{
    for (m_Current = m_Current; m_Current < m_Size; m_Current++)
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
void ArrayList<T>::resetIterator()
{
    m_Current = m_Head;
}

template<class T>
void ArrayList<T>::setIterator(T element)
{
    m_Current = element;
}

template<class T>
T ArrayList<T>::get(const int8_t index)
{
    if (index < m_Size)
        return m_Array[index];
    return NULL;
}

template<class T>
T& ArrayList<T>::operator[](const int nIndex)
{
    return m_Array[nIndex];
}

#endif /* ARRAYLIST_H_ */
