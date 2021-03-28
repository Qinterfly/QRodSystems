/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the MatrixDataObject class
 */

#include "matrixdataobject.h"

using namespace QRS;

uint MatrixDataObject::smNumInstances = 0;

//! Construct a matrix data object
MatrixDataObject::MatrixDataObject(QString const& name)
    : AbstractDataObject(DataObjectType::kMatrix, name)
{
    ++smNumInstances;
}

//! Get a number of created matrices
uint MatrixDataObject::numberInstances()
{
    return smNumInstances;
}

//! Insert a new item into MatrixDataObject
DataItemType& MatrixDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(3, 3));
    return mItems.at(rightKey);
}

//! Clone a matrix data object
AbstractDataObject* MatrixDataObject::clone() const
{
    MatrixDataObject* obj = new MatrixDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    return obj;
}
