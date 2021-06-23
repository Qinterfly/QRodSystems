/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the UserCrossSectionRodComponent class
 */

#ifndef USERCROSSSECTIONRODCOMPONENT_H
#define USERCROSSSECTIONRODCOMPONENT_H

#include "abstractcrosssectionrodcomponent.h"
#include "core/scalardataobject.h"

namespace QRS::Core
{

//! Cross section which properties are defined by user
class UserCrossSectionRodComponent : public AbstractCrossSectionRodComponent
{
public:
    UserCrossSectionRodComponent(QString const& name);
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override;

    // Area
    ScalarDataObject const* area() const { return mpArea; }
    // Inertia moments
    ScalarDataObject const* inertiaMomentTorsional() const { return mpInertiaMomentTorsional; }
    ScalarDataObject const* inertiaMomentX() const { return mpInertiaMomentX; }
    ScalarDataObject const* inertiaMomentY() const { return mpInertiaMomentY; }
    ScalarDataObject const* inertiaMomentProductXY() const { return mpInertiaProductXY; }
    // Center coordinates
    ScalarDataObject const* centerCoordinateX() const { return mpCenterCoordinateX; }
    ScalarDataObject const* centerCoordinateY() const { return mpCenterCoordinateY; }

    // Area
    void setArea(ScalarDataObject const* pArea) { mpArea = pArea; }
    // Inertia moments
    void setInertiaMomentTorsional(ScalarDataObject const* pInertiaMomentTorsional) { mpInertiaMomentTorsional = pInertiaMomentTorsional; }
    void setInertiaMomentX(ScalarDataObject const* pInertiaMomentX) { mpInertiaMomentX = pInertiaMomentX; }
    void setInertiaMomentY(ScalarDataObject const* pInertiaMomentY) { mpInertiaMomentY = pInertiaMomentY; }
    void setInertiaProductXY(ScalarDataObject const* pInertiaProductXY) { mpInertiaProductXY = pInertiaProductXY; }
    // Center coordinates
    void setCenterCoordinateX(ScalarDataObject const* pCenterCoordinateX) { mpCenterCoordinateX = pCenterCoordinateX; }
    void setCenterCoordinateY(ScalarDataObject const* pCenterCoordinateY) { mpCenterCoordinateY = pCenterCoordinateY; }
};

}


#endif // USERCROSSSECTIONRODCOMPONENT_H
