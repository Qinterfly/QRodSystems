/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the ScalarDataObject class
 */

#include "scalardataobject.h"

using namespace QRS;

uint ScalarDataObject::smNumInstances = 0;

//! Construct a scalar data object
ScalarDataObject::ScalarDataObject(QString const& name)
    : AbstractDataObject(DataObjectType::kScalar, name)
{
    ++smNumInstances;
}

//! Get a number of created scalars
uint ScalarDataObject::numberInstances()
{
    return smNumInstances;
}

//! Insert a new item into ScalarDataObject
DataItemType& ScalarDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(1, 1));
    return mItems.at(rightKey);
}

//! Clone a scalar data object
AbstractDataObject* ScalarDataObject::clone() const
{
    ScalarDataObject* obj = new ScalarDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    return obj;
}
