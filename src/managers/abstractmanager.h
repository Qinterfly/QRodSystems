/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of the AbstractManager class
 */

#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSettings;
class QToolBar;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
}

namespace QRS
{

namespace Managers
{

//! Abstract manager to create objects of different types
class AbstractManager : public QDialog
{
    Q_OBJECT

public:
    enum ManagerType
    {
        kDataObjects,
        kRodComponents,
        kRodConstructor
    };
    AbstractManager(QString& lastPath, QSettings& settings,
                    ManagerType type, QString groupName, QWidget* parent = nullptr);
    virtual ~AbstractManager() = 0;
    void saveSettings();
    void restoreSettings();

signals:
    void closed(QRS::Managers::AbstractManager::ManagerType type);

public slots:
    virtual void apply() = 0;

protected:
    void closeEvent(QCloseEvent* pEvent) override;
    void setToolBarShortcutHints(QToolBar* pToolBar);

protected:
    // Dock manager
    ads::CDockManager* mpDockManager = nullptr;
    // Data
    QString& mLastPath;

private:
    QSettings& mSettings;
    ManagerType const mkType;
    QString const mkGroupName;
};

}

}

#endif // ABSTRACTMANAGER_H
