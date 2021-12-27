/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the MechanicalRodComponent class
 */

#ifndef MECHANICALRODCOMPONENT_H
#define MECHANICALRODCOMPONENT_H

#include <QPointer>
#include "abstractrodcomponent.h"

namespace QRS::Core
{

class ScalarDataObject;

//! Stiffness and mass distributions of a rod
class MechanicalRodComponent : public AbstractRodComponent
{
public:
    MechanicalRodComponent(QString const& name);
    ~MechanicalRodComponent();
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override { return true; }
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjects const& dataObjects) override;
    void resolveReferences(DataObjects const& dataObjects) override;
    // Getters
    // Stiffness distribution
    ScalarDataObject const* tensionStiffness() const { return mpTensionStiffness; }
    ScalarDataObject const* torsionalStiffness() const { return mpTorsionalStiffness; }
    ScalarDataObject const* bendingStiffnessX() const { return mpBendingStiffnessX; }
    ScalarDataObject const* bendingStiffnessY() const { return mpBendingStiffnessY; }
    // Mass distribution
    ScalarDataObject const* linearMassDensity() const { return mpLinearMassDensity; }
    ScalarDataObject const* inertiaMassMomentX() const { return mpInertiaMassMomentX; }
    ScalarDataObject const* inertiaMassMomentY() const { return mpInertiaMassMomentY; }
    ScalarDataObject const* inertiaMassMomentZ() const { return mpInertiaMassMomentZ; }
    // Eccentricity
    ScalarDataObject const* eccentricityX() const { return mpEccentricityX; }
    ScalarDataObject const* eccentricityY() const { return mpEccentricityY; }
    // Contact diameter
    ScalarDataObject const* contactDiameter() const { return mpContactDiameter; }
    // Setters
    // Stiffness distribution
    void setTensionStiffness(ScalarDataObject const* pTensionStiffness) { mpTensionStiffness = pTensionStiffness; }
    void setTorsionalStiffness(ScalarDataObject const* pTorsionalStiffness) { mpTorsionalStiffness = pTorsionalStiffness; }
    void setBendingStiffnessX(ScalarDataObject const* pBendingStiffnessX) { mpBendingStiffnessX = pBendingStiffnessX; }
    void setBendingStiffnessY(ScalarDataObject const* pBendingStiffnessY) { mpBendingStiffnessY = pBendingStiffnessY; }
    // Mass distribution
    void setLinearMassDensity(ScalarDataObject const* pLinearMassDensity) { mpLinearMassDensity = pLinearMassDensity; }
    void setInertiaMassMomentX(ScalarDataObject const* pInertiaMassMomentX) { mpInertiaMassMomentX = pInertiaMassMomentX; }
    void setInertiaMassMomentY(ScalarDataObject const* pInertiaMassMomentY) { mpInertiaMassMomentY = pInertiaMassMomentY; }
    void setInertiaMassMomentZ(ScalarDataObject const* pInertiaMassMomentZ) { mpInertiaMassMomentZ = pInertiaMassMomentZ; }
    // Eccentricity
    void setEccentricityX(ScalarDataObject const* pEccentricityX) { mpEccentricityX = pEccentricityX; }
    void setEccentricityY(ScalarDataObject const* pEccentricityY) { mpEccentricityY = pEccentricityY; }
    // Contact diameter
    void setContactDiameter(ScalarDataObject const* pContactDiameter) { mpContactDiameter = pContactDiameter; }

private:
    static quint32 smNumInstances;
    // Stiffness distribution
    QPointer<ScalarDataObject const> mpTensionStiffness;
    QPointer<ScalarDataObject const> mpTorsionalStiffness;
    QPointer<ScalarDataObject const> mpBendingStiffnessX;
    QPointer<ScalarDataObject const> mpBendingStiffnessY;
    // Mass distribution
    QPointer<ScalarDataObject const> mpLinearMassDensity;
    QPointer<ScalarDataObject const> mpInertiaMassMomentX;
    QPointer<ScalarDataObject const> mpInertiaMassMomentY;
    QPointer<ScalarDataObject const> mpInertiaMassMomentZ;
    // Eccentricity
    QPointer<ScalarDataObject const> mpEccentricityX;
    QPointer<ScalarDataObject const> mpEccentricityY;
    // Contact diameter
    QPointer<ScalarDataObject const> mpContactDiameter;
};

}

#endif // MECHANICALRODCOMPONENT_H
