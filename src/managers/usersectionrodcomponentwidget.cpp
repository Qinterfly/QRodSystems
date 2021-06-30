/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
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
    : QWidget(parent)
    , mkMimeType(mimeType)
    , mUserSectionRodComponent(userSectionRodComponent)
{
    setPalette(parent->palette());
    setWindowState(Qt::WindowMaximized);
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
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setArea);
    pLayout->addWidget(new QLabel(tr("Area: ")));
    pLayout->addWidget(pEdit);
    return pLayout;
}

//! Create a group consisted of widgets to set moments of inertia
QWidget* UserSectionRodComponentWidget::createInertiaMomentsGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Moments of inertia: "));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    // Torsional
    pLayout->addWidget(new QLabel(tr("Torsional: ")), 0, 0);
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaMomentTorsional(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setInertiaMomentTorsional);
    pLayout->addWidget(pEdit, 0, 1);
    // X
    pLayout->addWidget(new QLabel(tr("X: ")), 1, 0);
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaMomentX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setInertiaMomentX);
    pLayout->addWidget(pEdit, 1, 1);
    // Y
    pLayout->addWidget(new QLabel(tr("Y: ")), 2, 0);
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaMomentY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setInertiaMomentY);
    pLayout->addWidget(pEdit, 2, 1);
    // XY
    pLayout->addWidget(new QLabel(tr("Product XY: ")), 3, 0);
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.inertiaProductXY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setInertiaProductXY);
    pLayout->addWidget(pEdit, 3, 1);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a group consisted of widgets to set coordinates of the center
QWidget* UserSectionRodComponentWidget::createCenterCoordinatesGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Coordinates of the center: "));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    // X
    pLayout->addWidget(new QLabel(tr("X: ")), 0, 0);
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.centerCoordinateX(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setCenterCoordinateX);
    pLayout->addWidget(pEdit, 0, 1);
    // Y
    pLayout->addWidget(new QLabel(tr("Y: ")), 1, 0);
    pEdit = new DataObjectLineEdit(mUserSectionRodComponent.centerCoordinateY(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    connect(pEdit, &DataObjectLineEdit::dataObjectSelected, this, &UserSectionRodComponentWidget::setCenterCoordinateY);
    pLayout->addWidget(pEdit, 1, 1);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Set an area
void UserSectionRodComponentWidget::setArea(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setArea((ScalarDataObject const*)pDataObject);
    emit modified();
}

//! Set a torsional moment of inertia
void UserSectionRodComponentWidget::setInertiaMomentTorsional(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setInertiaMomentTorsional((ScalarDataObject const*)pDataObject);
    emit modified();
}

//! Set a moment of inertia about the X axis
void UserSectionRodComponentWidget::setInertiaMomentX(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setInertiaMomentX((ScalarDataObject const*)pDataObject);
    emit modified();
}

//! Set a moment of inertia about the Y axis
void UserSectionRodComponentWidget::setInertiaMomentY(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setInertiaMomentY((ScalarDataObject const*)pDataObject);
    emit modified();
}

//! Set a product of inertia
void UserSectionRodComponentWidget::setInertiaProductXY(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setInertiaProductXY((ScalarDataObject const*)pDataObject);
    emit modified();
}

//! Set coordinate of the center along the X axis
void UserSectionRodComponentWidget::setCenterCoordinateX(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setCenterCoordinateX((ScalarDataObject const*)pDataObject);
    emit modified();
}

//! Set coordinate of the center along the Y axis
void UserSectionRodComponentWidget::setCenterCoordinateY(Core::AbstractDataObject const* pDataObject)
{
    mUserSectionRodComponent.setCenterCoordinateY((ScalarDataObject const*)pDataObject);
    emit modified();
}
