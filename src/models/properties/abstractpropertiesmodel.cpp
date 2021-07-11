/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Defintion of the AbstractPropertiesModel class
 */

#include <QTableView>
#include "abstractpropertiesmodel.h"
#include "hierarchy/abstracthierarchyitem.h"
#include "core/hierarchynode.h"

using namespace QRS::PropertiesModels;
using namespace QRS::HierarchyModels;
using namespace QRS::Core;

AbstractPropertiesModel::AbstractPropertiesModel(QTableView* pView, QVector<HierarchyModels::AbstractHierarchyItem*> items)
    : QStandardItemModel(pView)
{
    if (items.isEmpty())
        return;
    for (AbstractHierarchyItem* pItem : items)
        mItems.push_back(pItem);
    // Check if the set of items consists of directories and objects
    int numItems = mItems.size();
    HierarchyNode::NodeType nodeType = mItems[0]->mpNode->type();
    for (int i = 1; i != numItems; ++i)
    {
        if (mItems[i]->mpNode->type() != nodeType)
            return;
    }
    // Determine the type of selection
    mIsDirectory = nodeType == HierarchyNode::NodeType::kDirectory;
    // Create connections
    connect(this, &AbstractPropertiesModel::itemChanged, this, &AbstractPropertiesModel::modifyProperty);
}

AbstractPropertiesModel::~AbstractPropertiesModel()
{

}

//! Set attributes of selected directories
void AbstractPropertiesModel::setDirectoryAttributes()
{
    // Create the intersection flags
    bool isSameName = true;
    bool isSameNumberChildren = true;
    // Check if properties intersect
    int numItems = mItems.size();
    HierarchyNode* pCurrentNode = mItems[0]->mpNode;
    HierarchyNode* pNextNode;
    for (int i = 0; i != numItems - 1; ++i)
    {
        pNextNode = mItems[i + 1]->mpNode;
        isSameName = isSameName && pCurrentNode->value() == pNextNode->value();
        isSameNumberChildren = isSameNumberChildren && pCurrentNode->numberChildren() == pNextNode->numberChildren();
        pCurrentNode = pNextNode;
    }
    // Set only common properties by flags
    QString name = isSameName ? pCurrentNode->value().toString() : mkEmptyProperty;
    QString numberChildren = isSameNumberChildren ? QString::number(pCurrentNode->numberChildren()) : mkEmptyProperty;
    // Insert the properties into the table
    QStandardItem* pRoot = invisibleRootItem();
    pRoot->appendRow(preparePropertyRow(PropertyDirectory::kName, tr("Name"), name, true));
    pRoot->appendRow(preparePropertyRow(PropertyDirectory::kNumberChildren, tr("Number of children"), numberChildren, false));
}


//! Change names of selected directories
void AbstractPropertiesModel::modifyDirectoryName(QString const& name)
{
    int numItems = mItems.size();
    AbstractHierarchyItem* pItem;
    for (int i = 0; i != numItems; ++i)
    {
        pItem = mItems[i];
        pItem->mpNode->value() = name;
        pItem->setText(name);
    }
}

//! Prepare a row to insert into the table
QList<QStandardItem*> AbstractPropertiesModel::preparePropertyRow(int type, QString const& title,
                                                                  QVariant const& value, bool isValueEditable) const
{
    int const kColor = 236;
    QColor const kAlternateColor = QColor(kColor, kColor, kColor);
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
