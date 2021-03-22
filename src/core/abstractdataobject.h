/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of AbstractDataObject class
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


//! Data object which is designied in the way to be represented in a table easily
class AbstractDataObject
{
    using DataHolder = std::unordered_map<DataKeyType, DataItemType>;

public:
    AbstractDataObject(DataObjectType type, const QString& name);
    virtual ~AbstractDataObject() = default;
    virtual AbstractDataObject* clone() const = 0;
    virtual void addItem(DataValueType keyParameter) = 0;
    void removeItem(DataValueType keyParameter);
    DataIDType id() const { return mID; }
    DataObjectType type() const { return mType; }
    static uint numberObjects();

protected:
    const DataObjectType mType;
    QString mName;
    DataHolder mItems;
    DataIDType mID;

private:
    static uint smNumObjects;
};

// TODO

////! Vector data object
//class VectorDataObject : public AbstractDataObject
//{
//public:
//    VectorDataObject(const QString& name);
//    ~VectorDataObject() = default;
//    AbstractDataObject* clone() const override;
//    void addItem(DataValueType keyParameter) override;
//};

////! Matrix data object
//class MatrixDataObject : public AbstractDataObject
//{
//public:
//    MatrixDataObject(const QString& name);
//    ~MatrixDataObject() = default;
//    AbstractDataObject* clone() const override;
//    void addItem(DataValueType keyParameter) override;
//};

////! Surface data object
//class SurfaceDataObject : public AbstractDataObject
//{
//public:
//    SurfaceDataObject(const QString& name);
//    ~SurfaceDataObject() = default;
//    AbstractDataObject* clone() const override;
//    void addItem(DataValueType keyParameter) override;
//};


}

#endif // ABSTRACTDATAOBJECT_H
