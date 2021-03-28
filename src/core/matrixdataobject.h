/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the MatrixDataObject class
 */

#ifndef MATRIXDATAOBJECT_H
#define MATRIXDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS
{

//! Matrix data object
class MatrixDataObject : public AbstractDataObject
{
public:
    MatrixDataObject(QString const& name);
    ~MatrixDataObject() {};
    AbstractDataObject* clone() const override;
    DataItemType& addItem(DataValueType key) override;
    static uint numberInstances();

private:
    static uint smNumInstances;
};

}

#endif // MATRIXDATAOBJECT_H
