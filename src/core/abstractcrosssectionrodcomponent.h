/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the AbstractCrossSectionRodComponent class
 */

#ifndef ABSTRACTCROSSSECTIONRODCOMPONENT_H
#define ABSTRACTCROSSSECTIONRODCOMPONENT_H

#include "abstractrodcomponent.h"

namespace QRS::Core
{

//! General cross section of a rod
class AbstractCrossSectionRodComponent : public AbstractRodComponent
{
public:
    enum SectionType
    {
        kUserDefined
    };
    AbstractCrossSectionRodComponent(SectionType sectionType, QString const& name);
    virtual ~AbstractCrossSectionRodComponent() = 0;
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjectGetter const& getDataObject) override;

protected:
    void copyIntegratedProperties(AbstractCrossSectionRodComponent const* pCrossSection);

protected:
    // Info
    SectionType const mSectionType;
    static quint32 smNumInstances;
    // Area
    DataValueType mArea = 0.0;
    // Inertia moments
    DataValueType mInertiaMomentTorsional = 0.0;
    DataValueType mInertiaMomentX = 0.0;
    DataValueType mInertiaMomentY = 0.0;
    DataValueType mInertiaMomentXY = 0.0;
    // Center coordinates
    DataValueType mCenterCoordinateX = 0.0;
    DataValueType mCenterCoordinateY = 0.0;
    // Eccentricity
    DataValueType mEccentricityX = 0.0;
    DataValueType mEccentricityY = 0.0;
};

}

#endif // ABSTRACTCROSSSECTIONRODCOMPONENT_H
