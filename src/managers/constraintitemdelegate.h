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

//! Class to specify how options of a constraint can be edited
class ConstraintItemDelegate : public QStyledItemDelegate
{
public:
    ConstraintItemDelegate(Core::ConstraintRodComponent const& constraintRodComponent, QObject* parent = nullptr);
    QWidget* createEditor(QWidget* pCell, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* pEditor, const QModelIndex& index) const override;
    void setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    Core::ConstraintRodComponent const& mConstraintRodComponent;
    std::map<Core::ConstraintRodComponent::ConstraintType, QString> mTypes;
    std::map<Core::ConstraintRodComponent::ConstraintDirection, QString> mDirections;
    std::map<Core::ConstraintRodComponent::ConstraintCoordinateSystem, QString> mCoordinateSystems;
};

}

#endif // CONSTRAINTITEMDELEGATE_H
