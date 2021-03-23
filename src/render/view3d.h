/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the View3D class
 */

#ifndef VIEW3D_H
#define VIEW3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

//! A widget to represent the resulted rod system
class View3D : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    View3D(QWidget* parent = nullptr);
    ~View3D() = default;

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    bool mCore;
};

#endif // VIEW3D_H
