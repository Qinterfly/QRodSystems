/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the AbstractDataObject class
 */

#ifndef ABSTRACTDATAOBJECT_H
#define ABSTRACTDATAOBJECT_H

#include <QObject>
#include <QString>
#include <QDataStream>
#include <unordered_map>
#include "array.h"
#include "aliasdata.h"

namespace QRS::Core
{

using DataItemType = Array<DataValueType>;
using DataHolder = std::map<DataKeyType, DataItemType>;

//! Data object which is designied in the way to be represented in a table easily
class AbstractDataObject : public QObject
{
    Q_OBJECT

public:
    enum ObjectType
    {
        kScalar,
        kVector,
        kMatrix,
        kSurface
    };
    AbstractDataObject(ObjectType type, QString const& name);
    virtual ~AbstractDataObject() = 0;
    virtual AbstractDataObject* clone() const = 0;
    virtual DataItemType& addItem(DataKeyType key) = 0;
    void removeItem(DataValueType key);
    bool changeItemKey(DataKeyType oldKey, DataKeyType newKey, DataHolder* items = nullptr);
    DataValueType getAvailableItemKey(DataValueType key, DataHolder const* items = nullptr) const;
    bool setArrayValue(DataKeyType key, DataValueType newValue, quint32 iRow = 0, quint32 iColumn = 0);
    quint32 numberItems() const { return mItems.size(); }
    DataHolder const& getItems() { return mItems; }
    DataIDType id() const { return mID; }
    ObjectType type() const { return mType; }
    QString const& name() const { return mName; }
    void setName(QString const& name) { mName = name; }
    static quint32 maxObjectID() { return smMaxObjectID; }
    static void setMaxObjectID(quint32 iMaxObjectID) { smMaxObjectID = iMaxObjectID; }
    virtual void serialize(QDataStream& stream) const;
    virtual void deserialize(QDataStream& stream);
    friend QDataStream& operator<<(QDataStream& stream, AbstractDataObject const& obj);
    virtual void import(QTextStream& stream) = 0;

protected:
    const ObjectType mType;
    QString mName;
    DataIDType mID;
    DataHolder mItems;

private:
    static quint32 smMaxObjectID;
};

//! Print a data object to a stream
inline QDataStream& operator<<(QDataStream& stream, AbstractDataObject const& obj)
{
    obj.serialize(stream);
    return stream;
}

}

#endif // ABSTRACTDATAOBJECT_H
