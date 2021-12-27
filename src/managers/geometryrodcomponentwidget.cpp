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
    DataObjectSetFun setFun;
    // Radius vector
    pEdit = new DataObjectLineEdit(mGeometryRodComponent.radiusVector(), AbstractDataObject::ObjectType::kVector, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty<VectorDataObject>(pData, &GeometryRodComponent::setRadiusVector); };
    setDataObjectEditConnections(pEdit, setFun);
    pMainLayout->addWidget(new QLabel(tr("Radius vector: ")), 0, 0);
    pMainLayout->addWidget(pEdit, 0, 1);
    pMainLayout->addWidget(new QLabel(tr("(m)")), 0, 2);
    // Rotation matrix
    pEdit = new DataObjectLineEdit(mGeometryRodComponent.rotationMatrix(), AbstractDataObject::ObjectType::kMatrix, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty<MatrixDataObject>(pData, &GeometryRodComponent::setRotationMatrix); };
    setDataObjectEditConnections(pEdit, setFun);
    pMainLayout->addWidget(new QLabel(tr("Rotation matrix: ")), 1, 0);
    pMainLayout->addWidget(pEdit, 1, 1);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 0);
    setLayout(pMainLayout);
}

//! Set a property of a rod geometry
template<typename T>
void GeometryRodComponentWidget::setProperty(AbstractDataObject const* pDataObject, auto setFun)
{
    (mGeometryRodComponent.*setFun)((T const*)pDataObject);
    emit modified();
}
