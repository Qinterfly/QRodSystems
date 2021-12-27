/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the DoubleSpinBoxItemDelegate class
 */

#include <QDoubleSpinBox>
#include "doublespinboxitemdelegate.h"

using namespace QRS::Managers;

DoubleSpinBoxItemDelegate::DoubleSpinBoxItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{

}

//! Create a double value editor
QWidget* DoubleSpinBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */,
                                                 const QModelIndex& /* index */) const
{
    const short kNumDecimals = 15;
    QDoubleSpinBox* pEditor  = new QDoubleSpinBox(parent);
    pEditor->setFrame(false);
    pEditor->setMinimum(std::numeric_limits<double>::min());
    pEditor->setMaximum(std::numeric_limits<double>::max());
    pEditor->setDecimals(kNumDecimals);
    return pEditor;
}

//! Specify data to show
void DoubleSpinBoxItemDelegate::setEditorData(QWidget* pEditor, const QModelIndex& index) const
{
    double value = index.model()->data(index, Qt::UserRole).toDouble();
    QDoubleSpinBox* pSpinBox = static_cast<QDoubleSpinBox*>(pEditor);
    pSpinBox->setValue(value);
}

//! Set data to a model
void DoubleSpinBoxItemDelegate::setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const
{
    QDoubleSpinBox* pSpinBox = static_cast<QDoubleSpinBox*>(pEditor);
    pSpinBox->interpretText();
    double value = pSpinBox->value();
    pModel->setData(index, value, Qt::UserRole);
}

//! Set a geometry to render
void DoubleSpinBoxItemDelegate::updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option,
                                                     const QModelIndex& /* index */) const
{
    pEditor->setGeometry(option.rect);
}
