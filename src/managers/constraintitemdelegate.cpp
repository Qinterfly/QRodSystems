/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the ComboBoxItemDelegate class
 */

#include <QComboBox>
#include <QLineEdit>
#include "constraintitemdelegate.h"

using namespace QRS::Managers;
using namespace QRS::Core;

static const short skType = 0;
static const short skCoordinateSystem = 1;

ConstraintItemDelegate::ConstraintItemDelegate(ConstraintRodComponent const& constraintRodComponent, ConstraintTypeNames const& types,
                                               ConstraintCoordinateSystemNames const& coordinateSystems, QObject* parent)
    : QStyledItemDelegate(parent)
    , mConstraintRodComponent(constraintRodComponent)
    , mTypes(types)
    , mCoordinateSystems(coordinateSystems)
{

}

//! Create a comboBox to choose items
QWidget* ConstraintItemDelegate::createEditor(QWidget* pCell, const QStyleOptionViewItem& /* option */,
                                              const QModelIndex& index) const
{
    QComboBox* pEditor  = new QComboBox(pCell);
    switch (index.column())
    {
    case skType:
        for (auto const& item : mTypes)
        {
            QString currentText = index.model()->data(index, Qt::DisplayRole).toString();
            if (!mConstraintRodComponent.isConstraintExist(item.first) || !currentText.compare(item.second))
                pEditor->addItem(item.second, item.first);
        }
        break;
    case skCoordinateSystem:
        for (auto const& item : mCoordinateSystems)
            pEditor->addItem(item.second, item.first);
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
    QVariant oldData = index.data(Qt::UserRole);
    // Setting new data
    pModel->setData(index, text, Qt::DisplayRole);
    pModel->setData(index, pComboBox->currentData(Qt::UserRole).toInt(), Qt::UserRole);
    // Signaling old data
    int iRow = index.row();
    switch (index.column())
    {
    case skType:
        if (oldData.isNull())
            emit typeCreated(iRow);
        else
            emit typeChanged(iRow, (ConstraintRodComponent::ConstraintType)oldData.toInt());
        break;
    case skCoordinateSystem:
        emit coordinateSystemChanged(iRow);
        break;
    }
}

//! Set a geometry to render
void ConstraintItemDelegate::updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option,
                                                  const QModelIndex& /* index */) const
{
    pEditor->setGeometry(option.rect);
}
