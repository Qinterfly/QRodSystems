/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
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
    void deserialize(QDataStream& stream, DataObjects const& dataObjects) override;
    void resolveReferences(DataObjects const& dataObjects) override;
    SectionType sectionType() const { return mkSectionType; }

protected:
    void copyIntegratedProperties(AbstractSectionRodComponent const* pSection);

protected:
    // Info
    SectionType const mkSectionType;
    static quint32 smNumInstances;
    // Area
    QPointer<ScalarDataObject const> mpArea;
    // Inertia moments
    QPointer<ScalarDataObject const> mpInertiaMomentTorsional;
    QPointer<ScalarDataObject const> mpInertiaMomentX;
    QPointer<ScalarDataObject const> mpInertiaMomentY;
    // Center coordinates
    QPointer<ScalarDataObject const> mpCenterCoordinateX;
    QPointer<ScalarDataObject const> mpCenterCoordinateY;
};

}

#endif // ABSTRACTSECTIONRODCOMPONENT_H
