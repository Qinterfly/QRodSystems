/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the UserSectionRodComponentWidget class
 */

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include "usersectionrodcomponentwidget.h"
#include "core/usersectionrodcomponent.h"
#include "dataobjectlineedit.h"

using namespace QRS::Managers;
using namespace QRS::Core;

UserSectionRodComponentWidget::UserSectionRodComponentWidget(UserSectionRodComponent& userSectionRodComponent,
                                                             QString const& mimeType, QWidget* parent)
    : AbstractRodComponentWidget(mimeType, parent)
    , mUserSectionRodComponent(userSectionRodComponent)
{
    createContent();
}

//! Create all the content
void UserSectionRodComponentWidget::createContent()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    // Area
    pMainLayout->addLayout(createAreaLayout());
    // Moments of inertia
    pMainLayout->addWidget(createInertiaMomentsGroup());
    // Coordinates of the center
    pMainLayout->addWidget(createCenterCoordinatesGroup());
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(pMainLayout);
}

//! Create an area layout
QLayout* UserSectionRodComponentWidget::createAreaLayout()
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    DataObjectLineEdit* pEdit = new DataObjectLineEdit(mUserSectionRodComponent.area(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    DataObjectSetFun setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &UserSectionRodComponent::setArea); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(new QLabel(tr("Area: ")));
    pLayout->addWidget(pEdit);
    pLayout->addWidget(new QLabel(tr("(m<sup>2</sup>)")));
    return pLayout;
}

//! Create a group consisted of widgets to set moments of inertia
QWidget* UserSectionRodComponentWidget::createInertiaMomentsGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Moments of inertia"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    DataObjectSetFun setFun;
    QString const kDimension = tr("(m<sup>4</sup>)");
    // Torsional
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaMomentTorsional(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &UserSectionRodComponent::setInertiaMomentTorsional); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(new QLabel(tr("Torsional: ")), 0, 0);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(kDimension), 0, 2);
    // X
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaMomentX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &UserSectionRodComponent::setInertiaMomentX); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(new QLabel(tr("X: ")), 1, 0);
    pLayout->addWidget(pEdit, 1, 1);
    pLayout->addWidget(new QLabel(kDimension), 1, 2);
    // Y
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaMomentY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &UserSectionRodComponent::setInertiaMomentY); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(new QLabel(tr("Y: ")), 2, 0);
    pLayout->addWidget(pEdit, 2, 1);
    pLayout->addWidget(new QLabel(kDimension), 2, 2);
    // Layout
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a group consisted of widgets to set coordinates of the center
QWidget* UserSectionRodComponentWidget::createCenterCoordinatesGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Coordinates of the center"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    DataObjectSetFun setFun;
    QString const kDimension = tr("(m)");
    // X
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.centerCoordinateX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &UserSectionRodComponent::setCenterCoordinateX); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(new QLabel(tr("X: ")), 0, 0);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(new QLabel(kDimension), 0, 2);
    // Y
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.centerCoordinateY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty(pData, &UserSectionRodComponent::setCenterCoordinateY); };
    setDataObjectEditConnections(pEdit, setFun);
    pLayout->addWidget(new QLabel(tr("Y: ")), 1, 0);
    pLayout->addWidget(pEdit, 1, 1);
    pLayout->addWidget(new QLabel(kDimension), 1, 2);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Set a section property which takes a scalar data object
void UserSectionRodComponentWidget::setProperty(AbstractDataObject const* pDataObject, auto setFun)
{
    (mUserSectionRodComponent.*setFun)((ScalarDataObject const*)pDataObject);
    emit modified();
}
