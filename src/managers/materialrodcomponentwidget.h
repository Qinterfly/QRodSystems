/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the MaterialRodComponentWidget class
 */

#ifndef MATERIALRODCOMPONENTWIDGET_H
#define MATERIALRODCOMPONENTWIDGET_H

#include <QWidget>
#include "core/aliasdata.h"

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
class MaterialRodComponentWidget : public QWidget
{
    Q_OBJECT

signals:
    void modified();
    void editDataObjectRequested(Core::DataIDType id);

public:
    MaterialRodComponentWidget(Core::MaterialRodComponent& materialRodComponent, QString const& mimeType, QWidget* parent = nullptr);

private:
    void createContent();
    QWidget* createModuliWidget();
    QLayout* createBaseLayout();
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);

private:
    QString const mkMimeType;
    Core::MaterialRodComponent& mMaterialRodComponent;
};

}

}

#endif // MATERIALRODCOMPONENTWIDGET_H
