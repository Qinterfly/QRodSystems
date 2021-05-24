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
    if (nodeType == HierarchyNode::NodeType::kDirectory)
        setDirectoryAttributes();
    else
        setObjectAttributes();
}

//! Set directory characteristic attributes
void DataObjectsPropertiesModel::setDirectoryAttributes()
{
    // TODO
}

//! Set objects characteristic attributes
void DataObjectsPropertiesModel::setObjectAttributes()
{
    // Create the intersection flags
    bool isIntersectName = true;
    bool isIntersectType = true;
    bool isIntersectNumberItems = true;
    bool isIntersectIdentifier = true;
    // Check if properties intersect
    int numItems = mItems.size();
    AbstractDataObject* pCurrentDataObject = mItems[0]->mpDataObject;
    AbstractDataObject* pNextDataObject;
    for (int i = 0; i != numItems - 1; ++i)
    {
        pNextDataObject = mItems[i + 1]->mpDataObject;
        isIntersectName = isIntersectName && pCurrentDataObject->name() == pNextDataObject->name();
        isIntersectType = isIntersectType && pCurrentDataObject->type() == pNextDataObject->type();
        isIntersectNumberItems = isIntersectNumberItems && pCurrentDataObject->numberItems() == pNextDataObject->numberItems();
        isIntersectIdentifier = isIntersectIdentifier && pCurrentDataObject->id() == pNextDataObject->id();
        pCurrentDataObject = pNextDataObject;
    }
    // Set only common properties by flags
    QString const kNoCommon = "";
    QString name = isIntersectName ? pCurrentDataObject->name() : kNoCommon;
    QString type;
    QString numberEntities;
    if (isIntersectType)
    {
        switch (pCurrentDataObject->type())
        {
        case DataObjectType::kScalar:
            type = tr("Scalar");
            numberEntities = QString::number(1);
            break;
        case DataObjectType::kVector:
            type = tr("Vector");
            numberEntities = QString::number(3);
            break;
        case DataObjectType::kMatrix:
            type = tr("Matrix");
            numberEntities = QString::number(9);
            break;
        case DataObjectType::kSurface:
            type = tr("Surface");
            SurfaceDataObject* pSurfaceObject = (SurfaceDataObject*)pCurrentDataObject;
            numberEntities = QString::number(pSurfaceObject->numberLeadingItems());
            break;
        }
    }
    QString numberItems = isIntersectNumberItems ? QString::number(pCurrentDataObject->numberItems()) : kNoCommon;
    QString identifier = isIntersectIdentifier ? QString::number(pCurrentDataObject->id()) : kNoCommon;
    // Insert the properties into the table
    QStandardItem* pRoot = invisibleRootItem();
    pRoot->appendRow(preparePropertyRow(kName, tr("Name"), name, true));
    pRoot->appendRow(preparePropertyRow(kType, tr("Type"), type, false));
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
