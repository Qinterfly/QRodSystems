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
    ~GeometryRodComponent() {};
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override { return mpRadiusVector && mpRotationMatrix; };
    void setRadiusVector(VectorDataObject const* pRadiusVector) { mpRadiusVector = pRadiusVector; }
    void setRotationMatrix(MatrixDataObject const* pRotationMatrix) { mpRotationMatrix = pRotationMatrix; }

private:
    static quint32 smNumInstances;
    QPointer<VectorDataObject const> mpRadiusVector = nullptr;
    QPointer<MatrixDataObject const> mpRotationMatrix = nullptr;
};

}

#endif // GEOMETRYRODCOMPONENT_H
