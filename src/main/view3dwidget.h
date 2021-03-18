/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of View3DWidget class
 */

#ifndef VIEW3DWIDGET_H
#define VIEW3DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

//! A widget to represent the resulted rod system
class View3DWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    View3DWidget(QWidget* parent = nullptr);
    ~View3DWidget() = default;

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    bool mCore;
};

#endif // VIEW3DWIDGET_H
