/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the MatrixDataObject class
 */

#include "matrixdataobject.h"

using namespace QRS;

uint MatrixDataObject::smNumInstances = 0;
const IndexType skNumElements = 3;

//! Construct a matrix data object
MatrixDataObject::MatrixDataObject(QString const& name)
    : AbstractDataObject(DataObjectType::kMatrix, name)
{
    ++smNumInstances;
}

//! Insert a new item into MatrixDataObject
DataItemType& MatrixDataObject::addItem(DataValueType key)
{
    DataValueType rightKey = getAvailableItemKey(key);
    mItems.emplace(rightKey, DataItemType(skNumElements, skNumElements));
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

//! Import a matrix data object from a file
void MatrixDataObject::import(QTextStream& stream)
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
        for (IndexType i = 0; i != skNumElements; ++i)
            for (IndexType j = 0; j != skNumElements; ++j)
                stream >> item[i][j];
    }
}
