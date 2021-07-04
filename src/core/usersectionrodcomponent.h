/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the UserSectionRodComponent class
 */

#ifndef USERSECTIONRODCOMPONENT_H
#define USERSECTIONRODCOMPONENT_H

#include "abstractsectionrodcomponent.h"
#include "core/scalardataobject.h"

namespace QRS::Core
{

//! Section which properties are defined by user
class UserSectionRodComponent : public AbstractSectionRodComponent
{
public:
    UserSectionRodComponent(QString const& name);
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override;
    // Getters
    // Area
    ScalarDataObject const* area() const { return mpArea; }
    // Inertia moments
    ScalarDataObject const* inertiaMomentTorsional() const { return mpInertiaMomentTorsional; }
    ScalarDataObject const* inertiaMomentX() const { return mpInertiaMomentX; }
    ScalarDataObject const* inertiaMomentY() const { return mpInertiaMomentY; }
    // Center coordinates
    ScalarDataObject const* centerCoordinateX() const { return mpCenterCoordinateX; }
    ScalarDataObject const* centerCoordinateY() const { return mpCenterCoordinateY; }
    // Setters
    // Area
    void setArea(ScalarDataObject const* pArea) { mpArea = pArea; }
    // Inertia moments
    void setInertiaMomentTorsional(ScalarDataObject const* pInertiaMomentTorsional) { mpInertiaMomentTorsional = pInertiaMomentTorsional; }
    void setInertiaMomentX(ScalarDataObject const* pInertiaMomentX) { mpInertiaMomentX = pInertiaMomentX; }
    void setInertiaMomentY(ScalarDataObject const* pInertiaMomentY) { mpInertiaMomentY = pInertiaMomentY; }
    // Center coordinates
    void setCenterCoordinateX(ScalarDataObject const* pCenterCoordinateX) { mpCenterCoordinateX = pCenterCoordinateX; }
    void setCenterCoordinateY(ScalarDataObject const* pCenterCoordinateY) { mpCenterCoordinateY = pCenterCoordinateY; }
};

}


#endif // USERSECTIONRODCOMPONENT_H
