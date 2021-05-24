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
class QTableView;
QT_END_NAMESPACE

namespace QRS
{

namespace HierarchyModels
{

class AbstractHierarchyItem;
class DataObjectsHierarchyItem;

//! Model to represent properties of selected data objects
class DataObjectsPropertiesModel : public QStandardItemModel
{
    enum AttributeType
    {
        kName,
        kType,
        kNumberItems,
        kNumberEntities,
        kID
    };

public:
    DataObjectsPropertiesModel(QTableView* pView, QVector<HierarchyModels::AbstractHierarchyItem*> items);

private:
    void setDirectoryAttributes();
    void setObjectAttributes();
    QList<QStandardItem*> preparePropertyRow(AttributeType type, QString const& title, QVariant const& value, bool isValueEditable) const;

private:
    QVector<HierarchyModels::DataObjectsHierarchyItem*> mItems;
};

}

}


#endif // DATAOBJECTSPROPERTIESMODEL_H
