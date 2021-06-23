/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the AbstractCrossSectionRodComponent class
 */

#ifndef ABSTRACTCROSSSECTIONRODCOMPONENT_H
#define ABSTRACTCROSSSECTIONRODCOMPONENT_H

#include <QPointer>
#include "abstractrodcomponent.h"

namespace QRS::Core
{

class ScalarDataObject;

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
    QPointer<ScalarDataObject const> mpArea;
    // Inertia moments
    QPointer<ScalarDataObject const> mpInertiaMomentTorsional;
    QPointer<ScalarDataObject const> mpInertiaMomentX;
    QPointer<ScalarDataObject const> mpInertiaMomentY;
    QPointer<ScalarDataObject const> mpInertiaProductXY;
    // Center coordinates
    QPointer<ScalarDataObject const> mpCenterCoordinateX;
    QPointer<ScalarDataObject const> mpCenterCoordinateY;
};

}

#endif // ABSTRACTCROSSSECTIONRODCOMPONENT_H
