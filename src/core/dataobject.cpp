/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of DataObject class
 */

#include "dataobject.h"

namespace QRS
{
template class DataObject<ScalarDataType>;
template class DataObject<VectorDataType>;
template class DataObject<MatrixDataType>;
}

using namespace QRS;

template <typename T>
uint DataObject<T>::smID = 0;

//! Retrieve of the last created object
template <typename T>
uint DataObject<T>::getID()
{
    return DataObject<T>::smID;
}

//! Base constructor
template <typename T>
DataObject<T>::DataObject(const QString& name)
{
    ++smID;
    mName = name;
}

/*!
 * \brief Add a data entity
 * \param[in] keyParameter \f$ \xi \f$ is the leading variable
 * \param[in] entity It may be scalar, vector, matrix or surface
 */
template <typename T>
void DataObject<T>::addEntity(double keyParameter, const T& entity)
{
    mEntities.insert({keyParameter, entity});
}

//! Remove a data entity with the specified key
template <typename T>
void DataObject<T>::removeEntity(double keyParameter)
{
    mEntities.erase(keyParameter);
}

//! Modify a data entity with the specified key
template <typename T>
void DataObject<T>::modifyEntity(double keyParameter, const T& entity)
{
    mEntities[keyParameter] = entity;
}

