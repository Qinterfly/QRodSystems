/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the AbstractSectionRodComponent class
 */

#include "abstractsectionrodcomponent.h"
#include "core/scalardataobject.h"

using namespace QRS::Core;

quint32 AbstractSectionRodComponent::smNumInstances = 0;

AbstractSectionRodComponent::AbstractSectionRodComponent(SectionType sectionType, QString const& name)
    : AbstractRodComponent(kSection, name), mkSectionType(sectionType)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
AbstractSectionRodComponent::~AbstractSectionRodComponent()
{
    --smNumInstances;
}

//! Serialize a cross section
void AbstractSectionRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mkComponentType;
    stream << mName;
    stream << (quint32)mkSectionType;
    stream << (DataIDType)mID;
    // Area
    writeDataObjectPointer(stream, mpArea);
    // Inertia moments
    writeDataObjectPointer(stream, mpInertiaMomentTorsional);
    writeDataObjectPointer(stream, mpInertiaMomentX);
    writeDataObjectPointer(stream, mpInertiaMomentY);
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
void AbstractSectionRodComponent::deserialize(QDataStream& stream, DataObjects const& dataObjects)
{
    stream >> mID;
    // Area
    mpArea = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    // Inertia moments
    mpInertiaMomentTorsional = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpInertiaMomentX = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpInertiaMomentY = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    // Center coordinates
    mpCenterCoordinateX  = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpCenterCoordinateY   = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
}

//! Resolve references of a cross-section
void AbstractSectionRodComponent::resolveReferences(DataObjects const& dataObjects)
{
    // Area
    mpArea = (ScalarDataObject const*)substituteDataObject(dataObjects, mpArea);
    // Inertia moments
    mpInertiaMomentTorsional = (ScalarDataObject const*)substituteDataObject(dataObjects, mpInertiaMomentTorsional);
    mpInertiaMomentX = (ScalarDataObject const*)substituteDataObject(dataObjects, mpInertiaMomentX);
    mpInertiaMomentY = (ScalarDataObject const*)substituteDataObject(dataObjects, mpInertiaMomentY);
    // Center coordinates
    mpCenterCoordinateX  = (ScalarDataObject const*)substituteDataObject(dataObjects, mpCenterCoordinateX);
    mpCenterCoordinateY  = (ScalarDataObject const*)substituteDataObject(dataObjects, mpCenterCoordinateY);
}

//! Copy integrated properties of a cross section
void AbstractSectionRodComponent::copyIntegratedProperties(AbstractSectionRodComponent const* pSection)
{
    // Area
    mpArea = pSection->mpArea;
    // Inertia moments
    mpInertiaMomentTorsional = pSection->mpInertiaMomentTorsional;
    mpInertiaMomentX = pSection->mpInertiaMomentX;
    mpInertiaMomentY = pSection->mpInertiaMomentY;
    // Center coordinates
    mpCenterCoordinateX = pSection->mpCenterCoordinateX;
    mpCenterCoordinateY = pSection->mpCenterCoordinateY;
}

