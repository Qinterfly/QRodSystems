/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the GeometryComponentWidget class
 */

#ifndef GEOMETRYRODCOMPONENTWIDGET_H
#define GEOMETRYRODCOMPONENTWIDGET_H

#include <QWidget>
#include "aliasmanagers.h"

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
class GeometryRodComponent;
}

namespace Managers
{

//! Widget to construct a geometrical component of a rod
class GeometryRodComponentWidget : public QWidget
{
    Q_OBJECT

public:
    GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent, VectorDataObjects const& vectorDataObjects,
                               MatrixDataObjects const& matrixDataObjects, QWidget* parent = nullptr);
    ~GeometryRodComponentWidget() = default;

private slots:
    void setRadiusVector(int index);
    void setRotationMatrix(int index);

private:
    void createContent();

private:
    // Data
    Core::GeometryRodComponent& mGeometryRodComponent;
    VectorDataObjects const& mVectorDataObjects;
    MatrixDataObjects const& mMatrixDataObjects;
    // Widgets
    QComboBox* mpComboBoxRadiusVector;
    QComboBox* mpComboBoxRotationMatrix;
};

}

}

#endif // GEOMETRYRODCOMPONENTWIDGET_H
