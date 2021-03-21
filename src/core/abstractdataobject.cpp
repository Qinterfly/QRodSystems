/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of AbstractDataObject class
 */

#include "abstractdataobject.h"

using namespace QRS;

uint AbstractDataObject::smNumObjects = 0;

//! Base constructor
AbstractDataObject::AbstractDataObject(DataObjectType type, const QString& name)
    : mType(type)
    , mName(name)
{
    ++smNumObjects;
    mID = smNumObjects;
}

//! Get a number of created data objects
uint AbstractDataObject::numberObjects()
{
    return smNumObjects;
}

//! Remove an entity with the specified key
void AbstractDataObject::removeItem(DataKeyType keyParameter)
{
    mItems.erase(keyParameter);
}


//! Construct a scalar data object
ScalarDataObject::ScalarDataObject(const QString& name)
    : AbstractDataObject(DataObjectType::kScalar, name)
{

}

//! Construct a vector data object
VectorDataObject::VectorDataObject(const QString& name)
    : AbstractDataObject(DataObjectType::kVector, name)
{

}

//! Construct a matrix data object
MatrixDataObject::MatrixDataObject(const QString& name)
    : AbstractDataObject(DataObjectType::kMatrix, name)
{

}

//! Construct a surface data object
SurfaceDataObject::SurfaceDataObject(const QString& name)
    : AbstractDataObject(DataObjectType::kSurface, name)
{

}

//! Insert a new item into ScalarDataObject
void ScalarDataObject::addItem(DataValueType keyParameter)
{
    mItems.emplace(keyParameter, DataItemType(1, 1));
}

//! Insert a new item into VectorDataObject
void VectorDataObject::addItem(DataValueType keyParameter)
{
    mItems.emplace(keyParameter, DataItemType(1, 3));
}

//! Insert a new item into MatrixDataObject
void MatrixDataObject::addItem(DataValueType keyParameter)
{
    mItems.emplace(keyParameter, DataItemType(3, 3));
}

//! Insert a new item into SurfaceDataObject
void SurfaceDataObject::addItem(DataValueType keyParameter)
{
    mItems.emplace(keyParameter, DataItemType(1, 2));
}

