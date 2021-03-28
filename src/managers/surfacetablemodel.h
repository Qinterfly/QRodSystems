/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the SurfaceTableModel class
 */

#ifndef SURFACETABLEMODEL_H
#define SURFACETABLEMODEL_H

#include <QStandardItemModel>
#include "interfacetablemodel.h"

namespace QRS
{
class SurfaceDataObject;
}

//! Table model to represent a surface data object
class SurfaceTableModel : public QStandardItemModel, public InterfaceTableModel
{
    Q_OBJECT

public:
    SurfaceTableModel(QWidget* parent = nullptr);
    ~SurfaceTableModel() = default;
    void setDataObject(QRS::SurfaceDataObject* pDataObject);
    bool setData(const QModelIndex& indexEdit, const QVariant& value, int role = Qt::EditRole) override;
    void insertItemAfterSelected(QItemSelectionModel* selectionModel) override;
    void removeSelectedItem(QItemSelectionModel* selectionModel) override;
    void insertLeadingItemAfterSelected(QItemSelectionModel* selectionModel) override;
    void removeSelectedLeadingItem(QItemSelectionModel* selectionModel) override;

private:
    void updateContent();
    void clearContent();

private:
    QRS::SurfaceDataObject* mpDataObject = nullptr;
};

#endif // SURFACETABLEMODEL_H
