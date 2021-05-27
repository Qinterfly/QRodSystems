/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the VectorDataObject class
 */

#ifndef VECTORDATAOBJECT_H
#define VECTORDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS::Core
{

//! Vector data object
class VectorDataObject : public AbstractDataObject
{
public:
    VectorDataObject(QString const& name);
    ~VectorDataObject() {};
    AbstractDataObject* clone() const override;
    DataItemType& addItem(DataValueType key) override;
    static quint32 numberInstances() { return smNumInstances; }
    static void setNumberInstances(quint32 numInstances) { smNumInstances = numInstances; }
    virtual void import(QTextStream& stream) override;

private:
    static quint32 smNumInstances;
};

}

#endif // VECTORDATAOBJECT_H
