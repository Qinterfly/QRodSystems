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
    ScalarDataObject const* longitudinalFunction() const { return mpLongitudinalFunction; }
    ScalarDataObject const* timeCoefficient() const { return mpTimeCoefficient; }
    VectorDataObject const* timeRotationVector() const { return mpTimeRotationVector; }
    DataValueType multiplier() const { return mMultiplier; }
    bool isFollowing() const { return mIsFollowing; }
    // Setters
    void setType(LoadType type) { mLoadType = type; }
    void setDirectionVector(VectorDataObject const* pDirectionVector) { mpDirectionVector = pDirectionVector; }
    void setLongitudinalFunction(ScalarDataObject const* pLongitudinalFunction) { mpLongitudinalFunction = pLongitudinalFunction; }
    void setTimeCoefficient(ScalarDataObject const* pTimeCoefficient) { mpTimeCoefficient = pTimeCoefficient; }
    void setTimeRotationVector(VectorDataObject const* pTimeRotationVector) { mpTimeRotationVector = pTimeRotationVector; }
    void setMultiplier(DataValueType value) { mMultiplier = value; }
    void setFollowingState(bool isFollowing) { mIsFollowing = isFollowing; }

private:
    static quint32 smNumInstances;
    LoadType mLoadType = kNone;
    QPointer<VectorDataObject const> mpDirectionVector;
    QPointer<ScalarDataObject const> mpLongitudinalFunction;
    QPointer<ScalarDataObject const> mpTimeCoefficient;
    QPointer<VectorDataObject const> mpTimeRotationVector;
    DataValueType mMultiplier = 1.0;
    bool mIsFollowing = false;
};

}

#endif // LOADRODCOMPONENT_H
