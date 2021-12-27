/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the MechanicalRodComponentWidget class
 */

#ifndef MECHANICALRODCOMPONENTWIDGET_H
#define MECHANICALRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"

namespace QRS
{

namespace Core
{
class AbstractDataObject;
class MechanicalRodComponent;
}

namespace Managers
{

//! Widget to construct mechanical rod components consisted of stiffness and mass distributions
class MechanicalRodComponentWidget : public AbstractRodComponentWidget
{
public:
    MechanicalRodComponentWidget(Core::MechanicalRodComponent& mechanicalRodComponent, QString const& mimeType, QWidget* parent = nullptr);

private:
    void createContent();
    QWidget* createStiffnessGroup();
    QWidget* createMassGroup();
    QWidget* createEccentricityGroup();
    QLayout* createContactDiameterLayout();
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);

private:
    Core::MechanicalRodComponent& mMechanicalRodComponent;
};

}

}

#endif // MECHANICALRODCOMPONENTWIDGET_H
