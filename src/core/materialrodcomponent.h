/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the MaterialRodComponent class
 */

#ifndef MATERIALRODCOMPONENT_H
#define MATERIALRODCOMPONENT_H

#include <QPointer>
#include "abstractrodcomponent.h"

namespace QRS::Core
{

class ScalarDataObject;

//! Material properties of a rod
class MaterialRodComponent : public AbstractRodComponent
{
public:
    MaterialRodComponent(QString const& name);
    ~MaterialRodComponent();
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override;
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjectGetter const& getDataObject) override;
    // Getters
    ScalarDataObject const* elasticModulus() const { return mpElasticModulus; }
    ScalarDataObject const* shearModulus() const { return mpShearModulus; }
    ScalarDataObject const* poissonsRatio() const { return mpPoissonsRatio; }
    ScalarDataObject const* density() const { return mpDensity; }
    // Setters
    void setElasticModulus(ScalarDataObject const* pElasticModulus) { mpElasticModulus = pElasticModulus; }
    void setShearModulus(ScalarDataObject const* pShearModulus) { mpShearModulus = pShearModulus; }
    void setPoissonsRatio(ScalarDataObject const* pPoissonsRatio) { mpPoissonsRatio = pPoissonsRatio; }
    void setDensity(ScalarDataObject const* pDensity) { mpDensity = pDensity; }

private:
    static quint32 smNumInstances;
    QPointer<ScalarDataObject const> mpElasticModulus;
    QPointer<ScalarDataObject const> mpShearModulus;
    QPointer<ScalarDataObject const> mpPoissonsRatio;
    QPointer<ScalarDataObject const> mpDensity;
};

}

#endif // MATERIALRODCOMPONENT_H
