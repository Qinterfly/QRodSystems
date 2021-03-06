/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Implementation of the MatrixTableModel class
 */

#include <QTreeView>

#include "matrixtablemodel.h"
#include "core/abstractdataobject.h"

using namespace QRS::TableModels;
using namespace QRS::Core;

MatrixTableModel::MatrixTableModel(QWidget* parent)
    : QStandardItemModel(parent)
{
    setSortRole(Qt::UserRole);
    setColumnCount(3);
    setHorizontalHeaderLabels({"Key", "Column 1", "Column 2", "Column 3"});
}

//! Set a data object to represent
void MatrixTableModel::setDataObject(AbstractDataObject* pDataObject)
{
    mpDataObject = pDataObject;
    if (!mpDataObject)
    {
        clearContent();
        return;
    }
    updateContent();
}

//! Represent all items which a vector data object contains
void MatrixTableModel::updateContent()
{
    clearContent();
    if (!mpDataObject)
        return;
    QStandardItem* rootItem = invisibleRootItem();
    auto& mapMatrices = mpDataObject->getItems();
    for (auto& iterator : mapMatrices)
    {
        QStandardItem* keyItem = TableModelInterface::makeDoubleItem(iterator.first);
        rootItem->appendRow(keyItem);
        DataItemType const& array = iterator.second;
        quint32 nRows = array.rows();
        quint32 iRow = keyItem->row();
        for (quint32 i = 0; i != nRows; ++i)
            keyItem->appendRow(prepareRow(QString(), array, i));
        // Forbid to modify an array header
        for (quint32 j = 1; j != 4; ++j)
        {
            QStandardItem* arrayHeaderItem = new QStandardItem();
            arrayHeaderItem->setFlags(Qt::NoItemFlags);
            setItem(iRow, j, arrayHeaderItem);
        }
    }
}

//! Clear previously created items
void MatrixTableModel::clearContent()
{
    removeRows(0, rowCount());
}

//! Set the data acquired from a delegate
bool MatrixTableModel::setData(const QModelIndex& indexEdit, const QVariant& value, int role)
{
    if (role != Qt::UserRole)
        return false;
    // Retrieving a key
    int iParentRow = indexEdit.parent().row();
    bool isKeyEdited = iParentRow < 0;
    int iKeyRow;
    if (isKeyEdited)
        iKeyRow = indexEdit.row();
    else
        iKeyRow = iParentRow;
    double key = data(index(iKeyRow, 0), Qt::UserRole).toDouble();
    double newValue = value.toDouble();
    bool isOkay = false;
    bool isSort = false;
    // Check whether a key or value was changed
    short iColumn = indexEdit.column();
    if (isKeyEdited)
    {
        isOkay = mpDataObject->changeItemKey(key, newValue);
        isSort = true;
    }
    else
    {
        isOkay = mpDataObject->setArrayValue(key, newValue, indexEdit.row(), iColumn - 1);
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
void MatrixTableModel::insertItemAfterSelected(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    if (listSelected.isEmpty())
    {
        mpDataObject->addItem(0.0);
    }
    else
    {
        for (QModelIndex& currentIndex : listSelected)
        {
            // If it is a parent
            if (currentIndex.parent().row() < 0)
            {
                quint32 iRow = currentIndex.row();
                double key = index(iRow, 0).data(Qt::UserRole).toDouble();
                mpDataObject->addItem(key);
            }
        }
    }
    updateContent();
}

//! Remove an array under selection
void MatrixTableModel::removeSelectedItem(QItemSelectionModel* pSelectionModel)
{
    QModelIndexList listSelected = pSelectionModel->selectedIndexes();
    QStandardItem* itemTable;
    for (QModelIndex& currentIndex : listSelected)
    {
        itemTable = itemFromIndex(currentIndex);
        // If it is a parent
        if (itemTable->rowCount() > 0)
        {
            quint32 iRow = currentIndex.row();
            double key = index(iRow, 0).data(Qt::UserRole).toDouble();
            mpDataObject->removeItem(key);
        }
    }
    updateContent();
}
