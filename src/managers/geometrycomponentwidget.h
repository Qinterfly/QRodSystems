/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the GeometryComponentWidget class
 */

#ifndef GEOMETRYCOMPONENTWIDGET_H
#define GEOMETRYCOMPONENTWIDGET_H

#include <QWidget>

namespace QRS
{

namespace Managers
{

//! Widget to construct a geometrical component of a rod
class GeometryComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeometryComponentWidget(QWidget* parent = nullptr);
    ~GeometryComponentWidget();

private:
    void createContent();
};

}

}

#endif // GEOMETRYCOMPONENTWIDGET_H
