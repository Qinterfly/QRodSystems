/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the ScalarTableModel class
 */

#ifndef SCALARTABLEMODEL_H
#define SCALARTABLEMODEL_H

#include <QStandardItemModel>
#include "interfacetablemodel.h"

namespace QRS
{
class ScalarDataObject;
}

//! Table model to represent a scalar data object
class ScalarTableModel : public QStandardItemModel, public InterfaceTableModel
{
    Q_OBJECT

public:
    ScalarTableModel(QWidget* parent = nullptr);
    ~ScalarTableModel() = default;
    void setScalarDataObject(QRS::ScalarDataObject* pScalarDataObject);
    bool setData(const QModelIndex& indexEdit, const QVariant& value, int role = Qt::EditRole) override;
    void insertItemAfterSelected(QItemSelectionModel* selectionModel) override;
    void removeSelectedItem(QItemSelectionModel* selectionModel) override;

private:
    void updateContent();
    void clearContent();

private:
    QRS::ScalarDataObject* mpScalarDataObject = nullptr;
};


#endif // SCALARTABLEMODEL_H
