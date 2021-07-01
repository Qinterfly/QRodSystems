/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the AbstractHierarchyModel class
 */

#ifndef ABSTRACTHIERARCHYMODEL_H
#define ABSTRACTHIERARCHYMODEL_H

#include <QStandardItemModel>
#include "abstracthierarchyitem.h"

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
class HierarchyNode;
}

namespace HierarchyModels
{

using NodesState = std::unordered_map<Core::HierarchyNode*, bool>;

//! Hierarchy model which enables one to drag and drop elements of the same type
class AbstractHierarchyModel : public QStandardItemModel
{
    Q_OBJECT

public:
    AbstractHierarchyModel(QString const& mimeType, QTreeView* pView = nullptr);
    virtual ~AbstractHierarchyModel() = 0;
    virtual void updateContent() = 0;
    virtual void clearContent() = 0;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indicies) const override;
    bool dropMimeData(QMimeData const* pMimeData, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

signals:
    //! Emitted when hierarchical elements get renamed, moved or deleted
    void dataChanged();

private:
    bool processDropOnItem(QDataStream& stream, int& numItems, QModelIndex const& indexParent);
    bool processDropBetweenItems(QDataStream& stream, int& numItems, QModelIndex const& indexParent, int row);
    void retrieveExpandedState(NodesState& nodesState, QModelIndex const& indexParent, QTreeView const* pView);
    void setExpandedState(NodesState& nodesState, QModelIndex const& indexParent, QTreeView* pView);
    void updateContentExpanded();

protected:
    QString const mkMimeType;

};

}

}

#endif // ABSTRACTHIERARCHYMODEL_H
