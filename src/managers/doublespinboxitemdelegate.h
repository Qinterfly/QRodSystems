/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the DoubleSpinBoxItemDelegate class
 */

#ifndef DOUBLESPINBOXITEMDELEGATE_H
#define DOUBLESPINBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace QRS::Managers
{

//! Class to specify how table values can be edited
class DoubleSpinBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DoubleSpinBoxItemDelegate(QObject* parent = nullptr);
    ~DoubleSpinBoxItemDelegate() = default;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* pEditor, const QModelIndex& index) const override;
    void setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* pEditor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

}


#endif // DOUBLESPINBOXITEMDELEGATE_H
