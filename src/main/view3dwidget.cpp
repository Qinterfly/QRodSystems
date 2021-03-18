/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of View3DWidget class
 */

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include "view3dwidget.h"

View3DWidget::View3DWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    mCore = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
}

//! Initialize a graphical scene
void View3DWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
}

//! Render its content
void View3DWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
