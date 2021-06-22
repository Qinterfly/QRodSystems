/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the AbstractCrossSectionRodComponent class
 */

#include "abstractcrosssectionrodcomponent.h"

using namespace QRS::Core;

quint32 AbstractCrossSectionRodComponent::smNumInstances = 0;

AbstractCrossSectionRodComponent::AbstractCrossSectionRodComponent(SectionType sectionType, QString const& name)
    : AbstractRodComponent(kCrossSection, name), mSectionType(sectionType)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
AbstractCrossSectionRodComponent::~AbstractCrossSectionRodComponent()
{
    --smNumInstances;
}

//! Serialize a cross section
void AbstractCrossSectionRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mComponentType;
    stream << mName;
    stream << (quint32)mSectionType;
    stream << (DataIDType)mID;
    // Area
    stream << mArea;
    // Inertia moments
    stream << mInertiaMomentTorsional << mInertiaMomentX << mInertiaMomentY << mInertiaMomentXY;
    // Center coordinates
    stream << mCenterCoordinateX << mCenterCoordinateY;
    // Eccentricity
    stream << mEccentricityX << mEccentricityY;
}

/*!
 * \brief Partly deserialize an abstract rod component
 *
 * It is assumed that a type and name have already been assigned.
 * So, only integrated properties need to be set.
 */
void AbstractCrossSectionRodComponent::deserialize(QDataStream& stream, DataObjectGetter const& /*getDataObject*/)
{
    stream >> mID;
    // Area
    stream >> mArea;
    // Inertia moments
    stream >> mInertiaMomentTorsional >> mInertiaMomentX >> mInertiaMomentY >> mInertiaMomentXY;
    // Center coordinates
    stream >> mCenterCoordinateX >> mCenterCoordinateY;
    // Eccentricity
    stream >> mEccentricityX >> mEccentricityY;
}

//! Copy integrated properties of a cross section
void AbstractCrossSectionRodComponent::copyIntegratedProperties(AbstractCrossSectionRodComponent const* pCrossSection)
{
    // Area
    mArea = pCrossSection->mArea;
    // Inertia moments
    mInertiaMomentTorsional = pCrossSection->mInertiaMomentTorsional;
    mInertiaMomentX = pCrossSection->mInertiaMomentX;
    mInertiaMomentY = pCrossSection->mInertiaMomentY;
    mInertiaMomentXY = pCrossSection->mInertiaMomentXY;
    // Center coordinates
    mCenterCoordinateX = pCrossSection->mCenterCoordinateX;
    mCenterCoordinateY = pCrossSection->mCenterCoordinateY;
    // Eccentricity
    mEccentricityX = pCrossSection->mEccentricityX;
    mEccentricityY = pCrossSection->mEccentricityY;
}
