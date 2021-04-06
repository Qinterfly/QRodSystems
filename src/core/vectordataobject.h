/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the VectorDataObject class
 */

#ifndef VECTORDATAOBJECT_H
#define VECTORDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS
{

//! Vector data object
class VectorDataObject : public AbstractDataObject
{
public:
    VectorDataObject(QString const& name);
    ~VectorDataObject() {};
    AbstractDataObject* clone() const override;
    DataItemType& addItem(DataValueType key) override;
    static uint numberInstances() { return smNumInstances; }
    static void setNumberInstances(uint numInstances) { smNumInstances = numInstances; }
    virtual void import(QTextStream& stream) override;

private:
    static uint smNumInstances;
};

}

#endif // VECTORDATAOBJECT_H
