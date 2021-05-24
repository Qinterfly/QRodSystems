/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsPropertiesModel class
 */

#include <QTreeView>
#include "dataobjectspropertiesmodel.h"
#include "abstracthierarchyitem.h"

using namespace QRS::PropertiesModels;
using namespace QRS::HierarchyModels;

DataObjectsPropertiesModel::DataObjectsPropertiesModel(QTreeView* pView, QVector<AbstractHierarchyItem*> items)
    : QStandardItemModel(pView)
    , mItems(items)
{
    // TODO
}
