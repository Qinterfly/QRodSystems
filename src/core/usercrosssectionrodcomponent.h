/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the UserCrossSectionRodComponent class
 */

#ifndef USERCROSSSECTIONRODCOMPONENT_H
#define USERCROSSSECTIONRODCOMPONENT_H

#include "abstractcrosssectionrodcomponent.h"

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
    DataValueType& area() { return mArea; }
    // Inertia moments
    DataValueType& inertiaMomemntTorsional() { return mInertiaMomentTorsional; }
    DataValueType& inertiaMomentX() { return mInertiaMomentX; }
    DataValueType& inertiaMomentY() { return mInertiaMomentY; }
    DataValueType& inertiaMomentXY() { return mInertiaMomentXY; }
    // Center coordinates
    DataValueType& centerCoordinateX() { return mCenterCoordinateX; }
    DataValueType& centerCoordinateY() { return mCenterCoordinateY; }
    // Eccentricity
    DataValueType& eccentricityX() { return mEccentricityX; }
    DataValueType& eccentricityY() { return mEccentricityY; }
};

}


#endif // USERCROSSSECTIONRODCOMPONENT_H
