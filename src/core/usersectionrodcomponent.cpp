/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the UserSectionRodComponent class
 */

#include "usersectionrodcomponent.h"

using namespace QRS::Core;

UserSectionRodComponent::UserSectionRodComponent(QString const& name)
    : AbstractSectionRodComponent(kUserDefined, name)
{

}

//! Clone a user-defined cross section
AbstractRodComponent* UserSectionRodComponent::clone() const
{
    UserSectionRodComponent* pSection = new UserSectionRodComponent(mName);
    pSection->mID = mID;
    pSection->copyIntegratedProperties(this);
    --smNumInstances;
    return pSection;
}

//! \brief Check if specified data is complete
//!
//! Some of properties may be of zero values to achieve infinite stiffness
bool UserSectionRodComponent::isDataComplete() const
{
    return mpArea;
}
