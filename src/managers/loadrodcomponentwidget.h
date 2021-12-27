/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the LoadRodComponentWidget class
 */

#ifndef LOADRODCOMPONENTWIDGET_H
#define LOADRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"
#include "core/loadrodcomponent.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
class AbstractDataObject;
}

namespace Managers
{

//! Widget to construct a load applied to a rod
class LoadRodComponentWidget : public AbstractRodComponentWidget
{
public:
    LoadRodComponentWidget(Core::LoadRodComponent& loadRodComponent, QString const& mimeType, QWidget* parent = nullptr);

private:
    void createContent();
    QLayout* createBaseLayout();
    QWidget* createTimeGroup();
    QLayout* createLoadTypeLayout();
    QComboBox* createLoadTypeComboBox();
    template<typename T>
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);
    void setLoadUnits(Core::LoadRodComponent::LoadType type);

private:
    Core::LoadRodComponent& mLoadRodComponent;
    QLabel* mpLoadRodUnits;
};

}

}

#endif // LOADRODCOMPONENTWIDGET_H
