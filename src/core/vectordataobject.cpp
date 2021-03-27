/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the VectorDataObject class
 */

#include "vectordataobject.h"

using namespace QRS;

uint VectorDataObject::smNumVectors = 0;

//! Construct a vector data object
VectorDataObject::VectorDataObject(QString const& name)
    : AbstractDataObject(DataObjectType::kVector, name)
{
    ++smNumVectors;
}

//! Get a number of created vectors
uint VectorDataObject::numberVectors()
{
    return smNumVectors;
}

//! Insert a new item into VectorDataObject
DataItemType& VectorDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(1, 3));
    return mItems.at(rightKey);
}

//! Clone a vector data object
AbstractDataObject* VectorDataObject::clone() const
{
    VectorDataObject* obj = new VectorDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    return obj;
}
