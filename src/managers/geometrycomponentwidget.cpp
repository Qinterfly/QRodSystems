/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Definiton of the GeometryComponentWidget class
 */

#include "geometrycomponentwidget.h"

using namespace QRS::Managers;

GeometryComponentWidget::GeometryComponentWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowState(Qt::WindowMaximized);
    createContent();
}

GeometryComponentWidget::~GeometryComponentWidget()
{

}

//! Construct all the widgets
void GeometryComponentWidget::createContent()
{
    // TODO
}
