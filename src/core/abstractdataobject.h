/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the AbstractDataObject class
 */

#ifndef ABSTRACTDATAOBJECT_H
#define ABSTRACTDATAOBJECT_H

#include <QString>
#include <QDataStream>
#include <unordered_map>
#include "array.h"
#include "datatypes.h"

namespace QRS::Core
{

using DataItemType = Array<DataValueType>;
using DataHolder = std::map<DataKeyType, DataItemType>;

//! Data object which is designied in the way to be represented in a table easily
class AbstractDataObject
{
public:
    AbstractDataObject(DataObjectType type, QString const& name);
    virtual ~AbstractDataObject() = 0;
    virtual AbstractDataObject* clone() const = 0;
    virtual DataItemType& addItem(DataKeyType key) = 0;
    void removeItem(DataValueType key);
    bool changeItemKey(DataKeyType oldKey, DataKeyType newKey, DataHolder* items = nullptr);
    DataValueType getAvailableItemKey(DataValueType key, DataHolder const* items = nullptr) const;
    bool setArrayValue(DataKeyType key, DataValueType newValue, quint32 iRow = 0, quint32 iColumn = 0);
    quint32 numberItems() const { return mItems.size(); }
    DataHolder& getItems() { return mItems; }
    DataItemType& getItem(DataValueType keyParameter) { return mItems.at(keyParameter); }
    DataIDType id() const { return mID; }
    DataObjectType type() const { return mType; }
    QString const& name() const { return mName; }
    void setName(QString const& name) { mName = name; }
    static quint32 numberObjects() { return smNumObjects; }
    static void setNumberObjects(quint32 numObjects) { smNumObjects = numObjects; }
    virtual void serialize(QDataStream& stream) const;
    virtual void deserialize(QDataStream& stream);
    friend QDataStream& operator<<(QDataStream& stream, AbstractDataObject const& obj);
    friend QDataStream& operator>>(QDataStream& stream, AbstractDataObject& obj);
    virtual void import(QTextStream& stream) = 0;

protected:
    //! Object type
    const DataObjectType mType;
    //! Name of an object
    QString mName;
    //! Unique object identificator
    DataIDType mID;
    //! Map contains all created entities
    DataHolder mItems;

private:
    //! Number of all objects created
    static quint32 smNumObjects;
};

//! Print a data object to a stream
inline QDataStream& operator<<(QDataStream& stream, AbstractDataObject const& obj)
{
    obj.serialize(stream);
    return stream;
}

//! Read a data object from a stream
inline QDataStream& operator>>(QDataStream& stream, AbstractDataObject& obj)
{
    obj.deserialize(stream);
    return stream;
}

}

#endif // ABSTRACTDATAOBJECT_H
