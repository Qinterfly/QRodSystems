/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the LoadRodComponent class
 */

#ifndef LOADRODCOMPONENT_H
#define LOADRODCOMPONENT_H

#include <QPointer>
#include "abstractrodcomponent.h"

namespace QRS::Core
{

class ScalarDataObject;
class VectorDataObject;

//! Load applied to a rod
class LoadRodComponent : public AbstractRodComponent
{
public:
    enum LoadType
    {
        kNone,
        kForcedDisplacements, kForcedRotations,
        kPointForce, kPointMoment,
        kPointMass, kPointInertiaMoment,
        kPointLinearDamper, kPointRotationalDamper,
        kDistributedForce, kDistributedMoment,
        kAerodynamicFlow,
        kAcceleration,
        kInnerLiquidFlow,
        kDisplacementDamping, kRotationDamping
    };
    LoadRodComponent(QString const& name);
    ~LoadRodComponent();
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override;
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjects const& dataObjects) override;
    void resolveReferences(DataObjects const& dataObjects) override;
    // Getters
    LoadType loadType() const { return mLoadType; }
    VectorDataObject const* directionVector() const { return mpDirectionVector; }
    ScalarDataObject const* loadGraph() const { return mpLoadGraph; }
    ScalarDataObject const* timeCoefficient() const { return mpTimeCoefficient; }
    VectorDataObject const* timeRotationVector() const { return mpTimeRotationVector; }
    DataValueType multiplier() const { return mMultiplier; }
    // Setters
    void setType(LoadType type) { mLoadType = type; }
    void setDirectionVector(VectorDataObject const* pDirectionVector) { mpDirectionVector = pDirectionVector; }
    void setLoadGraph(ScalarDataObject const* pLoadGraph) { mpLoadGraph = pLoadGraph; }
    void setTimeCoefficient(ScalarDataObject const* pTimeCoefficient) { mpTimeCoefficient = pTimeCoefficient; }
    void setTimeRotationVector(VectorDataObject const* pTimeRotationVector) { mpTimeRotationVector = pTimeRotationVector; }
    void setMultiplier(DataValueType value) { mMultiplier = value; }

private:
    static quint32 smNumInstances;
    LoadType mLoadType = kNone;
    QPointer<VectorDataObject const> mpDirectionVector;
    QPointer<ScalarDataObject const> mpLoadGraph;
    QPointer<ScalarDataObject const> mpTimeCoefficient;
    QPointer<VectorDataObject const> mpTimeRotationVector;
    DataValueType mMultiplier = 1.0;
};

}


#endif // LOADRODCOMPONENT_H
