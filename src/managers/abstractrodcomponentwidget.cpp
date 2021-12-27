/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the AbstractRodComponentWidget class
 */

#include "abstractrodcomponentwidget.h"
#include "core/abstractdataobject.h"
#include "dataobjectlineedit.h"

using namespace QRS::Core;
using namespace QRS::Managers;

AbstractRodComponentWidget::AbstractRodComponentWidget(QString const& mimeType, QWidget* parent)
    : QWidget(parent)
    , mkMimeType(mimeType)
{
    setPalette(parent->palette());
    setWindowState(Qt::WindowMaximized);
}

AbstractRodComponentWidget::~AbstractRodComponentWidget()
{

}

//! Specify connections of an editor which hold pointers to data objects of different types
void AbstractRodComponentWidget::setDataObjectEditConnections(DataObjectLineEdit* pEdit, DataObjectSetFun& setFun)
{
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &AbstractRodComponentWidget::editDataObjectRequested);
}
