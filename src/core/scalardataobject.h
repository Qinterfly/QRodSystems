/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the ScalarDataObject class
 */

#ifndef SCALARDATAOBJECT_H
#define SCALARDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS::Core
{

//! Scalar data object
class ScalarDataObject : public AbstractDataObject
{
public:
    ScalarDataObject(QString const& name);
    ~ScalarDataObject() {};
    AbstractDataObject* clone() const override;
    DataItemType& addItem(DataValueType key) override;
    static quint32 numberInstances() { return smNumInstances; }
    static void setNumberInstances(quint32 numInstances) { smNumInstances = numInstances; }
    virtual void import(QTextStream& stream) override;

private:
    static quint32 smNumInstances;
};

}

#endif // SCALARDATAOBJECT_H
