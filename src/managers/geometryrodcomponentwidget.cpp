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

using namespace QRS::Managers;

GeometryRodComponentWidget::GeometryRodComponentWidget(Core::GeometryRodComponent& geometry, QWidget* parent)
    : QWidget(parent)
    , mGeometry(geometry)
{
    setWindowState(Qt::WindowMaximized);
    createContent();
}

GeometryRodComponentWidget::~GeometryRodComponentWidget()
{

}

//! Construct all the widgets
void GeometryRodComponentWidget::createContent()
{
    QGridLayout* pMainLayout = new QGridLayout(this);
    // Radius vector
    pMainLayout->addWidget(new QLabel(tr("Radius vector: ")), 0, 0);
    pMainLayout->addWidget(new QComboBox(), 0, 1);
    // Rotation matrix
    pMainLayout->addWidget(new QLabel(tr("Rotation matrix: ")), 1, 0);
    pMainLayout->addWidget(new QComboBox(), 1, 1);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 0);
    setLayout(pMainLayout);
}
