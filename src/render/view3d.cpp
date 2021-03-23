/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the View3D class
 */

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include "view3d.h"

View3D::View3D(QWidget* parent)
    : QOpenGLWidget(parent)
{
    mCore = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
}

//! Initialize a graphical scene
void View3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
}

//! Render its content
void View3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
