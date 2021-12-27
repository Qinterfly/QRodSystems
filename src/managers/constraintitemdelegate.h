/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the ComboBoxItemDelegate class
 */

#ifndef CONSTRAINTITEMDELEGATE_H
#define CONSTRAINTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "core/constraintrodcomponent.h"

namespace QRS::Managers
{

using ConstraintTypeNames = std::map<Core::ConstraintRodComponent::ConstraintType, QString>;
using ConstraintCoordinateSystemNames = std::map<Core::ConstraintRodComponent::ConstraintCoordinateSystem, QString>;

//! Class to specify how options of a constraint can be edited
class ConstraintItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ConstraintItemDelegate(Core::ConstraintRodComponent const& constraintRodComponent, ConstraintTypeNames const& types,
                           ConstraintCoordinateSystemNames const& coordinateSystems, QObject* parent = nullptr);
    QWidget* createEditor(QWidget* pCell, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* pEditor, const QModelIndex& index) const override;
    void setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

signals:
    void typeCreated(int iRow) const;
    void typeChanged(int iRow, Core::ConstraintRodComponent::ConstraintType oldType) const;
    void coordinateSystemChanged(int iRow) const;

private:
    Core::ConstraintRodComponent const& mConstraintRodComponent;
    ConstraintTypeNames const& mTypes;
    ConstraintCoordinateSystemNames const& mCoordinateSystems;
};

}

#endif // CONSTRAINTITEMDELEGATE_H
