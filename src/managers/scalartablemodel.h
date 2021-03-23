/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the ScalarTableModel class
 */

#ifndef SCALARTABLEMODEL_H
#define SCALARTABLEMODEL_H

#include <QStandardItemModel>

namespace QRS
{
class ScalarDataObject;
}

//! Table model to represent a scalar data object
class ScalarTableModel : public QStandardItemModel
{
    Q_OBJECT

public:
    ScalarTableModel(QRS::ScalarDataObject* pScalarDataObject, QWidget* parent = nullptr);
    ~ScalarTableModel() = default;

private:
    void updateContent();

private:
    QRS::ScalarDataObject* mpScalarDataObject;
};

#endif // SCALARTABLEMODEL_H
