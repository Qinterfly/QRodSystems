/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the DataObjectsPropertiesModel class
 */

#include <QTableView>
#include "core/abstractdataobject.h"
#include "core/surfacedataobject.h"
#include "core/hierarchynode.h"
#include "dataobjectspropertiesmodel.h"
#include "dataobjectshierarchyitem.h"

using namespace QRS::HierarchyModels;
using namespace QRS::Core;

DataObjectsPropertiesModel::DataObjectsPropertiesModel(QTableView* pView, QVector<AbstractHierarchyItem*> items)
    : QStandardItemModel(pView)
{
    if (items.isEmpty())
        return;
    for (AbstractHierarchyItem* pItem : items)
        mItems.push_back((DataObjectsHierarchyItem*)pItem);
    // Check if the set of items consists of directories and objects
    int numItems = mItems.size();
    HierarchyNode::NodeType nodeType = mItems[0]->mpNode->type();
    for (int i = 1; i != numItems; ++i)
    {
        if (mItems[i]->mpNode->type() != nodeType)
            return;
    }
    // Specify properties for all directories or objects
    bool isSingle = numItems == 1;
    if (nodeType == HierarchyNode::NodeType::kDirectory)
        setDirectoryAttributes(isSingle);
    else
        setObjectAttributes(isSingle);
}

//! Set directory characteristic attributes
void DataObjectsPropertiesModel::setDirectoryAttributes(bool isSingle)
{
    // TODO
}

//! Set objects characteristic attributes
void DataObjectsPropertiesModel::setObjectAttributes(bool isSingle)
{
    QStandardItem* pRoot = invisibleRootItem();
    QString name;
    QString objectType;
    QString numberItems;
    QString numberEntities;
    QString identifier;
    if (isSingle)
    {
        DataObjectsHierarchyItem* pItem = mItems[0];
        AbstractDataObject* pDataObject = pItem->mpDataObject;
        // Name
        name = pDataObject->name();
        // Object type and number of entities in each item
        switch (pDataObject->type())
        {
        case DataObjectType::kScalar:
            objectType = tr("Scalar");
            numberEntities = QString::number(1);
            break;
        case DataObjectType::kVector:
            objectType = tr("Vector");
            numberEntities = QString::number(3);
            break;
        case DataObjectType::kMatrix:
            objectType = tr("Matrix");
            numberEntities = QString::number(9);
            break;
        case DataObjectType::kSurface:
            objectType = tr("Surface");
            SurfaceDataObject* pSurfaceObject = (SurfaceDataObject*)pDataObject;
            numberEntities = QString::number(pSurfaceObject->numberLeadingItems());
            break;
        }
        // Number of items
        numberItems = QString::number(pDataObject->numberItems());
        // Identifier
        identifier = QString::number(pDataObject->id());
    }
    else
    {
        // TODO
    }
    pRoot->appendRow(preparePropertyRow(kName, tr("Name"), name, true));
    pRoot->appendRow(preparePropertyRow(kType, tr("Type"), objectType, false));
    pRoot->appendRow(preparePropertyRow(kNumberItems, tr("Number of items"), numberItems, false));
    pRoot->appendRow(preparePropertyRow(kNumberEntities, tr("Number of entities"), numberEntities, false));
    pRoot->appendRow(preparePropertyRow(kID, tr("Identifier"), identifier, false));
}


QList<QStandardItem*> DataObjectsPropertiesModel::preparePropertyRow(AttributeType type, QString const& title, QVariant const& value, bool isValueEditable) const
{
    QColor const kAlternateColor = QColor(236, 236, 236);
    QList<QStandardItem*> result;
    // Title
    QStandardItem* pTitleItem = new QStandardItem(title);
    pTitleItem->setFlags(Qt::ItemFlag::ItemIsEnabled);
    result.push_back(pTitleItem);
    // Value
    QStandardItem* pValueItem = new QStandardItem();
    pValueItem->setData(value, Qt::DisplayRole);
    pValueItem->setData(type, Qt::UserRole);
    if (!isValueEditable)
    {
        pValueItem->setFlags(pValueItem->flags() & (~Qt::ItemFlag::ItemIsEditable));
        pValueItem->setBackground(kAlternateColor);
    }
    result.push_back(pValueItem);
    return result;
}
