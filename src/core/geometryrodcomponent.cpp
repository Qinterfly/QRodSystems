/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the GeometryRodComponent class
 */

#include "geometryrodcomponent.h"
#include "vectordataobject.h"
#include "matrixdataobject.h"

using namespace QRS::Core;

quint32 GeometryRodComponent::smNumInstances = 0;

GeometryRodComponent::GeometryRodComponent(QString const& name)
    : AbstractRodComponent(kGeometry, name)
{
    ++smNumInstances;
}

//! Decrease a number of instances while being destroyed
GeometryRodComponent::~GeometryRodComponent()
{
    --smNumInstances;
}

//! Clone a geometrical rod component
AbstractRodComponent* GeometryRodComponent::clone() const
{
    GeometryRodComponent* pGeometry = new GeometryRodComponent(mName);
    pGeometry->mID = mID;
    pGeometry->mpRadiusVector = mpRadiusVector;
    pGeometry->mpRotationMatrix = mpRotationMatrix;
    --smNumInstances;
    return pGeometry;
}

//! Serialize all properties of a geometrical component
void GeometryRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mkComponentType;
    stream << mName;
    stream << (DataIDType)mID;
    writeDataObjectPointer(stream, mpRadiusVector);
    writeDataObjectPointer(stream, mpRotationMatrix);
}

//! Deserialize a geometrical component
void GeometryRodComponent::deserialize(QDataStream& stream, DataObjects const& dataObjects)
{
    stream >> mID;
    mpRadiusVector = (VectorDataObject const*)readDataObjectPointer(stream, dataObjects);
    mpRotationMatrix = (MatrixDataObject const*)readDataObjectPointer(stream, dataObjects);
}

//! Resolve references of a geometrical rod component
void GeometryRodComponent::resolveReferences(DataObjects const& dataObjects)
{
    mpRadiusVector = (VectorDataObject const*)substituteDataObject(dataObjects, mpRadiusVector);
    mpRotationMatrix = (MatrixDataObject const*)substituteDataObject(dataObjects, mpRotationMatrix);
}

//! Check whether the component data is complete
bool GeometryRodComponent::isDataComplete() const
{
    return mpRadiusVector && mpRotationMatrix;
};
