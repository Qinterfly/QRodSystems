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

//! Clone a geometrical component
AbstractRodComponent* GeometryRodComponent::clone() const
{
    GeometryRodComponent* pComponent = new GeometryRodComponent(mName);
    pComponent->mpRadiusVector = mpRadiusVector;
    pComponent->mpRotationMatrix = mpRotationMatrix;
    return pComponent;
}

//! Serialize all properties of a geometrical component
void GeometryRodComponent::serialize(QDataStream& stream) const
{
    AbstractRodComponent::serialize(stream);
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


