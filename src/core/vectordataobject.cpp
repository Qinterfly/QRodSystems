/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Implementation of the VectorDataObject class
 */

#include "vectordataobject.h"

using namespace QRS::Core;

quint32 VectorDataObject::smNumInstances = 0;
const IndexType skNumElements = 3;

//! Construct a vector data object
VectorDataObject::VectorDataObject(QString const& name)
    : AbstractDataObject(kVector, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
VectorDataObject::~VectorDataObject()
{
    --smNumInstances;
}

//! Insert a new item into VectorDataObject
DataItemType& VectorDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(1, skNumElements));
    return mItems.at(rightKey);
}

//! Clone a vector data object
AbstractDataObject* VectorDataObject::clone() const
{
    VectorDataObject* obj = new VectorDataObject(mName);
    obj->mItems = mItems;
    obj->mID = mID;
    --smNumInstances;
    return obj;
}

//! Import a vector data object from a file
void VectorDataObject::import(QTextStream& stream)
{
    mItems.clear();
    quint32 numItems;
    stream >> numItems;
    stream.readLine();
    double key;
    for (quint32 iItem = 0; iItem != numItems; ++iItem)
    {
        stream >> key;
        DataItemType& item = addItem(key);
        for (IndexType j = 0; j != skNumElements; ++j)
            stream >> item[0][j];
    }
}
