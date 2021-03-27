/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the DoubleSpinBoxItemDelegate class
 */

#ifndef DOUBLESPINBOXITEMDELEGATE_H
#define DOUBLESPINBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

//! Class to set how table values can be edited
class DoubleSpinBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DoubleSpinBoxItemDelegate(QObject* parent = nullptr);
    ~DoubleSpinBoxItemDelegate() = default;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // DOUBLESPINBOXITEMDELEGATE_H
