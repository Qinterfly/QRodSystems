/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the MatrixTableModel class
 */

#ifndef MATRIXTABLEMODEL_H
#define MATRIXTABLEMODEL_H

#include <QStandardItemModel>
#include "tablemodelinterface.h"

namespace QRS
{

namespace Core
{
class AbstractDataObject;
}

namespace TableModels
{

//! Table model to represent a matrix data object
class MatrixTableModel : public QStandardItemModel, public TableModelInterface
{
    Q_OBJECT

public:
    MatrixTableModel(QWidget* parent = nullptr);
    ~MatrixTableModel() = default;
    void setDataObject(Core::AbstractDataObject* pDataObject);
    bool setData(const QModelIndex& indexEdit, const QVariant& value, int role = Qt::EditRole) override;
    void insertItemAfterSelected(QItemSelectionModel* pSelectionModel) override;
    void insertLeadingItemAfterSelected(QItemSelectionModel* /*pSelectionModel*/) override { };
    void removeSelectedItem(QItemSelectionModel* pSelectionModel) override;
    void removeSelectedLeadingItem(QItemSelectionModel* /*pSelectionModel*/) override { };

private:
    void updateContent();
    void clearContent();

private:
    Core::AbstractDataObject* mpDataObject = nullptr;
};

}

}

#endif // MATRIXTABLEMODEL_H
