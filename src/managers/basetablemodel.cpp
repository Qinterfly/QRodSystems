/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the BaseTableModel class
 */

#include <QTreeView>

#include "basetablemodel.h"
#include "abstractdataobject.h"

using namespace QRS;

BaseTableModel::BaseTableModel(QWidget* parent)
    : QStandardItemModel(parent)
{

}

//! Set a data object to represent
void BaseTableModel::setDataObject(AbstractDataObject* pDataObject)
{
    mpDataObject = pDataObject;
    if (!mpDataObject)
    {
        clearContent();
        return;
    }
    switch (mpDataObject->type())
    {
    case kScalar:
        setColumnCount(2);
        setHorizontalHeaderLabels({"Key", "Value"});
        break;
    case kVector:
        setColumnCount(4);
        setHorizontalHeaderLabels({"Key", "Value 1", "Value 2", "Value 3"});
        break;
    default:
        break;
    }
    updateContent();
}

//! Represent all items which a data object contains
void BaseTableModel::updateContent()
{
    clearContent();
    if (!mpDataObject)
        return;
    QStandardItem* rootItem = invisibleRootItem();
    auto& map = mpDataObject->getItems();
    for (auto& iterator : map)
    {
        DataItemType& array = iterator.second;
        rootItem->appendRow(prepareRow(iterator.first, array, 0));
    }
}

//! Clear previously created items
void BaseTableModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Set the data acquired from a delegate
bool BaseTableModel::setData(const QModelIndex& indexEdit, const QVariant& value, int role)
{
    if (role != Qt::UserRole)
        return false;
    double key = data(index(indexEdit.row(), 0), Qt::UserRole).toDouble();
    double newValue = value.toDouble();
    bool isOkay = false;
    bool isSort = false;
    // Check whether a key or value was changed
    short iColumn = indexEdit.column();
    if (iColumn == 0)
    {
        isOkay = mpDataObject->changeItemKey(key, newValue);
        isSort = true;
    }
    else
    {
        isOkay = mpDataObject->setArrayValue(key, newValue, 0, iColumn - 1);
    }
    // Display the changed value
    if (isOkay)
    {
        QStandardItemModel::setData(indexEdit, value, Qt::UserRole);
        QStandardItemModel::setData(indexEdit, QString::number(newValue, 'g', kNumShowPrecision), Qt::EditRole);
        if (isSort)
            sort(0, Qt::AscendingOrder);
    }
    return isOkay;
}

//! Insert a new item after selected one
void BaseTableModel::insertItemAfterSelected(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    if (listSelected.isEmpty())
    {
        mpDataObject->addItem(0.0);
    }
    else
    {
        uint iRow;
        double key;
        for (QModelIndex& currentIndex : listSelected)
        {
            iRow = currentIndex.row();
            key = index(iRow, 0).data(Qt::UserRole).toDouble();
            mpDataObject->addItem(key);
        }
    }
    updateContent();
}

//! Remove an array under selection
void BaseTableModel::removeSelectedItem(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    uint iRow;
    double key;
    for (QModelIndex& ind : listSelected)
    {
        iRow = ind.row();
        key = index(iRow, 0).data(Qt::UserRole).toDouble();
        mpDataObject->removeItem(key);
    }
    updateContent();
}

