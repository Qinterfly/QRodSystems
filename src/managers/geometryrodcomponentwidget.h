/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the GeometryComponentWidget class
 */

#ifndef GEOMETRYRODCOMPONENTWIDGET_H
#define GEOMETRYRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"

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
class GeometryRodComponentWidget : public AbstractRodComponentWidget
{
public:
    GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent, QString const& mimeType, QWidget* parent = nullptr);

private:
    void createContent();
    template<typename T>
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);

private:
    Core::GeometryRodComponent& mGeometryRodComponent;
};

}

}

#endif // GEOMETRYRODCOMPONENTWIDGET_H
