/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Declaration of the SurfaceDataObject class
 */

#ifndef SURFACEDATAOBJECT_H
#define SURFACEDATAOBJECT_H

#include "abstractdataobject.h"

namespace QRS::Core
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
    quint32 numberLeadingItems() const { return mLeadingItems.size(); }
    DataHolder& getLeadingItems() { return mLeadingItems; }
    static quint32 numberInstances() { return smNumInstances; }
    static void setNumberInstances(quint32 numInstances) { smNumInstances = numInstances; }
    void serialize(QDataStream& stream) const override;
    virtual void deserialize(QDataStream& stream) override;
    virtual void import(QTextStream& stream) override;

private:
    static quint32 smNumInstances;
    DataHolder mLeadingItems;
};

}

#endif // SURFACEDATAOBJECT_H
