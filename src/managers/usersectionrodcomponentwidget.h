/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the UserSectionRodComponentWidget class
 */

#ifndef USERSECTIONRODCOMPONENTWIDGET_H
#define USERSECTIONRODCOMPONENTWIDGET_H

#include <QWidget>
#include "aliasdata.h"

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
class UserSectionRodComponentWidget : public QWidget
{
    Q_OBJECT

public:
    UserSectionRodComponentWidget(Core::UserSectionRodComponent& userSectionRodComponent,
                                  QString const& mimeType, QWidget* parent = nullptr);
signals:
    void modified();
    void editDataObjectRequested(Core::DataIDType id);

private:
    void createContent();
    QLayout* createAreaLayout();
    QWidget* createInertiaMomentsGroup();
    QWidget* createCenterCoordinatesGroup();
    void setProperty(Core::AbstractDataObject const* pDataObject, auto setFun);

private:
    QString const mkMimeType;
    Core::UserSectionRodComponent& mUserSectionRodComponent;
};

}

}
#endif // USERSECTIONRODCOMPONENTWIDGET_H
