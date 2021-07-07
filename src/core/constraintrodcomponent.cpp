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
    stream << (quint32)mConstraints.size();                           // Number of constraints
    for (auto const& itemType : mConstraints)
    {
        stream << itemType.first;                                     // Type of a constraint
        DirectionConstraints const& mapDirections = itemType.second;
        stream << (quint32)mapDirections.size();                      // Number of directions
        for (auto const& itemDirection : mapDirections)
            stream << itemDirection.first << itemDirection.second;    // Direction and coordinate system
    }
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
        quint32 numDirections;
        stream >> type;
        stream >> numDirections;
        DirectionConstraints mapDirections;
        for (quint32 iDirection = 0; iDirection != numDirections; ++iDirection)
        {
            ConstraintDirection direction;
            ConstraintCoordinateSystem coordinateSystem;
            stream >> direction;
            stream >> coordinateSystem;
            mapDirections.emplace(direction, coordinateSystem);
        }
        mConstraints.emplace(type, mapDirections);
    }
}

//! Check whether the constraint of the specified type already exists
bool ConstraintRodComponent::isConstraintExist(ConstraintType type, ConstraintDirection direction)
{
    return mConstraints.contains(type) && mConstraints[type].contains(direction);
}

//! Add a constraint if it has not been added before
bool ConstraintRodComponent::addConstraint(ConstraintType type, ConstraintDirection direction, ConstraintCoordinateSystem coordinateSystem)
{
    if (isConstraintExist(type, direction))
        return false;
    mConstraints[type][direction] = coordinateSystem;
    return true;
}
