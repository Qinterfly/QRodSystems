/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the ConstraintRodComponentWidget class
 */

#ifndef CONSTRAINTRODCOMPONENTWIDGET_H
#define CONSTRAINTRODCOMPONENTWIDGET_H

#include "abstractrodcomponentwidget.h"
#include "constraintitemdelegate.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QTableWidgetItem;
class QToolBar;
QT_END_NAMESPACE

namespace QRS
{

namespace Managers
{

//! Widget to consturct constraints of a rod
class ConstraintRodComponentWidget : public AbstractRodComponentWidget
{
public:
    ConstraintRodComponentWidget(Core::ConstraintRodComponent& constraintRodComponent, QWidget* parent = nullptr);
    ~ConstraintRodComponentWidget();

private:
    // Creating
    void createContent();
    QToolBar* createToolBar();
    void createTableWidget();
    // Interaction
    void addRow();
    void removeSelectedRows();
    void representConstraintData();
    // Helpers
    void setTableHeight();
    void specifyConstraintNames();
    QVariant getItemData(int iRow, int iColumn);

private slots:
    void setConstraintData(int iRow);

private:
    Core::ConstraintRodComponent& mConstraintRodComponent;
    QTableWidget* mpTableConstraint;
    ConstraintItemDelegate* mpItemDelegate;
    ConstraintTypeNames mTypeNames;
    ConstraintCoordinateSystemNames mCoordinateSystemNames;
};

}

}

#endif // CONSTRAINTRODCOMPONENTWIDGET_H
