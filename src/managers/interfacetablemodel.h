/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Interface of a table model
 */

#ifndef INTERFACETABLEMODEL_H
#define INTERFACETABLEMODEL_H

#include<QItemSelection>

//! User interface to add and remove items
class InterfaceTableModel
{
public:
    virtual void insertItemAfterSelected(QItemSelectionModel* selectionModel) = 0;
    virtual void removeSelectedItem(QItemSelectionModel* selectionModel) = 0;
    virtual ~InterfaceTableModel() { };
};

#endif // INTERFACETABLEMODEL_H
