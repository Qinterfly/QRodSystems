/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the MatrixDataObject class
 */

#ifndef MATRIXDATAOBJECT_H
#define MATRIXDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS::Core
{

//! Matrix data object
class MatrixDataObject : public AbstractDataObject
{
public:
    MatrixDataObject(QString const& name);
    ~MatrixDataObject();
    AbstractDataObject* clone() const override;
    DataItemType& addItem(DataValueType key) override;
    static quint32 numberInstances() { return smNumInstances; }
    virtual void import(QTextStream& stream) override;

private:
    static quint32 smNumInstances;
};

}

#endif // MATRIXDATAOBJECT_H
