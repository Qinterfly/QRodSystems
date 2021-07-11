/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the AbstractPropertiesModel class
 */

#ifndef ABSTRACTPROPERTIESMODEL_H
#define ABSTRACTPROPERTIESMODEL_H

#include <QStandardItemModel>

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

//! Model to represent general properties
class AbstractPropertiesModel : public QStandardItemModel
{
    Q_OBJECT

public:
    AbstractPropertiesModel(QTableView* pView, QVector<HierarchyModels::AbstractHierarchyItem*> items);
    virtual ~AbstractPropertiesModel() = 0;

signals:
    void propertyChanged();

protected slots:
    virtual void modifyProperty(QStandardItem* pChangedProperty) = 0;
    void modifyDirectoryName(QString const& name);

protected:
    void setDirectoryAttributes();
    QList<QStandardItem*> preparePropertyRow(int type, QString const& title, QVariant const& value, bool isValueEditable) const;

protected:
    QVector<HierarchyModels::AbstractHierarchyItem*> mItems;
    bool mIsDirectory;
    QString const mkEmptyProperty = "";

private:
    enum PropertyDirectory
    {
        kName,
        kNumberChildren
    };
};

}

}

#endif // ABSTRACTPROPERTIESMODEL_H
