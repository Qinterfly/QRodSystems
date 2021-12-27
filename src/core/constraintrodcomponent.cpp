/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the ConstraintRodComponent class
 */

#include "constraintrodcomponent.h"

using namespace QRS::Core;

quint32 ConstraintRodComponent::smNumInstances = 0;

ConstraintRodComponent::ConstraintRodComponent(QString const& name)
    : AbstractRodComponent(kConstraint, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
ConstraintRodComponent::~ConstraintRodComponent()
{
    --smNumInstances;
}

//! Clone a constraint rod component
AbstractRodComponent* ConstraintRodComponent::clone() const
{
    ConstraintRodComponent* pConstraint = new ConstraintRodComponent(mName);
    pConstraint->mID = mID;
    pConstraint->mConstraints = mConstraints;
    --smNumInstances;
    return pConstraint;
}

//! Serialize all properties of a constraint component
void ConstraintRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mkComponentType;
    stream << mName;
    stream << (DataIDType)mID;
    stream << (quint32)mConstraints.size();  // Number of constraints
    for (auto const& item : mConstraints)
        stream << item.first << item.second; // Constraint type and coordinate system
}

//! Deserialize a constraint component
void ConstraintRodComponent::deserialize(QDataStream& stream, DataObjects const& /*dataObjects*/)
{
    quint32 numConstraints;
    stream >> mID;
    stream >> numConstraints;
    for (quint32 iConstraint = 0; iConstraint != numConstraints; ++iConstraint)
    {
        ConstraintType type;
        ConstraintCoordinateSystem coordinateSystem;
        stream >> type;
        stream >> coordinateSystem;
        mConstraints.emplace(type, coordinateSystem);
    }
}

//! Check whether the constraint of the specified type exists
bool ConstraintRodComponent::isConstraintExist(ConstraintType type) const
{
    return mConstraints.contains(type);
}

//! Set a constraint
void ConstraintRodComponent::setConstraint(ConstraintType type, ConstraintCoordinateSystem coordinateSystem)
{
    mConstraints[type] = coordinateSystem;
}

//! Remove the constriant of a given type
bool ConstraintRodComponent::removeConstraint(ConstraintType type)
{
    if (!mConstraints.contains(type))
        return false;
    mConstraints.erase(type);
    return true;
}

