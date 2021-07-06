/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definiton of the GeometryComponentWidget class
 */

#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include "geometryrodcomponentwidget.h"
#include "dataobjectlineedit.h"
#include "core/geometryrodcomponent.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"

using namespace QRS::Managers;
using namespace QRS::Core;

GeometryRodComponentWidget::GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent,
                                                       QString const& mimeType, QWidget* parent)
    : AbstractRodComponentWidget(mimeType, parent)
    , mGeometryRodComponent(geometryRodComponent)
{
    createContent();
}

//! Create all the widgets
void GeometryRodComponentWidget::createContent()
{
    QGridLayout* pMainLayout = new QGridLayout(this);
    DataObjectLineEdit* pEdit;
    // Radius vector
    pEdit = new DataObjectLineEdit(mGeometryRodComponent.radiusVector(), AbstractDataObject::ObjectType::kVector, mkMimeType);
    pMainLayout->addWidget(new QLabel(tr("Radius vector: ")), 0, 0);
    connect(pEdit, &DataObjectLineEdit::selected, this, &GeometryRodComponentWidget::setRadiusVector);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &GeometryRodComponentWidget::editDataObjectRequested);
    pMainLayout->addWidget(pEdit, 0, 1);
    pMainLayout->addWidget(new QLabel(tr("(m)")), 0, 2);
    // Rotation matrix
    pEdit = new DataObjectLineEdit(mGeometryRodComponent.rotationMatrix(), AbstractDataObject::ObjectType::kMatrix, mkMimeType);
    pMainLayout->addWidget(new QLabel(tr("Rotation matrix: ")), 1, 0);
    connect(pEdit, &DataObjectLineEdit::selected, this, &GeometryRodComponentWidget::setRotationMatrix);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &GeometryRodComponentWidget::editDataObjectRequested);
    pMainLayout->addWidget(pEdit, 1, 1);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 0);
    setLayout(pMainLayout);
}

//! Set a radius vector
void GeometryRodComponentWidget::setRadiusVector(Core::AbstractDataObject const* pDataObject)
{
    mGeometryRodComponent.setRadiusVector((VectorDataObject const*)pDataObject);
    emit modified();
}

//! Set a rotation matrix
void GeometryRodComponentWidget::setRotationMatrix(Core::AbstractDataObject const* pDataObject)
{
    mGeometryRodComponent.setRotationMatrix((MatrixDataObject const*)pDataObject);
    emit modified();
}
