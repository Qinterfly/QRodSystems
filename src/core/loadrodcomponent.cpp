/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the LoadRodComponent class
 */

#include "loadrodcomponent.h"
#include "scalardataobject.h"
#include "vectordataobject.h"

using namespace QRS::Core;

quint32 LoadRodComponent::smNumInstances = 0;

LoadRodComponent::LoadRodComponent(QString const& name)
    : AbstractRodComponent(kLoad, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
LoadRodComponent::~LoadRodComponent()
{
    --smNumInstances;
}

//! Clone a rod load
AbstractRodComponent* LoadRodComponent::clone() const
{
    LoadRodComponent* pLoad = new LoadRodComponent(mName);
    pLoad->mID = mID;
    pLoad->mLoadType = mLoadType;
    pLoad->mpDirectionVector = mpDirectionVector;
    pLoad->mpLongitudinalFunction = mpLongitudinalFunction;
    pLoad->mpTimeCoefficient = mpTimeCoefficient;
    pLoad->mpTimeRotationVector = mpTimeRotationVector;
    pLoad->mMultiplier = mMultiplier;
    pLoad->mIsFollowing = mIsFollowing;
    --smNumInstances;
    return pLoad;
}

//! Serialize all properties of a rod load
void LoadRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mkComponentType;
    stream << mName;
    stream << (DataIDType)mID;
    stream << mLoadType;
    writeDataObjectPointer(stream, mpDirectionVector);
    writeDataObjectPointer(stream, mpLongitudinalFunction);
    writeDataObjectPointer(stream, mpTimeCoefficient);
    writeDataObjectPointer(stream, mpTimeRotationVector);
    stream << mMultiplier;
    stream << mIsFollowing;
}

//! Deserialize a rod load
void LoadRodComponent::deserialize(QDataStream& stream, DataObjects const& dataObjects)
{
    stream >> mID;
    stream >> mLoadType;
    mpDirectionVector = (VectorDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpLongitudinalFunction = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpTimeCoefficient = (ScalarDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpTimeRotationVector = (VectorDataObject const*)readDataObjectPointer(stream, dataObjects);
    stream >> mMultiplier;
    stream >> mIsFollowing;
}

//! Resolve references of a rod load
void LoadRodComponent::resolveReferences(DataObjects const& dataObjects)
{
    mpDirectionVector = (VectorDataObject const*)substituteDataObject(dataObjects, mpDirectionVector);
    mpLongitudinalFunction = (ScalarDataObject const*)substituteDataObject(dataObjects, mpLongitudinalFunction);
    mpTimeCoefficient = (ScalarDataObject const*)substituteDataObject(dataObjects, mpTimeCoefficient);
    mpTimeRotationVector = (VectorDataObject const*)substituteDataObject(dataObjects, mpTimeRotationVector);
}

//! Check whether the component data is complete
bool LoadRodComponent::isDataComplete() const
{
    return mLoadType != kNone && mpDirectionVector;
}
