/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definition of the AbstractHierarchyModel class
 */

#include <QTreeView>
#include <QMimeData>
#include "abstracthierarchymodel.h"
#include "hierarchyitem.h"

AbstractHierarchyModel::AbstractHierarchyModel(QString const& mimeType, QTreeView* pView)
    : QStandardItemModel(pView)
    , kMimeType(mimeType)
{

}

AbstractHierarchyModel::~AbstractHierarchyModel()
{

}

//! Specify allowed drop actions
Qt::DropActions AbstractHierarchyModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

//! Specify allowed drag actions
Qt::DropActions AbstractHierarchyModel::supportedDragActions() const
{
    return Qt::CopyAction;
}

//! Retrieve the mime types
QStringList AbstractHierarchyModel::mimeTypes() const
{
    return QStringList() << kMimeType;
}

//! Encode each item according to a given list of indicies
QMimeData* AbstractHierarchyModel::mimeData(const QModelIndexList& indicies) const
{
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << indicies.count();
    for (QModelIndex const& index : indicies)
    {
        if (index.isValid())
        {
            HierarchyItem* pItem = (HierarchyItem*)itemFromIndex(index);
            pItem->write(stream);
        }
    }
    QMimeData* pMimeData = new QMimeData();
    pMimeData->setData(kMimeType, encodedData);
    return pMimeData;
}
