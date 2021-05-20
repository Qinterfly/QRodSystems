/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the TableModelInterface
 */

#ifndef TABLEMODELINTERFACE_H
#define TABLEMODELINTERFACE_H

#include <QItemSelection>

QT_BEGIN_NAMESPACE
class QStandardItem;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
template <typename T>
class Array;
}

namespace TableModels
{

static const short kNumShowPrecision = 6;

//! User interface to add and remove items
class TableModelInterface
{
public:
    virtual void insertItemAfterSelected(QItemSelectionModel* pSelectionModel) = 0;
    virtual void insertLeadingItemAfterSelected(QItemSelectionModel* pSelectionModel) = 0;
    virtual void removeSelectedItem(QItemSelectionModel* pSelectionModel) = 0;
    virtual void removeSelectedLeadingItem(QItemSelectionModel* pSelectionModel) = 0;
    virtual ~TableModelInterface() { };
    static QStandardItem* makeDoubleItem(double value);
    static QList<QStandardItem*> prepareRow(Core::Array<double>& array, uint iRow);
    static QList<QStandardItem*> prepareRow(double const& key, Core::Array<double>& array, uint iRow);
    static QList<QStandardItem*> prepareRow(QString const& name, Core::Array<double>& array, uint iRow);
    static QStandardItem* makeLabelItem(QString const& name);
};

}

}

#endif // TABLEMODELINTERFACE_H
