/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the DoubleSpinBoxItemDelegate class
 */

#include "doublespinboxitemdelegate.h"

#include <QDoubleSpinBox>

DoubleSpinBoxItemDelegate::DoubleSpinBoxItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{

}

//! Create a double value editor
QWidget* DoubleSpinBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    const short kNumDecimals = 15;
    QDoubleSpinBox* editor  = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(std::numeric_limits<double>::min());
    editor->setMaximum(std::numeric_limits<double>::max());
    editor->setDecimals(kNumDecimals);
    return editor;
}

//! Specify data to show
void DoubleSpinBoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    double value = index.model()->data(index, Qt::UserRole).toDouble();
    QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

//! Set data to a model
void DoubleSpinBoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    model->setData(index, value, Qt::UserRole);
}

//! Set a geometry to render
void DoubleSpinBoxItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
