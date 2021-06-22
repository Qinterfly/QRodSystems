/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
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
    return pGeometry;
}

//! Serialize all properties of a geometrical component
void GeometryRodComponent::serialize(QDataStream& stream) const
{
    stream << (quint32)mComponentType;
    stream << mName;
    stream << (DataIDType)mID;
    writeDataObjectPointer(stream, mpRadiusVector);
    writeDataObjectPointer(stream, mpRotationMatrix);
}

//! Deserialize a geometrical component
void GeometryRodComponent::deserialize(QDataStream& stream, DataObjectGetter const& getDataObject)
{
    stream >> mID;
    mpRadiusVector = (VectorDataObject const*)readDataObjectPointer(stream, getDataObject);
    mpRotationMatrix = (MatrixDataObject const*)readDataObjectPointer(stream, getDataObject);
}
