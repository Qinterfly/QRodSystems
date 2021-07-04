/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the GeometryComponentWidget class
 */

#ifndef GEOMETRYRODCOMPONENTWIDGET_H
#define GEOMETRYRODCOMPONENTWIDGET_H

#include <QWidget>

namespace QRS
{

namespace Core
{
class GeometryRodComponent;
class AbstractDataObject;
}

namespace Managers
{

//! Widget to construct a geometrical rod component
class GeometryRodComponentWidget : public QWidget
{
    Q_OBJECT

public:
    GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent, QString const& mimeType, QWidget* parent = nullptr);

signals:
    void modified();

private slots:
    void setRadiusVector(Core::AbstractDataObject const* pDataObject);
    void setRotationMatrix(Core::AbstractDataObject const* pDataObject);

private:
    void createContent();

private:
    QString const mkMimeType;
    Core::GeometryRodComponent& mGeometryRodComponent;
};

}

}

#endif // GEOMETRYRODCOMPONENTWIDGET_H
