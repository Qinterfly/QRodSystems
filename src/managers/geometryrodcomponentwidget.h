/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the GeometryComponentWidget class
 */

#ifndef GEOMETRYRODCOMPONENTWIDGET_H
#define GEOMETRYRODCOMPONENTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
class GeometryRodComponent;
class AbstractDataObject;
}

namespace Managers
{

//! Widget to construct a geometrical component of a rod
class GeometryRodComponentWidget : public QWidget
{
    Q_OBJECT

public:
    GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent, QWidget* parent = nullptr);
    ~GeometryRodComponentWidget() = default;

signals:
    void modified(bool flag = true);

private slots:
    void setRadiusVector(Core::AbstractDataObject const* pDataObject);
    void setRotationMatrix(Core::AbstractDataObject const* pDataObject);

private:
    void createContent();

private:
    Core::GeometryRodComponent& mGeometryRodComponent;
};

}

}

#endif // GEOMETRYRODCOMPONENTWIDGET_H
