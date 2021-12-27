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

namespace Core
{
class AbstractDataObject;
}

namespace Managers
{

class DataObjectLineEdit;

using DataObjectSetFun = std::function<void(Core::AbstractDataObject const*)>;

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
    void setDataObjectEditConnections(DataObjectLineEdit* pEdit, DataObjectSetFun& setFun);

protected:
    QString const mkMimeType;
};

}

}

#endif // ABSTRACTRODCOMPONENTWIDGET_H
