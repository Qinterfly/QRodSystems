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
    static uint numberSurfaces();

private:
    static uint smNumSurfaces;
};

}

#endif // SURFACEDATAOBJECT_H
