/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the BaseTableModel class
 */

#ifndef BASETABLEMODEL_H
#define BASETABLEMODEL_H

#include <QStandardItemModel>
#include "tablemodelinterface.h"

namespace QRS
{
class AbstractDataObject;
}

//! Table model to represent either a scalar or vector data object
class BaseTableModel : public QStandardItemModel, public TableModelInterface
{
    Q_OBJECT

public:
    BaseTableModel(QWidget* parent = nullptr);
    ~BaseTableModel() = default;
    void setDataObject(QRS::AbstractDataObject* pDataObject);
    bool setData(const QModelIndex& indexEdit, const QVariant& value, int role = Qt::EditRole) override;
    void insertItemAfterSelected(QItemSelectionModel* pSelectionModel) override;
    void insertLeadingItemAfterSelected(QItemSelectionModel* /*pSelectionModel*/) override { };
    void removeSelectedItem(QItemSelectionModel* pSelectionModel) override;
    void removeSelectedLeadingItem(QItemSelectionModel* /*pSelectionModel*/) override { };

private:
    void updateContent();
    void clearContent();

private:
    QRS::AbstractDataObject* mpDataObject = nullptr;
};

#endif // BASETABLEMODEL_H
