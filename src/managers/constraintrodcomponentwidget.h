/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the ConstraintRodComponentWidget class
 */

#ifndef CONSTRAINTRODCOMPONENTWIDGET_H
#define CONSTRAINTRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QTableWidgetItem;
QT_END_NAMESPACE

namespace QRS
{

namespace Core
{
class ConstraintRodComponent;
}

namespace Managers
{

class ConstraintItemDelegate;

//! Widget to consturct constraints of a rod
class ConstraintRodComponentWidget : public AbstractRodComponentWidget
{
public:
    ConstraintRodComponentWidget(Core::ConstraintRodComponent& constraintRodComponent, QWidget* parent = nullptr);
    ~ConstraintRodComponentWidget();

private:
    void createContent();
    void createTableConstraintWidget();
    void setTableConstraintHeight();
    void addTableConstraintRow();
    int getItemData(int iRow, int iColumn);

private slots:
    void setConstraintData(QTableWidgetItem* pItem);

private:
    Core::ConstraintRodComponent& mConstraintRodComponent;
    QTableWidget* mpTableConstraint;
    ConstraintItemDelegate* mpItemDelegate;
};

}

}

#endif // CONSTRAINTRODCOMPONENTWIDGET_H
