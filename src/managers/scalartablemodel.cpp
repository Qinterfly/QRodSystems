/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the ScalarTableModel class
 */

#include "scalartablemodel.h"
#include "scalardataobject.h"

#include <QTreeView>

using namespace QRS;

ScalarTableModel::ScalarTableModel(QWidget* parent)
    : QStandardItemModel(parent)
{
    setColumnCount(2);
    setHorizontalHeaderLabels({"Key", "Value"});
}

//! Set a scalar data object to represent
void ScalarTableModel::setScalarDataObject(QRS::ScalarDataObject* pScalarDataObject)
{
    ScalarTableModel::mpScalarDataObject = pScalarDataObject;
    updateContent();
}

//! Helper function to prepare row
QList<QStandardItem*> prepareRow(double const& key, double const& value)
{
    return { InterfaceTableModel::makeDoubleItem(key),
             InterfaceTableModel::makeDoubleItem(value) };
}

//! Represent all items contained in a scalar data object
void ScalarTableModel::updateContent()
{
    clearContent();
    if (!mpScalarDataObject)
        return;
    QStandardItem* rootItem = invisibleRootItem();
    auto& mapScalars = mpScalarDataObject->getItems();
    for (auto& item : mapScalars)
    {
        rootItem->appendRow(prepareRow(item.first, item.second[0][0]));
    }
}

//! Clear previously created items
void ScalarTableModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Set the data acquired from a delegate
bool ScalarTableModel::setData(const QModelIndex& indexEdit, const QVariant& value, int role)
{
    if (role != Qt::UserRole)
        return false;
    double key = data(index(indexEdit.row(), 0), Qt::UserRole).toDouble();
    double doubleValue = value.toDouble();
    bool isOkay = false;
    bool isSort = false;
    // Check whether a key or value was changed
    switch (indexEdit.column())
    {
    case 0:
    {
        double newKey = value.toDouble();
        isOkay = mpScalarDataObject->changeItemKey(key, newKey);
        isSort = true;
        break;
    }
    case 1:
        isOkay = mpScalarDataObject->setArrayValue(key, doubleValue);
        break;
    }
    // Display the changed value
    if (isOkay)
    {
        QStandardItemModel::setData(indexEdit, value, Qt::UserRole);
        QStandardItemModel::setData(indexEdit, QString::number(doubleValue, 'g', kNumShowPrecision), Qt::EditRole);
        if (isSort)
            sort(0, Qt::AscendingOrder);
    }
    return isOkay;
}

//! Insert a new item after selected one
void ScalarTableModel::insertItemAfterSelected(QItemSelectionModel* selectionModel)
{
    QModelIndexList listSelected = selectionModel->selectedRows();
    if (listSelected.isEmpty())
    {
        mpScalarDataObject->addItem(0.0);
    }
    else
    {
        QModelIndex& lastSelected = *listSelected.rbegin();
        uint iRow = lastSelected.row();
        double key = index(iRow, 0).data(Qt::UserRole).toDouble();
        mpScalarDataObject->addItem(key);
    }
    updateContent();
}

//! Remove an array under selection
void ScalarTableModel::removeSelectedItem(QItemSelectionModel* selectionModel)
{
    QModelIndexList listSelected = selectionModel->selectedRows();
    uint iRow;
    double key;
    for (QModelIndex& ind : listSelected)
    {
        iRow = ind.row();
        key = index(iRow, 0).data(Qt::UserRole).toDouble();
        mpScalarDataObject->removeItem(key);
    }
    updateContent();
}

