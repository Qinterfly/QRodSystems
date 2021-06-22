/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definiton of the GeometryComponentWidget class
 */

#include <QGridLayout>
#include <QComboBox>
#include <QSpacerItem>
#include <QLabel>
#include "geometryrodcomponentwidget.h"
#include "core/vectordataobject.h"
#include "core/matrixdataobject.h"
#include "core/geometryrodcomponent.h"

using namespace QRS::Managers;
using namespace QRS::Core;

template <typename T>
QComboBox* createObjectsComboBox(auto const& objects, T const* pCurrentObject);

GeometryRodComponentWidget::GeometryRodComponentWidget(Core::GeometryRodComponent& geometryRodComponent,
                                                       VectorDataObjects const& vectorDataObjects,
                                                       MatrixDataObjects const& matrixDataObjects,
                                                       QWidget* parent)
    : QWidget(parent)
    , mGeometryRodComponent(geometryRodComponent)
    , mVectorDataObjects(vectorDataObjects)
    , mMatrixDataObjects(matrixDataObjects)
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
    mpComboBoxRadiusVector = createObjectsComboBox(mVectorDataObjects, mGeometryRodComponent.radiusVector());
    connect(mpComboBoxRadiusVector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GeometryRodComponentWidget::setRadiusVector);
    pMainLayout->addWidget(new QLabel(tr("Radius vector: ")), 0, 0);
    pMainLayout->addWidget(mpComboBoxRadiusVector, 0, 1);
    // Rotation matrix
    mpComboBoxRotationMatrix = createObjectsComboBox(mMatrixDataObjects, mGeometryRodComponent.rotationMatrix());
    connect(mpComboBoxRotationMatrix, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GeometryRodComponentWidget::setRotationMatrix);
    pMainLayout->addWidget(new QLabel(tr("Rotation matrix: ")), 1, 0);
    pMainLayout->addWidget(mpComboBoxRotationMatrix, 1, 1);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 0);
    setLayout(pMainLayout);
}

//! Set a radius vector by selected index
void GeometryRodComponentWidget::setRadiusVector(int index)
{
    QVariant data = mpComboBoxRadiusVector->itemData(index, Qt::UserRole);
    if (!data.isNull())
        mGeometryRodComponent.setRadiusVector(mVectorDataObjects.at(data.value<DataIDType>()));
    else
        mGeometryRodComponent.setRadiusVector(nullptr);
}

//! Set a rotation matrix by selected index
void GeometryRodComponentWidget::setRotationMatrix(int index)
{
    QVariant data = mpComboBoxRotationMatrix->itemData(index, Qt::UserRole);
    if (!data.isNull())
        mGeometryRodComponent.setRotationMatrix(mMatrixDataObjects.at(data.value<DataIDType>()));
    else
        mGeometryRodComponent.setRotationMatrix(nullptr);
}

//! Helper function to create a comobobox which consists of objects names from a given set
template <typename T>
QComboBox* createObjectsComboBox(auto const& objects, T const* pCurrentObject)
{
    QComboBox* pComboBox = new QComboBox();
    pComboBox->addItem(QString());
    pComboBox->setCurrentIndex(0);
    for (auto const& iter : objects)
    {
        T const* pObject = iter.second;
        pComboBox->addItem(pObject->name(), iter.first);
        if (pCurrentObject == pObject)
            pComboBox->setCurrentIndex(pComboBox->count() - 1);
    }
    return pComboBox;
}
