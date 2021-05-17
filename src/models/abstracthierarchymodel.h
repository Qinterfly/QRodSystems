/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the AbstractHierarchyModel class
 */

#ifndef ABSTRACTHIERARCHYMODEL_H
#define ABSTRACTHIERARCHYMODEL_H

#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

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
    QMimeData* mimeData(const QModelIndexList &indicies) const override;

signals:
    void dataModified(bool flag);

protected:
    QString const kMimeType;
};

#endif // ABSTRACTHIERARCHYMODEL_H
