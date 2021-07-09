/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the MechanicalRodComponentWidget class
 */

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "mechanicalrodcomponentwidget.h"
#include "dataobjectlineedit.h"
#include "core/mechanicalrodcomponent.h"
#include "core/scalardataobject.h"

using namespace QRS::Core;
using namespace QRS::Managers;

MechanicalRodComponentWidget::MechanicalRodComponentWidget(MechanicalRodComponent& mechanicalRodComponent,
                                                           QString const& mimeType, QWidget* parent)
    : AbstractRodComponentWidget(mimeType, parent)
    , mMechanicalRodComponent(mechanicalRodComponent)
{
    createContent();
}

//! Create all the widgets
void MechanicalRodComponentWidget::createContent()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    // Stiffness
    pMainLayout->addWidget(createStiffnessGroup());
    // Mass
    pMainLayout->addWidget(createMassGroup());
    // Eccentricity
    pMainLayout->addWidget(createEccentricityGroup());
    // Contact diameter
    pMainLayout->addLayout(createContactDiameterLayout());
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(pMainLayout);
}

//! Create a group consisted of widgets to set stiffness distributions
QWidget* MechanicalRodComponentWidget::createStiffnessGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Stiffness"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    DataObjectSetFun setFun;
    // Tension
    pLayout->addWidget(new QLabel(tr("Tension: ")), 0, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.tensionStiffness(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setTensionStiffness); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(tr("(N)")), 0, 2);
    // Torsional
    pLayout->addWidget(new QLabel(tr("Torsional: ")), 1, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.torsionalStiffness(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setTorsionalStiffness); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 1, 1);
    pLayout->addWidget(new QLabel(tr("(N*m<sup>2</sup>)")), 1, 2);
    // Bending stiffness around X
    pLayout->addWidget(new QLabel(tr("Bending X: ")), 2, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.bendingStiffnessX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setBendingStiffnessX); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 2, 1);
    pLayout->addWidget(new QLabel(tr("(N*m<sup>2</sup>)")), 2, 2);
    // Bending stiffness around Y
    pLayout->addWidget(new QLabel(tr("Bending Y: ")), 3, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.bendingStiffnessY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setBendingStiffnessY); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 3, 1);
    pLayout->addWidget(new QLabel(tr("(N*m<sup>2</sup>)")), 3, 2);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a group consisted of widgets to set mass distributions
QWidget* MechanicalRodComponentWidget::createMassGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Mass"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    DataObjectSetFun setFun;
    // Linear mass density
    pLayout->addWidget(new QLabel(tr("Linear density: ")), 0, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.linearMassDensity(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setLinearMassDensity); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(tr("(kg/m)")), 0, 2);
    // Inertia moment X
    pLayout->addWidget(new QLabel(tr("Inertia moment X: ")), 1, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.inertiaMassMomentX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setInertiaMassMomentX); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 1, 1);
    pLayout->addWidget(new QLabel(tr("(kg*m<sup>4</sup>)")), 1, 2);
    // Inertia moment Y
    pLayout->addWidget(new QLabel(tr("Inertia moment Y: ")), 2, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.inertiaMassMomentY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setInertiaMassMomentY); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 2, 1);
    pLayout->addWidget(new QLabel(tr("(kg*m<sup>4</sup>)")), 2, 2);
    // Inertia moment Z
    pLayout->addWidget(new QLabel(tr("Inertia moment Z: ")), 3, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.inertiaMassMomentZ(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setInertiaMassMomentZ); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 3, 1);
    pLayout->addWidget(new QLabel(tr("(kg*m<sup>4</sup>)")), 3, 2);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a group consisted of widgets to set eccentricity distributions
QWidget* MechanicalRodComponentWidget::createEccentricityGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Eccentricity"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    DataObjectSetFun setFun;
    // X
    pLayout->addWidget(new QLabel(tr("X: ")), 0, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.eccentricityX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setEccentricityX); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(tr("(m)")), 0, 2);
    // Y
    pLayout->addWidget(new QLabel(tr("Y: ")), 1, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.eccentricityY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setEccentricityY); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 1, 1);
    pLayout->addWidget(new QLabel(tr("(m)")), 1, 2);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a layout to set a contact diameter
QLayout* MechanicalRodComponentWidget::createContactDiameterLayout()
{
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    DataObjectSetFun setFun;
    pLayout->addWidget(new QLabel(tr("Contact diameter: ")), 0, 0);
    pEdit = new DataObjectLineEdit(mMechanicalRodComponent.contactDiameter(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MechanicalRodComponent::setContactDiameter); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(tr("(m)")), 0, 2);
    return pLayout;
}

//! Set a mechanical property which takes a scalar data object
void MechanicalRodComponentWidget::setProperty(AbstractDataObject const* pDataObject, auto setFun)
{
    (mMechanicalRodComponent.*setFun)((ScalarDataObject const*)pDataObject);
    emit modified();
}
