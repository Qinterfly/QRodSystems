/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Implementation of static functions of TableModelInterface
 */

#include <QStandardItem>
#include "tablemodelinterface.h"
#include "core/array.h"

using namespace QRS::TableModels;
using namespace QRS::Core;

//! Helper function to make an item which holds a double value
QStandardItem* TableModelInterface::makeDoubleItem(double value)
{
    QStandardItem* item = new QStandardItem;
    item->setData(value, Qt::UserRole);
    item->setData(QString::number(value, 'g', kNumShowPrecision), Qt::EditRole);
    return item;
}

//! Helper function to create an item which holds a string and cannot be modfifed
QStandardItem* TableModelInterface::makeLabelItem(QString const& name)
{
    QStandardItem* itemName = new QStandardItem(name);
    itemName->setFlags(Qt::NoItemFlags);
    return itemName;
}

//! Helper function to copy a row from an array
QList<QStandardItem*> TableModelInterface::prepareRow(Array<double>& array, quint32 iRow)
{
    QList<QStandardItem*> resultList;
    quint32 nCols = array.cols();
    for (quint32 j = 0; j != nCols; ++j)
        resultList.push_back(makeDoubleItem(array[iRow][j]));
    return resultList;
}

//! Helper function to copy a row from an array and associate it with an key
QList<QStandardItem*> TableModelInterface::prepareRow(double const& key, Array<double>& array, quint32 iRow)
{
    QList<QStandardItem*> resultList = prepareRow(array, iRow);
    resultList.push_front(makeDoubleItem(key));
    return resultList;
}

//! Helper function to copy a row from an array and associate it with a name
QList<QStandardItem*> TableModelInterface::prepareRow(QString const& name, Array<double>& array, quint32 iRow)
{
    QList<QStandardItem*> resultList = prepareRow(array, iRow);
    resultList.push_front(makeLabelItem(name));
    return resultList;
}
