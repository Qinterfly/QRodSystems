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

ScalarTableModel::ScalarTableModel(ScalarDataObject* pScalarDataObject, QWidget* parent)
    : QStandardItemModel(parent)
    , mpScalarDataObject(pScalarDataObject)
{
    setColumnCount(2);
    updateContent();
}

//! Represent all items contained in a scalar data object
void ScalarTableModel::updateContent()
{

}
