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


namespace QRS
{

using DataValueType = double;
using DataKeyType = double;
using DataItemType = Array<DataValueType>;

enum DataObjectType
{
    kScalar,
    kVector,
    kMatrix,
    kSurface
};

//! Data object which is designied in the way to be represented in a table easily
class AbstractDataObject
{
    using DataHolder = std::unordered_map<DataKeyType, DataItemType>;

public:
    AbstractDataObject(DataObjectType type, const QString& name);
    virtual ~AbstractDataObject() = default;
    uint getID() const { return mID; }
    virtual void addItem(DataValueType keyParameter) = 0;
    void removeItem(DataValueType keyParameter);
    static uint numberObjects();

protected:
    const DataObjectType mType;
    QString mName;
    DataHolder mItems;
    uint mID;

private:
    static uint smNumObjects;
};

//! Scalar data object
class ScalarDataObject : public AbstractDataObject
{
public:
    ScalarDataObject(const QString& name);
    ~ScalarDataObject() {};
    void addItem(DataValueType keyParameter) override;
};

//! Vector data object
class VectorDataObject : public AbstractDataObject
{
public:
    VectorDataObject(const QString& name);
    ~VectorDataObject() = default;
    void addItem(DataValueType keyParameter) override;
};

//! Matrix data object
class MatrixDataObject : public AbstractDataObject
{
public:
    MatrixDataObject(const QString& name);
    ~MatrixDataObject() = default;
    void addItem(DataValueType keyParameter) override;
};

//! Surface data object
class SurfaceDataObject : public AbstractDataObject
{
public:
    SurfaceDataObject(const QString& name);
    ~SurfaceDataObject() = default;
    void addItem(DataValueType keyParameter) override;
};


}

#endif // ABSTRACTDATAOBJECT_H
