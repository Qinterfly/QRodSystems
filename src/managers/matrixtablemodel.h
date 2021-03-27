/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the MatrixTableModel class
 */

#ifndef MATRIXTABLEMODEL_H
#define MATRIXTABLEMODEL_H

#include <QStandardItemModel>
#include "interfacetablemodel.h"

namespace QRS
{
class AbstractDataObject;
}

//! Table model to represent a matrix data object
class MatrixTableModel : public QStandardItemModel, public InterfaceTableModel
{
    Q_OBJECT

public:
    MatrixTableModel(QWidget* parent = nullptr);
    ~MatrixTableModel() = default;
    void setDataObject(QRS::AbstractDataObject* pDataObject);
    bool setData(const QModelIndex& indexEdit, const QVariant& value, int role = Qt::EditRole) override;
    void insertItemAfterSelected(QItemSelectionModel* selectionModel) override;
    void removeSelectedItem(QItemSelectionModel* selectionModel) override;

private:
    void updateContent();
    void clearContent();

private:
    QRS::AbstractDataObject* mpDataObject = nullptr;
};

#endif // MATRIXTABLEMODEL_H
