/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the UserCrossSectionRodComponent class
 */

#include "usercrosssectionrodcomponent.h"

using namespace QRS::Core;

UserCrossSectionRodComponent::UserCrossSectionRodComponent(QString const& name)
    : AbstractCrossSectionRodComponent(kUserDefined, name)
{

}

//! Clone a user defied cross section
AbstractRodComponent* UserCrossSectionRodComponent::clone() const
{
    UserCrossSectionRodComponent* pCrossSection = new UserCrossSectionRodComponent(mName);
    pCrossSection->mID = mID;
    pCrossSection->copyIntegratedProperties(this);
    return pCrossSection;
}

//! \brief Check if a specified data is complete
//!
//! Some of properties may be of zero values to achieve infinite stiffness
bool UserCrossSectionRodComponent::isDataComplete() const
{
    return mArea >= 0
           && mInertiaMomentTorsional >= 0
           && mInertiaMomentX >= 0 && mInertiaMomentY >= 0;
}
