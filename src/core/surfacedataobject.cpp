/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the SurfaceDataObject class
 */

#include "surfacedataobject.h"

using namespace QRS;

uint SurfaceDataObject::smNumInstances = 0;

//! Construct a surface data object
SurfaceDataObject::SurfaceDataObject(QString const& name)
    : AbstractDataObject(DataObjectType::kSurface, name)
{
    ++smNumInstances;
    mLeadingItems.emplace(0.0, Array<double>());
    mLeadingItems.emplace(1.0, Array<double>());
}

//! Insert a new item into SurfaceDataObject
DataItemType& SurfaceDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(1, mLeadingItems.size()));
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

//! Add a leading item
DataKeyType SurfaceDataObject::addLeadingItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key, &mLeadingItems);
    mLeadingItems.emplace(rightKey, Array<double>());
    uint numLeadingItems = mLeadingItems.size();
    for (auto& item : mItems)
        item.second.resize(1, numLeadingItems);
    return rightKey;
}

//! Remove a leading item
void SurfaceDataObject::removeLeadingItem(DataValueType key)
{
    if (mLeadingItems.size() == 1)
        return;
    int iColumn = std::distance(mLeadingItems.begin(), mLeadingItems.find(key));
    for (auto& item : mItems)
        item.second.removeColumn(iColumn);
    mLeadingItems.erase(key);
}

//! Modify a leading item key
bool SurfaceDataObject::changeLeadingItemKey(DataKeyType oldKey, DataKeyType newKey)
{
    int iOldColumn = std::distance(mLeadingItems.begin(), mLeadingItems.find(oldKey));
    bool isOkay = changeItemKey(oldKey, newKey, &mLeadingItems);
    if (isOkay)
    {
        int iNewColumn = std::distance(mLeadingItems.begin(), mLeadingItems.find(newKey));
        for (auto& item : mItems)
            item.second.swapColumns(iOldColumn, iNewColumn);
    }
    return isOkay;
}

//! Serialize additional data of a surface object
void SurfaceDataObject::serialize(QDataStream& stream) const
{
    AbstractDataObject::serialize(stream);
    stream << (quint32)mLeadingItems.size();
    for (auto& item : mLeadingItems)
    {
        stream << item.first;
        stream << item.second;
    }
}

//! Deserialize additional data of a surface object
void SurfaceDataObject::deserialize(QDataStream& stream)
{
    AbstractDataObject::deserialize(stream);
    mLeadingItems.clear();
    quint32 numLeadingItems;
    DataKeyType key;
    stream >> numLeadingItems;
    for (quint32 i = 0; i != numLeadingItems; ++i)
    {
        DataItemType leadingDataItem;
        stream >> key;
        stream >> leadingDataItem;
        mLeadingItems.emplace(key, leadingDataItem);
    }
}

