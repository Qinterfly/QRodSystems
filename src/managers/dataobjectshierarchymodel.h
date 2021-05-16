/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the DataObjectsHierarchyModel class
 */

#ifndef DATAOBJECTSHIERARCHYMODEL_H
#define DATAOBJECTSHIERARCHYMODEL_H

#include <QStandardItemModel>
#include <unordered_map>
#include "../core/datatypes.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QItemSelectionModel;
QT_END_NAMESPACE

namespace QRS
{
class HierarchyTree;
class AbstractDataObject;
}

using mapDataObjects = std::unordered_map<QRS::DataIDType, QRS::AbstractDataObject*>;

//! Tree model to represent and modify a hierarchy of data objects
class DataObjectsHierarchyModel : public QStandardItemModel
{
    Q_OBJECT

public:
    DataObjectsHierarchyModel(mapDataObjects& dataObjects, QRS::HierarchyTree& hierarchyDataObjects, QTreeView* pView = nullptr);
    ~DataObjectsHierarchyModel() = default;
    void updateContent();
    void clearContent();
    bool isEmpty() const;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    bool dropMimeData(QMimeData const* pMimeData, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
    QMimeData* mimeData(const QModelIndexList &indicies) const override;
    void selectItem(int iRow);

signals:
    void dataModified(bool flag);
    void dataObjectSelected(QRS::DataIDType id);
    void selectionCleared();

public slots:
    void retrieveSelectedDataObject();
    void removeSelectedItems();

private:
    bool processDropOnItem(QDataStream& stream, int& numItems, QModelIndex const& indexParent);
    bool processDropBetweenItems(QDataStream& stream, int& numItems, QModelIndex const& indexParent, int row);

private slots:
    void renameDataObject(QStandardItem* pStandardItem);

private:
    mapDataObjects& mDataObjects;
    QRS::HierarchyTree& mHierarchyDataObjects;
};

#endif // DATAOBJECTSHIERARCHYMODEL_H
