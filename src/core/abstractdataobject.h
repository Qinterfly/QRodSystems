/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the AbstractDataObject class
 */

#ifndef ABSTRACTDATAOBJECT_H
#define ABSTRACTDATAOBJECT_H

#include <QString>
#include <unordered_map>
#include "array.h"
#include "datatypes.h"

namespace QRS
{

using DataItemType = Array<DataValueType>;
using DataHolder = std::map<DataKeyType, DataItemType>;


//! Data object which is designied in the way to be represented in a table easily
class AbstractDataObject
{
public:
    AbstractDataObject(DataObjectType type, QString const& name);
    virtual ~AbstractDataObject() = default;
    virtual AbstractDataObject* clone() const = 0;
    virtual DataItemType& addItem(DataKeyType key) = 0;
    void removeItem(DataValueType key);
    bool changeItemKey(DataKeyType oldKey, DataKeyType newKey, DataHolder* items = nullptr);
    DataValueType getAvailableItemKey(DataValueType key, DataHolder const* items = nullptr) const;
    bool setArrayValue(DataKeyType key, DataValueType newValue, uint iRow = 0, uint iColumn = 0);
    DataHolder& getItems() { return mItems; }
    DataItemType& getItem(DataValueType keyParameter) { return mItems.at(keyParameter); }
    DataIDType id() const { return mID; }
    DataObjectType type() const { return mType; }
    void setName(QString const& name) { mName = name; }
    static uint numberObjects();

protected:
    const DataObjectType mType;
    QString mName;
    DataHolder mItems;
    DataIDType mID;

private:
    static uint smNumObjects;
};

}

#endif // ABSTRACTDATAOBJECT_H
