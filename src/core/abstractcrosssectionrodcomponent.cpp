/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the AbstractCrossSectionRodComponent class
 */

#include "abstractcrosssectionrodcomponent.h"
#include "core/scalardataobject.h"

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
    writeDataObjectPointer(stream, mpArea);
    // Inertia moments
    writeDataObjectPointer(stream, mpInertiaMomentTorsional);
    writeDataObjectPointer(stream, mpInertiaMomentX);
    writeDataObjectPointer(stream, mpInertiaMomentY);
    writeDataObjectPointer(stream, mpInertiaProductXY);
    // Center coordinates
    writeDataObjectPointer(stream, mpCenterCoordinateX);
    writeDataObjectPointer(stream, mpCenterCoordinateY);
}

/*!
 * \brief Partly deserialize an abstract rod component
 *
 * It is assumed that a type and name have already been assigned.
 * So, only integrated properties need to be set.
 */
void AbstractCrossSectionRodComponent::deserialize(QDataStream& stream, DataObjectGetter const& getDataObject)
{
    stream >> mID;
    // Area
    mpArea = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    // Inertia moments
    mpInertiaMomentTorsional = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpInertiaMomentX = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpInertiaMomentY = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpInertiaProductXY = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    // Center coordinates
    mpCenterCoordinateX  = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpCenterCoordinateY   = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
}

//! Copy integrated properties of a cross section
void AbstractCrossSectionRodComponent::copyIntegratedProperties(AbstractCrossSectionRodComponent const* pCrossSection)
{
    // Area
    mpArea = pCrossSection->mpArea;
    // Inertia moments
    mpInertiaMomentTorsional = pCrossSection->mpInertiaMomentTorsional;
    mpInertiaMomentX = pCrossSection->mpInertiaMomentX;
    mpInertiaMomentY = pCrossSection->mpInertiaMomentY;
    mpInertiaProductXY = pCrossSection->mpInertiaProductXY;
    // Center coordinates
    mpCenterCoordinateX = pCrossSection->mpCenterCoordinateX;
    mpCenterCoordinateY = pCrossSection->mpCenterCoordinateY;
}
