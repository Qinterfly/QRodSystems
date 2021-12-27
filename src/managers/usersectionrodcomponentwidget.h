/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the UserSectionRodComponentWidget class
 */

#ifndef USERSECTIONRODCOMPONENTWIDGET_H
#define USERSECTIONRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"

namespace QRS
{

namespace Core
{
class UserSectionRodComponent;
class AbstractDataObject;
}

namespace Managers
{

//! Widget to construct a user-defined section of a rod
class UserSectionRodComponentWidget : public AbstractRodComponentWidget
{
public:
    UserSectionRodComponentWidget(Core::UserSectionRodComponent& userSectionRodComponent,
                                  QString const& mimeType, QWidget* parent = nullptr);

private:
    void createContent();
    QLayout* createAreaLayout();
    QWidget* createInertiaMomentsGroup();
    QWidget* createCenterCoordinatesGroup();
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);

private:
    Core::UserSectionRodComponent& mUserSectionRodComponent;
};

}

}

#endif // USERSECTIONRODCOMPONENTWIDGET_H
