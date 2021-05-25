/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the SurfaceTableModel class
 */

#ifndef SURFACETABLEMODEL_H
#define SURFACETABLEMODEL_H

#include <QStandardItemModel>
#include "tablemodelinterface.h"

namespace QRS
{

namespace Core
{
class SurfaceDataObject;
}

namespace TableModels
{

//! Table model to represent a surface data object
class SurfaceTableModel : public QStandardItemModel, public TableModelInterface
{
    Q_OBJECT

public:
    SurfaceTableModel(QWidget* parent = nullptr);
    ~SurfaceTableModel() = default;
    void setDataObject(Core::SurfaceDataObject* pDataObject);
    bool setData(const QModelIndex& indexEdit, const QVariant& value, int role = Qt::EditRole) override;
    void insertItemAfterSelected(QItemSelectionModel* pSelectionModel) override;
    void removeSelectedItem(QItemSelectionModel* pSelectionModel) override;
    void insertLeadingItemAfterSelected(QItemSelectionModel* pSelectionModel) override;
    void removeSelectedLeadingItem(QItemSelectionModel* pSelectionModel) override;

private:
    void updateContent();
    void clearContent();

private:
    Core::SurfaceDataObject* mpDataObject = nullptr;
};

}

}

#endif // SURFACETABLEMODEL_H
