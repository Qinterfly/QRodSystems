/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Implementation of the SurfaceTableModel class
 */

#include <QTreeView>

#include "surfacetablemodel.h"
#include "core/surfacedataobject.h"

using namespace QRS::TableModels;
using namespace QRS::Core;

SurfaceTableModel::SurfaceTableModel(QWidget* parent)
    : QStandardItemModel(parent)
{
    setSortRole(Qt::UserRole);
}

//! Set a surface data object to represent
void SurfaceTableModel::setDataObject(SurfaceDataObject* pDataObject)
{
    mpDataObject = pDataObject;
    if (!mpDataObject)
    {
        clearContent();
        return;
    }
    updateContent();
}

//! Represent all items which a data object contains
void SurfaceTableModel::updateContent()
{
    clearContent();
    if (!mpDataObject)
        return;
    QStandardItem* rootItem = invisibleRootItem();
    auto& mapLeadingItems = mpDataObject->getLeadingItems();
    // Creating a header
    QList<QStandardItem*> header;
    header.push_back(makeLabelItem("XY"));
    for (auto& leadingItem : mapLeadingItems)
    {
        header.push_back(makeDoubleItem(leadingItem.first));
    }
    rootItem->appendRow(header);
    // Setting row items
    auto& mapItems = mpDataObject->getItems();
    for (auto& rowItem : mapItems)
    {
        DataItemType const& array = rowItem.second;
        rootItem->appendRow(prepareRow(rowItem.first, array, 0));
    }
}

//! Clear previously created items
void SurfaceTableModel::clearContent()
{
    removeRows(0, rowCount());
    removeColumns(0, columnCount());
}

//! Set the data acquired from a delegate
bool SurfaceTableModel::setData(const QModelIndex& indexEdit, const QVariant& value, int role)
{
    if (role != Qt::UserRole)
        return false;
    int iRow = indexEdit.row();
    double currentValue = data(indexEdit, Qt::UserRole).toDouble();
    double newValue = value.toDouble();
    bool isOkay = false;
    // Changing a leading key
    if (iRow == 0)
    {
        isOkay = mpDataObject->changeLeadingItemKey(currentValue, newValue);
    }
    else
    {
        int iColumn = indexEdit.column();
        // Check whether a key or value was changed
        if (iColumn == 0)
        {
            isOkay = mpDataObject->changeItemKey(currentValue, newValue);
        }
        else
        {
            double key = data(index(iRow, 0), Qt::UserRole).toDouble();
            isOkay = mpDataObject->setArrayValue(key, newValue, 0, iColumn - 1);
        }
    }
    // Display the results
    if (isOkay)
        updateContent();
    return isOkay;
}

//! Insert a new item after selected one
void SurfaceTableModel::insertItemAfterSelected(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    if (listSelected.isEmpty())
    {
        mpDataObject->addItem(0.0);
    }
    else
    {
        quint32 iRow;
        double key;
        for (QModelIndex& currentIndex : listSelected)
        {
            iRow = currentIndex.row();
            if (iRow != 0)
            {
                key = index(iRow, 0).data(Qt::UserRole).toDouble();
                mpDataObject->addItem(key);
            }
        }
    }
    updateContent();
}

//! Remove an array under selection
void SurfaceTableModel::removeSelectedItem(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    quint32 iRow;
    double key;
    for (QModelIndex& currentIndex : listSelected)
    {
        iRow = currentIndex.row();
        if (iRow != 0)
        {
            key = index(iRow, 0).data(Qt::UserRole).toDouble();
            mpDataObject->removeItem(key);
        }
    }
    updateContent();
}

//! Add a new leading item after selected one
void SurfaceTableModel::insertLeadingItemAfterSelected(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    int iColumn;
    double key;
    for (QModelIndex& currentIndex : listSelected)
    {
        iColumn = currentIndex.column();
        if (iColumn > 0)
        {
            key = index(0, iColumn).data(Qt::UserRole).toDouble();
            mpDataObject->addLeadingItem(key);
        }
    }
    updateContent();
}

//! Remove a selected leading item
void SurfaceTableModel::removeSelectedLeadingItem(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    quint32 iColumn;
    double key;
    for (QModelIndex& currentIndex : listSelected)
    {
        iColumn = currentIndex.column();
        if (iColumn > 0)
        {
            key = index(0, iColumn).data(Qt::UserRole).toDouble();
            mpDataObject->removeLeadingItem(key);
        }
    }
    updateContent();
}
