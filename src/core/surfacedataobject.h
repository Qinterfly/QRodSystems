/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the SurfaceDataObject class
 */

#ifndef SURFACEDATAOBJECT_H
#define SURFACEDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS
{

//! Surface data object
class SurfaceDataObject : public AbstractDataObject
{
public:
    SurfaceDataObject(QString const& name);
    ~SurfaceDataObject() {};
    AbstractDataObject* clone() const override;
    DataItemType& addItem(DataValueType key) override;
    DataKeyType addLeadingItem(DataValueType key);
    void removeLeadingItem(DataValueType key);
    bool changeLeadingItemKey(DataKeyType oldKey, DataKeyType newKey);
    DataHolder& getLeadingItems() { return mLeadingItems; }
    static uint numberInstances() { return smNumInstances; }
    static void setNumberInstances(uint numInstances) { smNumInstances = numInstances; }
    void serialize(QDataStream& stream) const override;
    virtual void deserialize(QDataStream& stream) override;
    virtual void import(QTextStream& stream) override;

private:
    static uint smNumInstances;
    DataHolder mLeadingItems;
};

}

#endif // SURFACEDATAOBJECT_H
