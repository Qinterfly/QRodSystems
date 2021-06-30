/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the AbstractSectionRodComponent class
 */

#ifndef ABSTRACTSECTIONRODCOMPONENT_H
#define ABSTRACTSECTIONRODCOMPONENT_H

#include <QPointer>
#include "abstractrodcomponent.h"

namespace QRS::Core
{

class ScalarDataObject;

//! General cross section of a rod
class AbstractSectionRodComponent : public AbstractRodComponent
{
public:
    enum SectionType
    {
        kUserDefined
    };
    AbstractSectionRodComponent(SectionType sectionType, QString const& name);
    virtual ~AbstractSectionRodComponent() = 0;
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjectGetter const& getDataObject) override;
    SectionType sectionType() const { return mSectionType; }

protected:
    void copyIntegratedProperties(AbstractSectionRodComponent const* pSection);

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

#endif // ABSTRACTSECTIONRODCOMPONENT_H