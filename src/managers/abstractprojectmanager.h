/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the AbstractProjectManager class
 */

#ifndef ABSTRACTPROJECTMANAGER_H
#define ABSTRACTPROJECTMANAGER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
}

namespace QRS
{

namespace Core
{
class Project;
}

namespace Managers
{

//! Abstract manager to create objects of different types
class AbstractProjectManager : public QDialog
{
    Q_OBJECT

public:
    enum ManagerType
    {
        kDataObjects,
        kRodComponents,
        kRodConstructor
    };
    AbstractProjectManager(Core::Project& project, QString& lastPath, QSettings& settings,
                           ManagerType type, QString groupName, QWidget* parent = nullptr);
    virtual ~AbstractProjectManager() = 0;
    void saveSettings();
    void restoreSettings();

signals:
    void closed(QRS::Managers::AbstractProjectManager::ManagerType type);

public slots:
    virtual void apply() = 0;

protected:
    void closeEvent(QCloseEvent* pEvent) override;

protected:
    // Dock manager
    ads::CDockManager* mpDockManager = nullptr;
    // Data
    Core::Project& mProject;
    QString& mLastPath;

private:
    QSettings& mSettings;
    ManagerType const mType;
    QString const mGroupName;
};

}

}

#endif // ABSTRACTPROJECTMANAGER_H
