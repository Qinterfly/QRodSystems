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
    return { new QStandardItem(QString::number(key)), new QStandardItem(QString::number(value)) };
}

//! Represent all items contained in a scalar data object
void ScalarTableModel::updateContent()
{
    if (!mpScalarDataObject)
        return;
    QStandardItem* rootItem = invisibleRootItem();
    auto& mapScalars = mpScalarDataObject->getItems();
    for (auto& item : mapScalars)
    {
        rootItem->appendRow(prepareRow(item.first, item.second[0][0]));
    }
}

