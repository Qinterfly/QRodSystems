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
}

namespace PropertiesModels
{

//! Model to represent properties of selected data objects
class DataObjectsPropertiesModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum PropertyType
    {
        kName,
        kType,
        kNumberItems,
        kNumberEntities,
        kID,
        kNumberChildren
    };

    DataObjectsPropertiesModel(QTableView* pView, QVector<HierarchyModels::AbstractHierarchyItem*> items);

signals:
    void propertyChanged(bool flag);

private:
    void setDirectoryAttributes();
    void setObjectAttributes();
    QList<QStandardItem*> preparePropertyRow(PropertyType type, QString const& title, QVariant const& value, bool isValueEditable) const;

private slots:
    void modifyProperty(QStandardItem* pChangedProperty);

private:
    QVector<HierarchyModels::DataObjectsHierarchyItem*> mItems;
};

}

}


#endif // DATAOBJECTSPROPERTIESMODEL_H
