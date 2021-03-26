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
    ScalarTableModel(QWidget* parent = nullptr);
    ~ScalarTableModel() = default;
    void setScalarDataObject(QRS::ScalarDataObject* pScalarDataObject);

private:
    void updateContent();

private:
    QRS::ScalarDataObject* mpScalarDataObject = nullptr;
};


#endif // SCALARTABLEMODEL_H
