/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the AbstractRodComponentWidget class
 */

#include "abstractrodcomponentwidget.h"

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
