/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of ControlTabs class
 */

#ifndef CONTROLTABS_H
#define CONTROLTABS_H

#include <QWidget>

//! A toolbar consisted of object designers
class ManagersTab : public QWidget
{
    Q_OBJECT

public:
    explicit ManagersTab(QWidget* parent = nullptr);
    ~ManagersTab() = default;
};



#endif // CONTROLTABS_H
