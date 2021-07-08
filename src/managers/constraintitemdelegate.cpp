/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the ComboBoxItemDelegate class
 */

#include <QComboBox>
#include <QTableWidget>
#include <map>
#include "constraintitemdelegate.h"

using namespace QRS::Managers;
using namespace QRS::Core;

ConstraintItemDelegate::ConstraintItemDelegate(ConstraintRodComponent const& constraintRodComponent, QObject* parent)
    : QStyledItemDelegate(parent)
    , mConstraintRodComponent(constraintRodComponent)
{
    // Setting types
    mTypes[ConstraintRodComponent::kDisplacement] = "Displacement";
    mTypes[ConstraintRodComponent::kRotation] = "Rotation";
    // Setting directions
    mDirections[ConstraintRodComponent::kX] = "X";
    mDirections[ConstraintRodComponent::kY] = "Y";
    mDirections[ConstraintRodComponent::kZ] = "Z";
    // Setting directions
    mCoordinateSystems[ConstraintRodComponent::kGlobal] = "Global";
    mCoordinateSystems[ConstraintRodComponent::kLocal] = "Local";
}

//! Create a comboBox to choose items
QWidget* ConstraintItemDelegate::createEditor(QWidget* pCell, const QStyleOptionViewItem& /* option */,
                                              const QModelIndex& index) const
{
    QComboBox* pEditor  = new QComboBox(pCell);
    switch (index.column())
    {
    case 0:
        for (auto const& item : mTypes)
        {
            if (!mConstraintRodComponent.isConstraintFullySet(item.first))
                pEditor->addItem(item.second);
        }
        break;
    case 1:
    {
        QTableWidget* pTable = (QTableWidget*)parent();
        auto type = (ConstraintRodComponent::ConstraintType)pTable->item(index.row(), 0)->data(Qt::UserRole).toInt();
        for (auto const& item : mDirections)
        {
            if (!mConstraintRodComponent.isConstraintExist(type, item.first))
                pEditor->addItem(item.second);
        }
        break;
    }
    case 2:
        pEditor->addItem("Global");
        pEditor->addItem("Local");
        break;
    }
    pEditor->setFrame(false);
    return pEditor;
}

//! Specify data to show
void ConstraintItemDelegate::setEditorData(QWidget* pEditor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox* pComboBox = static_cast<QComboBox*>(pEditor);
    pComboBox->setCurrentText(text);
}

//! Set data to a model
void ConstraintItemDelegate::setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const
{
    QComboBox* pComboBox = static_cast<QComboBox*>(pEditor);
    QString text = pComboBox->currentText();
    pModel->setData(index, text, Qt::DisplayRole);
    pModel->setData(index, pComboBox->currentIndex(), Qt::UserRole);
}

//! Set a geometry to render
void ConstraintItemDelegate::updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option,
                                                  const QModelIndex& /* index */) const
{
    pEditor->setGeometry(option.rect);
}
