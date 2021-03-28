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
    static uint numberInstances();

private:
    static uint smNumInstances;
    DataHolder mLeadingItems;
};

}

#endif // SURFACEDATAOBJECT_H
