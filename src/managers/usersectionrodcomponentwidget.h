/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the UserSectionRodComponentWidget class
 */

#ifndef USERSECTIONRODCOMPONENTWIDGET_H
#define USERSECTIONRODCOMPONENTWIDGET_H

#include <QWidget>

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
    void modified(bool flag = true);

private:
    void createContent();
    QLayout* createAreaLayout();
    QWidget* createInertiaMomentsGroup();
    QWidget* createCenterCoordinatesGroup();
    // Setters
    void setArea(Core::AbstractDataObject const* pDataObject);
    void setInertiaMomentTorsional(Core::AbstractDataObject const* pDataObject);
    void setInertiaMomentX(Core::AbstractDataObject const* pDataObject);
    void setInertiaMomentY(Core::AbstractDataObject const* pDataObject);
    void setInertiaProductXY(Core::AbstractDataObject const* pDataObject);
    void setCenterCoordinateX(Core::AbstractDataObject const* pDataObject);
    void setCenterCoordinateY(Core::AbstractDataObject const* pDataObject);

private:
    QString const mkMimeType;
    Core::UserSectionRodComponent& mUserSectionRodComponent;
};

}

}
#endif // USERSECTIONRODCOMPONENTWIDGET_H
