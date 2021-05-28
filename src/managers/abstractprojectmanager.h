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
    AbstractProjectManager(Core::Project& project, QSettings& settings, QString& lastPath, QString nameManager, QWidget* parent = nullptr);
    virtual ~AbstractProjectManager() = 0;
    void saveSettings();
    void restoreSettings();

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* pEvent) override;

protected:
    // Dock manager
    ads::CDockManager* mpDockManager = nullptr;
    // Data
    Core::Project& mProject;
    QSettings& mSettings;
    QString& mLastPath;
    QString mNameManager;
};

}

}

#endif // ABSTRACTPROJECTMANAGER_H
