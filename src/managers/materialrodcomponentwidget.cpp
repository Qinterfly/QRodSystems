/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the MaterialRodComponentWidget class
 */

#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QGroupBox>
#include "materialrodcomponentwidget.h"
#include "dataobjectlineedit.h"
#include "core/materialrodcomponent.h"
#include "core/scalardataobject.h"

using namespace QRS::Core;
using namespace QRS::Managers;

MaterialRodComponentWidget::MaterialRodComponentWidget(Core::MaterialRodComponent& materialRodComponent,
                                                       QString const& mimeType, QWidget* parent)
    : AbstractRodComponentWidget(mimeType, parent)
    , mMaterialRodComponent(materialRodComponent)
{
    createContent();
}

//! Create all the widgets
void MaterialRodComponentWidget::createContent()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    // Elastic and shear moduli
    pMainLayout->addWidget(createModuliGroup());
    // Density and Poisson's ratio
    pMainLayout->addLayout(createBaseLayout());
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(pMainLayout);
}

//! Create a group consisted of widgets to set physical moduli
QWidget* MaterialRodComponentWidget::createModuliGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Moduli"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    std::function<void(AbstractDataObject const*)> setFun;
    // Elastic modulus
    pEdit = new DataObjectLineEdit(mMaterialRodComponent.elasticModulus(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MaterialRodComponent::setElasticModulus); };
    pLayout->addWidget(new QLabel(tr("Elastic: ")), 0, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &MaterialRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(tr("(Pa)")), 0, 2);
    // Shear modulus
    pEdit = new DataObjectLineEdit(mMaterialRodComponent.shearModulus(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MaterialRodComponent::setShearModulus); };
    pLayout->addWidget(new QLabel(tr("Shear: ")), 1, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &MaterialRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 1, 1);
    pLayout->addWidget(new QLabel(tr("(Pa)")), 1, 2);
    // Layout
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a layout consisted of widgets to set density and Poisson's ratio
QLayout* MaterialRodComponentWidget::createBaseLayout()
{
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    std::function<void(AbstractDataObject const*)> setFun;
    // Density
    pEdit = new DataObjectLineEdit(mMaterialRodComponent.density(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MaterialRodComponent::setDensity); };
    pLayout->addWidget(new QLabel(tr("Density: ")), 0, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &MaterialRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(tr("(kg/m<sup>3</sup>)")), 0, 2);
    // Poisson's ratio
    pEdit = new DataObjectLineEdit(mMaterialRodComponent.poissonsRatio(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &MaterialRodComponent::setPoissonsRatio); };
    pLayout->addWidget(new QLabel(tr("Poisson's ratio: ")), 1, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &MaterialRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 1, 1);
    return pLayout;
}

//! Set a material property which takes a scalar data object
void MaterialRodComponentWidget::setProperty(AbstractDataObject const* pDataObject, auto setFun)
{
    (mMaterialRodComponent.*setFun)((ScalarDataObject const*)pDataObject);
    emit modified();
}
