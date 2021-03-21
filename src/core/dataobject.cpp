/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of DataObject class
 */

#include "dataobject.h"

using namespace QRS;

template <typename T>
uint DataObject<T>::smID = 0;

template <typename T>
DataObject<T>::DataObject(QString name)
    : mName(name)
{
    ++smID;
}

//! Add a data entity
template <typename T>
void DataObject<T>::addEntity(double key, const T& entity)
{
    mEntities.insert(key, entity);
}
