/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of ScalarDataObject class
 */

#include "scalardataobject.h"

using namespace QRS;

uint ScalarDataObject::smNumScalars = 0;

//! Construct a scalar data object
ScalarDataObject::ScalarDataObject(const QString& name)
    : AbstractDataObject(DataObjectType::kScalar, name)
{
    ++smNumScalars;
}

//! Get a number of created scalars
uint ScalarDataObject::numberScalars()
{
    return smNumScalars;
}

//! Insert a new item into ScalarDataObject
void ScalarDataObject::addItem(DataValueType keyParameter)
{
    mItems.emplace(keyParameter, DataItemType(1, 1));
}

//! Clone a scalar data object
AbstractDataObject* ScalarDataObject::clone() const
{
    ScalarDataObject* obj = new ScalarDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    return obj;
}

