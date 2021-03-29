/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the AbstractDataObject class
 */

#include "abstractdataobject.h"

using namespace QRS;

uint AbstractDataObject::smNumObjects = 0;

//! Base constructor
AbstractDataObject::AbstractDataObject(DataObjectType type, QString const& name)
    : mType(type)
    , mName(name)
{
    ++smNumObjects;
    mID = smNumObjects;
}

//! Modify a key existed
bool AbstractDataObject::changeItemKey(DataKeyType oldKey, DataKeyType newKey, DataHolder* items)
{
    if (!items)
        items = &mItems;
    // If the table does not contain the old key or the new one is already presented
    if (items->find(oldKey) == items->end() || items->find(newKey) != items->end())
        return false;
    DataItemType obj = items->at(oldKey);
    items->erase(oldKey);
    items->emplace(newKey, std::move(obj));
    return true;
}

//! Remove an entity with the specified key
void AbstractDataObject::removeItem(DataKeyType key)
{
    mItems.erase(key);
}

//! Set an array value with the specified indices
bool AbstractDataObject::setArrayValue(DataKeyType key, DataValueType newValue, uint iRow, uint iColumn)
{
    if (mItems.find(key) == mItems.end())
        return false;
    DataItemType& array = mItems.at(key);
    array[iRow][iColumn] = newValue;
    return true;
}

//! Check if a given key is unique
//! \return Returns the input value of the key if it is unique, otherwise -- a first available key
DataValueType AbstractDataObject::getAvailableItemKey(DataValueType key, DataHolder const* items) const
{
    if (!items)
        items = &mItems;
    static const double kMultLastKey = 1.05;
    static const double kEpsilon = std::numeric_limits<double>::epsilon();
    // Check if a set containes an item with the specified key
    auto currentIterator = items->find(key);
    if (currentIterator == items->end())
    {
        return key;
    }
    else
    {
        auto nextIterator = currentIterator;
        ++nextIterator;
        // Whether a multiplied value of a last found key or a mean value is returned
        if (nextIterator == items->end())
        {
            double lastKey = items->rbegin()->first;
            return qAbs(lastKey) <= kEpsilon ? 1.0 : lastKey * kMultLastKey;
        }
        else
        {
            return (nextIterator->first + currentIterator->first) / 2.0;
        }
    }
}

//! Serialize an abstract data object
void AbstractDataObject::serialize(QDataStream& stream) const
{
    stream << (quint32)mType;
    stream << mName;
    stream << (quint32)mID;
    stream << (quint32)mItems.size();
    for (auto& item : mItems)
    {
        stream << item.first;
        stream << item.second;
    }
}

/*! /brief Partly deserialize an abstract data object
 * It is assumed that a type and name have already been assigned.
 * So, only an identifier and items need to be set
 */
void AbstractDataObject::deserialize(QDataStream& stream)
{
    mItems.clear();
    quint32 numItems;
    DataKeyType key;
    stream >> mID;
    stream >> numItems;
    for (quint32 i = 0; i != numItems; ++i)
    {
        DataItemType dataItem;
        stream >> key;
        stream >> dataItem;
        mItems.emplace(key, dataItem);
    }
}
