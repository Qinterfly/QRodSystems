/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the AbstractRodComponentWidget class
 */

#ifndef ABSTRACTRODCOMPONENTWIDGET_H
#define ABSTRACTRODCOMPONENTWIDGET_H

#include <QWidget>
#include "core/aliasdata.h"

namespace QRS
{

namespace Managers
{

//! Widget to construct rod components of different types
class AbstractRodComponentWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractRodComponentWidget(QString const& mimeType, QWidget* parent = nullptr);
    virtual ~AbstractRodComponentWidget() = 0;

signals:
    void modified();
    void editDataObjectRequested(Core::DataIDType id);

protected:
    QString const mkMimeType;
};

}

}



#endif // ABSTRACTRODCOMPONENTWIDGET_H
