/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the MechanicalRodComponent class
 */

#include "mechanicalrodcomponent.h"
#include "scalardataobject.h"

using namespace QRS::Core;

quint32 MechanicalRodComponent::smNumInstances = 0;

MechanicalRodComponent::MechanicalRodComponent(QString const& name)
    : AbstractRodComponent(kMechanical, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
MechanicalRodComponent::~MechanicalRodComponent()
{
    --smNumInstances;
}

//! Clone a geometrical rod component
AbstractRodComponent* MechanicalRodComponent::clone() const
{
    MechanicalRodComponent* pMechanical = new MechanicalRodComponent(mName);
    pMechanical->mID = mID;
    // Stiffness distribution
    pMechanical->mpTensionStiffness   = mpTensionStiffness;
    pMechanical->mpTorsionalStiffness = mpTorsionalStiffness;
    pMechanical->mpBendingStiffnessX  = mpBendingStiffnessX;
    pMechanical->mpBendingStiffnessY  = mpBendingStiffnessY;
    // Mass distribution
    pMechanical->mpLinearMassDensity  = mpLinearMassDensity;
    pMechanical->mpInertiaMassMomentX = mpInertiaMassMomentX;
    pMechanical->mpInertiaMassMomentY = mpInertiaMassMomentY;
    pMechanical->mpInertiaMassMomentZ = mpInertiaMassMomentZ;
    // Eccentricity
    pMechanical->mpEccentricityX = mpEccentricityX;
    pMechanical->mpEccentricityY = mpEccentricityY;
    // Contact diameter
    pMechanical->mpContactDiameter = mpContactDiameter;
    --smNumInstances;
    return pMechanical;
}

//! Serialize all properties of a geometrical component
void MechanicalRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mkComponentType;
    stream << mName;
    stream << (DataIDType)mID;
    // Stiffness distribution
    writeDataObjectPointer(stream, mpTensionStiffness);
    writeDataObjectPointer(stream, mpTorsionalStiffness);
    writeDataObjectPointer(stream, mpBendingStiffnessX);
    writeDataObjectPointer(stream, mpBendingStiffnessY);
    // Mass distribution
    writeDataObjectPointer(stream, mpLinearMassDensity);
    writeDataObjectPointer(stream, mpInertiaMassMomentX);
    writeDataObjectPointer(stream, mpInertiaMassMomentY);
    writeDataObjectPointer(stream, mpInertiaMassMomentZ);
    // Eccentricity
    writeDataObjectPointer(stream, mpEccentricityX);
    writeDataObjectPointer(stream, mpEccentricityY);
    // Contact diameter
    writeDataObjectPointer(stream, mpContactDiameter);
}

//! Deserialize a geometrical component
void MechanicalRodComponent::deserialize(QDataStream& stream, DataObjects const& dataObjects)
{
    stream >> mID;
    // Stiffness distribution
    mpTensionStiffness   = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpTorsionalStiffness = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpBendingStiffnessX  = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpBendingStiffnessY  = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    // Mass distribution
    mpLinearMassDensity  = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpInertiaMassMomentX = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpInertiaMassMomentY = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpInertiaMassMomentZ = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    // Eccentricity
    mpEccentricityX = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpEccentricityY = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    // Contact diameter
    mpContactDiameter = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
}

//! Resolve references of a geometrical rod component
void MechanicalRodComponent::resolveReferences(DataObjects const& dataObjects)
{
    // Stiffness distribution
    mpTensionStiffness = (ScalarDataObject const*)substituteDataObject(dataObjects, mpTensionStiffness);
    mpTorsionalStiffness = (ScalarDataObject const*)substituteDataObject(dataObjects, mpTorsionalStiffness);
    mpBendingStiffnessX = (ScalarDataObject const*)substituteDataObject(dataObjects, mpBendingStiffnessX);
    mpBendingStiffnessY = (ScalarDataObject const*)substituteDataObject(dataObjects, mpBendingStiffnessY);
    // Mass distribution
    mpLinearMassDensity = (ScalarDataObject const*)substituteDataObject(dataObjects, mpLinearMassDensity);
    mpInertiaMassMomentX = (ScalarDataObject const*)substituteDataObject(dataObjects, mpInertiaMassMomentX);
    mpInertiaMassMomentY = (ScalarDataObject const*)substituteDataObject(dataObjects, mpInertiaMassMomentY);
    mpInertiaMassMomentZ = (ScalarDataObject const*)substituteDataObject(dataObjects, mpInertiaMassMomentZ);
    // Eccentricity
    mpEccentricityX = (ScalarDataObject const*)substituteDataObject(dataObjects, mpEccentricityX);
    mpEccentricityY = (ScalarDataObject const*)substituteDataObject(dataObjects, mpEccentricityY);
    // Contact diameter
    mpContactDiameter = (ScalarDataObject const*)substituteDataObject(dataObjects, mpContactDiameter);
}
