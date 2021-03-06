/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of the ControlTabs class
 */

#ifndef CONTROLTABS_H
#define CONTROLTABS_H

#include <QWidget>

namespace QRS::App
{

//! A toolbar consisted of object designers
class ManagersTab : public QWidget
{
    Q_OBJECT

public:
    explicit ManagersTab(QWidget* parent = nullptr);
    ~ManagersTab() = default;

signals:
    void actionDataObjectsTriggered();
    void actionRodPropertiesTriggered();
    void actionRodConstructorTriggered();
};

}


#endif // CONTROLTABS_H
