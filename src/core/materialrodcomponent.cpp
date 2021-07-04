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
void MaterialRodComponent::deserialize(QDataStream& stream, DataObjectGetter const& getDataObject)
{
    stream >> mID;
    mpElasticModulus = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpShearModulus = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpPoissonsRatio = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpDensity = (ScalarDataObject const*)readDataObjectPointer(stream, getDataObject);
}

//! Check whether the component data is complete
bool MaterialRodComponent::isDataComplete() const
{
    return mpElasticModulus && mpShearModulus
           && mpDensity && mpPoissonsRatio;
}
