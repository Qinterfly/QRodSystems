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
}

namespace Managers
{

//! Widget to construct a geometrical component of a rod
class GeometryRodComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeometryRodComponentWidget(Core::GeometryRodComponent& geometry, QWidget* parent = nullptr);
    ~GeometryRodComponentWidget();

private:
    void createContent();

private:
    Core::GeometryRodComponent& mGeometry;
};

}

}

#endif // GEOMETRYRODCOMPONENTWIDGET_H
