/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the DataObjectsPropertiesModel class
 */

#ifndef DATAOBJECTSPROPERTIESMODEL_H
#define DATAOBJECTSPROPERTIESMODEL_H

#include "abstractpropertiesmodel.h"

QT_BEGIN_NAMESPACE
class QTableView;
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
class DataObjectsPropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT

public:
    DataObjectsPropertiesModel(QTableView* pView, QVector<HierarchyModels::AbstractHierarchyItem*> items);

protected slots:
    void modifyProperty(QStandardItem* pChangedProperty) override;

private:
    enum PropertyDataObject
    {
        kName,
        kType,
        kNumberItems,
        kNumberEntities,
        kID
    };
    void setObjectAttributes();
};

}

}

#endif // DATAOBJECTSPROPERTIESMODEL_H
