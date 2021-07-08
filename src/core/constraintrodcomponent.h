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
        kDisplacement,
        kRotation
    };
    enum ConstraintDirection
    {
        kX,
        kY,
        kZ
    };
    enum ConstraintCoordinateSystem
    {
        kGlobal,
        kLocal
    };
    using DirectionConstraints = std::unordered_map<ConstraintDirection, ConstraintCoordinateSystem>;
    using Constraints = std::unordered_map<ConstraintType, DirectionConstraints>;
    ConstraintRodComponent(QString const& name);
    ~ConstraintRodComponent();
    AbstractRodComponent* clone() const override;
    bool isDataComplete() const override { return mConstraints.size() != 0; };
    static quint32 numberInstances() { return smNumInstances; }
    void serialize(QDataStream& stream) const override;
    void deserialize(QDataStream& stream, DataObjects const& dataObjects) override;
    void resolveReferences(DataObjects const&) override {};
    bool isConstraintFullySet(ConstraintType type) const;
    bool isConstraintExist(ConstraintType type, ConstraintDirection direction) const;
    void setConstraint(ConstraintType type, ConstraintDirection direction, ConstraintCoordinateSystem coordinateSystem);

private:
    static quint32 smNumInstances;
    Constraints mConstraints;
};

}


#endif // CONSTRAINTRODCOMPONENT_H
