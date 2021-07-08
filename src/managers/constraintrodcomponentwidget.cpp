/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the ConstraintRodComponentWidget class
 */

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include "constraintrodcomponentwidget.h"
#include "constraintitemdelegate.h"
#include "core/constraintrodcomponent.h"

using namespace QRS::Core;
using namespace QRS::Managers;

ConstraintRodComponentWidget::ConstraintRodComponentWidget(Core::ConstraintRodComponent& constraintRodComponent, QWidget* parent)
    : AbstractRodComponentWidget(QString(), parent)
    , mConstraintRodComponent(constraintRodComponent)
{
    createContent();
}

ConstraintRodComponentWidget::~ConstraintRodComponentWidget()
{
    delete mpItemDelegate;
}

//! Create all the widgets
void ConstraintRodComponentWidget::createContent()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    // Table
    createTableConstraintWidget();
    pMainLayout->addWidget(mpTableConstraint);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(pMainLayout);
}

//! Create a table to construct constraints
void ConstraintRodComponentWidget::createTableConstraintWidget()
{
    mpTableConstraint = new QTableWidget();
    mpTableConstraint->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);
    mpTableConstraint->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    mpTableConstraint->setSortingEnabled(false);
    mpTableConstraint->setColumnCount(3);
    mpTableConstraint->setEditTriggers(QAbstractItemView::AllEditTriggers);
    // Modifying the horizontal header
    mpTableConstraint->setHorizontalHeaderLabels({tr("         Type         "), tr("Direction"), tr("Coordinate System")});
    QHeaderView* pHeader = mpTableConstraint->horizontalHeader();
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    // Modifying the vertical header
    pHeader = mpTableConstraint->verticalHeader();
    pHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    // Setting an item delegate
    mpItemDelegate = new ConstraintItemDelegate(mConstraintRodComponent, mpTableConstraint);
    mpTableConstraint->setItemDelegate(mpItemDelegate);
    // Specifying the height of the table
    addTableConstraintRow();
    setTableConstraintHeight();
    // Setting connections
    connect(mpTableConstraint, &QTableWidget::itemChanged, this, &ConstraintRodComponentWidget::setConstraintData);
}

//! Set the height of the table to be enough to represent all the rows
void ConstraintRodComponentWidget::setTableConstraintHeight()
{
    int numRows = mpTableConstraint->rowCount();
    int minHeight = mpTableConstraint->horizontalHeader()->height();
    for (int i = 0; i != numRows; ++i)
        minHeight += mpTableConstraint->rowHeight(i);
    mpTableConstraint->setMaximumHeight(minHeight);
}

//! Add a row at the end of the table
void ConstraintRodComponentWidget::addTableConstraintRow()
{
    const QSignalBlocker blocker(mpTableConstraint);
    int iRow = mpTableConstraint->rowCount();
    mpTableConstraint->setRowCount(iRow + 1);
    for (int j = 0; j != 3; ++j)
    {
        QTableWidgetItem* pItem = new QTableWidgetItem();
        if (j > 0)
            pItem->setFlags(pItem->flags() & Qt::ItemIsEditable);
        mpTableConstraint->setItem(iRow, j, pItem);
    }
}

//! Change a constraint field
void ConstraintRodComponentWidget::setConstraintData(QTableWidgetItem* pItem)
{
    int iRow = pItem->row();
    int iColumn = pItem->column();
    ConstraintRodComponent::ConstraintType type = (ConstraintRodComponent::ConstraintType)getItemData(iRow, iColumn);
    if (iColumn != mpTableConstraint->columnCount() - 1)
    {
        QTableWidgetItem* pNextItem = mpTableConstraint->item(iRow, iColumn + 1);
        {
            const QSignalBlocker blocker(mpTableConstraint);
            pNextItem->setFlags(pItem->flags());
        }
    }
    else
    {
        ConstraintRodComponent::ConstraintDirection direction = (ConstraintRodComponent::ConstraintDirection)getItemData(iRow, 1);
        ConstraintRodComponent::ConstraintCoordinateSystem coordSystem = (ConstraintRodComponent::ConstraintCoordinateSystem)getItemData(iRow, 2);
        mConstraintRodComponent.setConstraint(type, direction, coordSystem);
    }
}

//! Retrieve item data
int ConstraintRodComponentWidget::getItemData(int iRow, int iColumn)
{
    return mpTableConstraint->item(iRow, iColumn)->data(Qt::UserRole).toInt();
}
