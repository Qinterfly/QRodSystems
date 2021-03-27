/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of static functions of InterfaceTableModel
 */

#include <QStandardItem>
#include "interfacetablemodel.h"

//! Helper function to make an item which holds a double value
QStandardItem* InterfaceTableModel::makeDoubleItem(double value)
{
    QStandardItem* item = new QStandardItem;
    item->setData(value, Qt::UserRole);
    item->setData(QString::number(value, 'g', kNumShowPrecision), Qt::EditRole);
    return item;
}
