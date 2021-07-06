/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the MaterialRodComponentWidget class
 */

#ifndef MATERIALRODCOMPONENTWIDGET_H
#define MATERIALRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"

namespace QRS
{

namespace Core
{
class MaterialRodComponent;
class AbstractDataObject;
}

namespace Managers
{

//! Widget to construct a material rod component
class MaterialRodComponentWidget : public AbstractRodComponentWidget
{
public:
    MaterialRodComponentWidget(Core::MaterialRodComponent& materialRodComponent, QString const& mimeType, QWidget* parent = nullptr);

private:
    void createContent();
    QWidget* createModuliWidget();
    QLayout* createBaseLayout();
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);

private:
    Core::MaterialRodComponent& mMaterialRodComponent;
};

}

}

#endif // MATERIALRODCOMPONENTWIDGET_H
