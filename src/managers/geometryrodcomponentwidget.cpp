/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definiton of the GeometryComponentWidget class
 */

#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include "geometryrodcomponentwidget.h"
#include "dataobjectlineedit.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/geometryrodcomponent.h"

using namespace QRS::Managers;
using namespace QRS::Core;

GeometryRodComponentWidget::GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent,
                                                       QString const& mimeType, QWidget* parent)
    : QWidget(parent)
    , mkMimeType(mimeType)
    , mGeometryRodComponent(geometryRodComponent)
{
    setPalette(parent->palette());
    setWindowState(Qt::WindowMaximized);
    createContent();
}

//! Create all the widgets
void GeometryRodComponentWidget::createContent()
{
    QGridLayout* pMainLayout = new QGridLayout(this);
    // Radius vector
    DataObjectLineEdit* pRadiusVectorEdit = new DataObjectLineEdit(mGeometryRodComponent.radiusVector(),
                                                                   AbstractDataObject::ObjectType::kVector, mkMimeType);
    pMainLayout->addWidget(new QLabel(tr("Radius vector: ")), 0, 0);
    pMainLayout->addWidget(pRadiusVectorEdit, 0, 1);
    connect(pRadiusVectorEdit, &DataObjectLineEdit::dataObjectSelected, this, &GeometryRodComponentWidget::setRadiusVector);
    // Rotation matrix
    DataObjectLineEdit* pRotationMatrixEdit = new DataObjectLineEdit(mGeometryRodComponent.rotationMatrix(),
                                                                     AbstractDataObject::ObjectType::kMatrix, mkMimeType);
    pMainLayout->addWidget(new QLabel(tr("Rotation matrix: ")), 1, 0);
    pMainLayout->addWidget(pRotationMatrixEdit, 1, 1);
    connect(pRotationMatrixEdit, &DataObjectLineEdit::dataObjectSelected, this, &GeometryRodComponentWidget::setRotationMatrix);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 0);
    setLayout(pMainLayout);
}

//! Set a radius vector
void GeometryRodComponentWidget::setRadiusVector(Core::AbstractDataObject const* pDataObject)
{
    mGeometryRodComponent.setRadiusVector((VectorDataObject*)pDataObject);
    emit modified();
}

//! Set a rotation matrix
void GeometryRodComponentWidget::setRotationMatrix(Core::AbstractDataObject const* pDataObject)
{
    mGeometryRodComponent.setRotationMatrix((MatrixDataObject*)pDataObject);
    emit modified();
}
