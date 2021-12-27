/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the ConstraintRodComponent class
 */

#ifndef CONSTRAINTRODCOMPONENT_H
#define CONSTRAINTRODCOMPONENT_H

#include "abstractrodcomponent.h"

namespace QRS::Core
{

//! Component to restrict movements of a rod
class ConstraintRodComponent : public AbstractRodComponent
{
public:
    enum ConstraintType
    {
        kDisplacementX, kDisplacementY, kDisplacementZ,
        kRotationX, kRotationY, kRotationZ
    };
    enum ConstraintCoordinateSystem
    {
        kGlobal,
        kLocal
    };
    using Constraints = std::map<ConstraintType, ConstraintCoordinateSystem>;
    ConstraintRodComponent(QString const& name);
    ~ConstraintRodComponent();
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override { return mConstraints.size() != 0; };
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjects const& dataObjects) override;
    void resolveReferences(DataObjects const&) override {};
    bool isConstraintExist(ConstraintType type) const;
    void setConstraint(ConstraintType type, ConstraintCoordinateSystem coordinateSystem);
    bool removeConstraint(ConstraintType type);
    Constraints const& constraints() const { return mConstraints; }

private:
    static quint32 smNumInstances;
    Constraints mConstraints;
};

}


#endif // CONSTRAINTRODCOMPONENT_H
