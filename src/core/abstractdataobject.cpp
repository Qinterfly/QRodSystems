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

//! Get a number of created data objects
uint AbstractDataObject::numberObjects()
{
    return smNumObjects;
}

//! Modify a key existed
bool AbstractDataObject::changeItemKey(DataKeyType oldKey, DataKeyType newKey)
{
    // If the table does not contain the old key or the new one is already presented
    if (mItems.find(oldKey) == mItems.end() || mItems.find(newKey) != mItems.end())
        return false;
    DataItemType obj = mItems[oldKey];
    mItems.erase(oldKey);
    mItems.emplace(newKey, std::move(obj));
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
DataValueType AbstractDataObject::getAvailableItemKey(DataValueType key) const
{
    static const double kMultLastKey = 1.05;
    static const double kEpsilon = std::numeric_limits<double>::epsilon();
    // Check if a set containes an item with the specified key
    auto currentIterator = mItems.find(key);
    if (currentIterator == mItems.end())
    {
        return key;
    }
    else
    {
        auto nextIterator = currentIterator;
        ++nextIterator;
        // Whether a multiplied value of a last found key or a mean value is returned
        if (nextIterator == mItems.end())
        {
            double lastKey = mItems.rbegin()->first;
            return qAbs(lastKey) <= kEpsilon ? 1.0 : lastKey * kMultLastKey;
        }
        else
        {
            return (nextIterator->first + currentIterator->first) / 2.0;
        }
    }
}


