/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of static functions of InterfaceTableModel
 */

#include <QStandardItem>
#include "interfacetablemodel.h"
#include "array.h"

//! Helper function to make an item which holds a double value
QStandardItem* InterfaceTableModel::makeDoubleItem(double value)
{
    QStandardItem* item = new QStandardItem;
    item->setData(value, Qt::UserRole);
    item->setData(QString::number(value, 'g', kNumShowPrecision), Qt::EditRole);
    return item;
}

//! Helper function to copy a row from an array
QList<QStandardItem*> InterfaceTableModel::prepareRow(QRS::Array<double>& array, uint iRow)
{
    QList<QStandardItem*> resultList;
    uint nCols = array.cols();
    for (uint j = 0; j != nCols; ++j)
        resultList.push_back(makeDoubleItem(array[iRow][j]));
    return resultList;
}

//! Helper function to copy a row from an array and associate it with an key
QList<QStandardItem*> InterfaceTableModel::prepareRow(double const& key, QRS::Array<double>& array, uint iRow)
{
    QList<QStandardItem*> resultList = prepareRow(array, iRow);
    resultList.push_front(makeDoubleItem(key));
    return resultList;
}

//! Helper function to copy a row from an array and associate it with a name
QList<QStandardItem*> InterfaceTableModel::prepareRow(QString const& name, QRS::Array<double>& array, uint iRow)
{
    QList<QStandardItem*> resultList = prepareRow(array, iRow);
    QStandardItem* itemName = new QStandardItem(name);
    itemName->setFlags(Qt::NoItemFlags);
    resultList.push_front(itemName);
    return resultList;
}
