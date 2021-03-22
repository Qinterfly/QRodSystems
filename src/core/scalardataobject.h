#ifndef SCALARDATAOBJECT_H
#define SCALARDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS
{

//! Scalar data object
class ScalarDataObject : public AbstractDataObject
{
public:
    ScalarDataObject(const QString& name);
    ~ScalarDataObject() {};
    AbstractDataObject* clone() const override;
    void addItem(DataValueType keyParameter) override;
    static uint numberScalars();

private:
    static uint smNumScalars;

};

}

#endif // SCALARDATAOBJECT_H
