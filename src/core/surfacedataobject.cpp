/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the SurfaceDataObject class
 */

#include "surfacedataobject.h"

using namespace QRS;

uint SurfaceDataObject::smNumSurfaces = 0;

//! Construct a surface data object
SurfaceDataObject::SurfaceDataObject(QString const& name)
    : AbstractDataObject(DataObjectType::kScalar, name)
{
    ++smNumSurfaces;
}

//! Get a number of created surfaces
uint SurfaceDataObject::numberSurfaces()
{
    return smNumSurfaces;
}

//! Insert a new item into SurfaceDataObject
DataItemType& SurfaceDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(1, 2));
    return mItems.at(rightKey);
}

//! Clone a surface data object
AbstractDataObject* SurfaceDataObject::clone() const
{
    SurfaceDataObject* obj = new SurfaceDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    return obj;
}
