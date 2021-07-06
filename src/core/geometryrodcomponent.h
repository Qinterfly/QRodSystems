/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the GeometryRodComponent class
 */

#ifndef GEOMETRYRODCOMPONENT_H
#define GEOMETRYRODCOMPONENT_H

#include <QPointer>
#include "abstractrodcomponent.h"

namespace QRS::Core
{

class VectorDataObject;
class MatrixDataObject;

//! Geometrical configuration of a rod
class GeometryRodComponent : public AbstractRodComponent
{
public:
    GeometryRodComponent(QString const& name);
    ~GeometryRodComponent();
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override;
    void setRadiusVector(VectorDataObject const* pRadiusVector) { mpRadiusVector = pRadiusVector; }
    void setRotationMatrix(MatrixDataObject const* pRotationMatrix) { mpRotationMatrix = pRotationMatrix; }
    VectorDataObject const* radiusVector() const { return mpRadiusVector; }
    MatrixDataObject const* rotationMatrix() const { return mpRotationMatrix; }
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjects const& dataObjects) override;
    void resolveReferences(DataObjects const& dataObjects) override;

private:
    static quint32 smNumInstances;
    QPointer<VectorDataObject const> mpRadiusVector;
    QPointer<MatrixDataObject const> mpRotationMatrix;
};

}

#endif // GEOMETRYRODCOMPONENT_H
