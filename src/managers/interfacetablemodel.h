/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Interface of a table model
 */

#ifndef INTERFACETABLEMODEL_H
#define INTERFACETABLEMODEL_H

#include<QItemSelection>

QT_BEGIN_NAMESPACE
class QStandardItem;
QT_END_NAMESPACE

namespace QRS
{
template <typename T>
class Array;
}

static const short kNumShowPrecision = 4;

//! User interface to add and remove items
class InterfaceTableModel
{
public:
    virtual void insertItemAfterSelected(QItemSelectionModel* selectionModel) = 0;
    virtual void insertLeadingItemAfterSelected(QItemSelectionModel* selectionModel) = 0;
    virtual void removeSelectedItem(QItemSelectionModel* selectionModel) = 0;
    virtual void removeSelectedLeadingItem(QItemSelectionModel* selectionModel) = 0;
    virtual ~InterfaceTableModel() { };
    static QStandardItem* makeDoubleItem(double value);
    static QList<QStandardItem*> prepareRow(QRS::Array<double>& array, uint iRow);
    static QList<QStandardItem*> prepareRow(double const& key, QRS::Array<double>& array, uint iRow);
    static QList<QStandardItem*> prepareRow(QString const& name, QRS::Array<double>& array, uint iRow);
    static QStandardItem* makeLabelItem(QString const& name);
};

#endif // INTERFACETABLEMODEL_H
