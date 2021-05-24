/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsPropertiesModel class
 */

#ifndef DATAOBJECTSPROPERTIESMODEL_H
#define DATAOBJECTSPROPERTIESMODEL_H

#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

namespace QRS
{

namespace HierarchyModels
{
class AbstractHierarchyItem;
}

namespace PropertiesModels
{

//! Model to represent properties of selected data objects
class DataObjectsPropertiesModel : public QStandardItemModel
{
public:
    DataObjectsPropertiesModel(QTreeView* pView, QVector<HierarchyModels::AbstractHierarchyItem*> items);

private:
    QVector<HierarchyModels::AbstractHierarchyItem*> mItems;
};

}

}


#endif // DATAOBJECTSPROPERTIESMODEL_H
