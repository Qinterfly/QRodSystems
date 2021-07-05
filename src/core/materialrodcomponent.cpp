/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the MaterialRodComponent class
 */

#include "materialrodcomponent.h"
#include "scalardataobject.h"

using namespace QRS::Core;

quint32 MaterialRodComponent::smNumInstances = 0;

MaterialRodComponent::MaterialRodComponent(QString const& name)
    : AbstractRodComponent(kMaterial, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
MaterialRodComponent::~MaterialRodComponent()
{
    --smNumInstances;
}

//! Clone a material rod component
AbstractRodComponent* MaterialRodComponent::clone() const
{
    MaterialRodComponent* pMaterial = new MaterialRodComponent(mName);
    pMaterial->mID = mID;
    pMaterial->mpElasticModulus = mpElasticModulus;
    pMaterial->mpShearModulus = mpShearModulus;
    pMaterial->mpPoissonsRatio = mpPoissonsRatio;
    pMaterial->mpDensity = mpDensity;
    --smNumInstances;
    return pMaterial;
}

//! Serialize all properties of a material component
void MaterialRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mkComponentType;
    stream << mName;
    stream << (DataIDType)mID;
    writeDataObjectPointer(stream, mpElasticModulus);
    writeDataObjectPointer(stream, mpShearModulus);
    writeDataObjectPointer(stream, mpPoissonsRatio);
    writeDataObjectPointer(stream, mpDensity);
}

//! Deserialize a material component
void MaterialRodComponent::deserialize(QDataStream& stream, DataObjects const& dataObjects)
{
    stream >> mID;
    mpElasticModulus = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpShearModulus = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpPoissonsRatio = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpDensity = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
}

//! Resolve references of a material rod component
void MaterialRodComponent::resolveReferences(DataObjects const& dataObjects)
{
    mpElasticModulus = (ScalarDataObject const*)substituteDataObject(dataObjects, mpElasticModulus);
    mpShearModulus = (ScalarDataObject const*)substituteDataObject(dataObjects, mpShearModulus);
    mpPoissonsRatio = (ScalarDataObject const*)substituteDataObject(dataObjects, mpPoissonsRatio);
    mpDensity = (ScalarDataObject const*)substituteDataObject(dataObjects, mpDensity);
}

//! Check whether the component data is complete
bool MaterialRodComponent::isDataComplete() const
{
    return mpElasticModulus && mpShearModulus
           && mpDensity && mpPoissonsRatio;
}
