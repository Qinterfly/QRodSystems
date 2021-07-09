/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the ConstraintRodComponentWidget class
 */

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QToolBar>
#include <set>
#include "constraintrodcomponentwidget.h"
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
    pMainLayout->setSpacing(0);
    // Set how to convert types into names
    specifyConstraintNames();
    // Arranging widgets
    createTableWidget();
    pMainLayout->addWidget(createToolBar());
    pMainLayout->addWidget(mpTableConstraint);
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(pMainLayout);
}

//! Create a toolbar to add and remove constraints
QToolBar* ConstraintRodComponentWidget::createToolBar()
{
    const int kIconSize = 22;
    QToolBar* pToolBar = new QToolBar();
    pToolBar->setIconSize(QSize(kIconSize, kIconSize));
    pToolBar->setContentsMargins(0, 0, 0, 0);
    // Actions
    QAction* pAction;
    pAction = pToolBar->addAction(QIcon(":/icons/table-row-add.svg"), tr("Add Row (A)"),
                                  this, &ConstraintRodComponentWidget::addRow);
    pAction->setShortcut(Qt::Key_A);
    pAction = pToolBar->addAction(QIcon(":/icons/table-row-delete.svg"), tr("Remove Row (D)"),
                                  this, &ConstraintRodComponentWidget::removeSelectedRows);
    pAction->setShortcut(Qt::Key_D);
    return pToolBar;
}

//! Create a table to construct constraints
void ConstraintRodComponentWidget::createTableWidget()
{
    mpTableConstraint = new QTableWidget();
    mpTableConstraint->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);
    mpTableConstraint->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    mpTableConstraint->setSortingEnabled(false);
    mpTableConstraint->setColumnCount(2);
    // Modifying the horizontal header
    QString indent;
    indent.fill(' ', 15);
    QString typeName = indent + tr("Type") + indent;
    mpTableConstraint->setHorizontalHeaderLabels({typeName, tr("Coordinate System")});
    QHeaderView* pHeader = mpTableConstraint->horizontalHeader();
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    // Modifying the vertical header
    pHeader = mpTableConstraint->verticalHeader();
    pHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    // Setting an item delegate
    mpItemDelegate = new ConstraintItemDelegate(mConstraintRodComponent, mTypeNames, mCoordinateSystemNames, mpTableConstraint);
    mpTableConstraint->setItemDelegate(mpItemDelegate);
    // Representing existed data
    representConstraintData();
    if (mpTableConstraint->rowCount() == 0)
        addRow();
    // Setting connections
    connect(mpItemDelegate, &ConstraintItemDelegate::typeCreated, this, &ConstraintRodComponentWidget::setConstraintData);
    connect(mpItemDelegate, &ConstraintItemDelegate::typeChanged, [this](int iRow, ConstraintRodComponent::ConstraintType type)
    {
        mConstraintRodComponent.removeConstraint(type);
        setConstraintData(iRow);
    });
    connect(mpItemDelegate, &ConstraintItemDelegate::coordinateSystemChanged, this, &ConstraintRodComponentWidget::setConstraintData);
}

//! Add a row at the end of the table
void ConstraintRodComponentWidget::addRow()
{
    int iRow = mpTableConstraint->rowCount();
    int numColumns = mpTableConstraint->columnCount();
    // Checking if the previous row is fully set
    if (iRow > 0)
    {
        bool isExit = false;
        for (int j = 0; j != numColumns; ++j)
            isExit = isExit || mpTableConstraint->item(iRow - 1, j)->data(Qt::DisplayRole).toString().isEmpty();
        if (isExit)
            return;
    }
    // Adding a row
    mpTableConstraint->setRowCount(iRow + 1);
    for (int j = 0; j != numColumns; ++j)
        mpTableConstraint->setItem(iRow, j, new QTableWidgetItem());
    // Setting the height of the table
    setTableHeight();
}

//! Remove selected rows from the table
void ConstraintRodComponentWidget::removeSelectedRows()
{
    QItemSelectionModel* pSelectionModel = mpTableConstraint->selectionModel();
    QModelIndexList selectedIndices = pSelectionModel->selectedIndexes();
    // Retrieve unique indices of rows
    std::set<int> selectedRows;
    for (QModelIndex const& index : selectedIndices)
        selectedRows.insert(index.row());
    if (selectedRows.empty())
        return;
    for (auto iter = selectedRows.rbegin(); iter != selectedRows.rend(); ++iter)
    {
        // Erasing constraints
        int iRow = *iter;
        QVariant data = getItemData(iRow, 0);
        if (!data.isNull())
        {
            auto type = (ConstraintRodComponent::ConstraintType)data.toInt();
            mConstraintRodComponent.removeConstraint(type);
        }
        // Removing rows
        mpTableConstraint->removeRow(iRow);
    }
    setTableHeight();
    emit modified();
}

//! Represent existing constraints
void ConstraintRodComponentWidget::representConstraintData()
{
    int iRow = mpTableConstraint->rowCount();
    auto const& constraints = mConstraintRodComponent.constraints();
    QTableWidgetItem* pItem;
    for (auto const& item : constraints)
    {
        addRow();
        // Type
        pItem = mpTableConstraint->item(iRow, 0);
        auto type = item.first;
        pItem->setData(Qt::UserRole, type);
        pItem->setData(Qt::DisplayRole, mTypeNames[type]);
        // Coordinate system
        pItem = mpTableConstraint->item(iRow, 1);
        auto coordinateSystem = item.second;
        pItem->setData(Qt::UserRole, coordinateSystem);
        pItem->setData(Qt::DisplayRole, mCoordinateSystemNames[coordinateSystem]);
        ++iRow;
    }
}

//! Change a constraint property
void ConstraintRodComponentWidget::setConstraintData(int iRow)
{
    int numColumns = mpTableConstraint->columnCount();
    std::vector<int> indData;
    indData.resize(numColumns);
    QVariant data;
    // Retrieving selected indices
    for (int j = 0; j != numColumns; ++j)
    {
        data = getItemData(iRow, j);
        if (data.isNull())
            return;
        indData[j] = data.toInt();
    }
    // Type
    auto type = (ConstraintRodComponent::ConstraintType)indData[0];
    // Coordinate system
    auto coordinateSystem = (ConstraintRodComponent::ConstraintCoordinateSystem)indData[1];
    // Setting constraint data
    mConstraintRodComponent.setConstraint(type, coordinateSystem);
    emit modified();
}

//! Set the height of the table to be enough to represent all rows
void ConstraintRodComponentWidget::setTableHeight()
{
    const int kExtend = 2;
    int numRows = mpTableConstraint->rowCount();
    int minHeight = mpTableConstraint->horizontalHeader()->height();
    for (int i = 0; i != numRows; ++i)
        minHeight += mpTableConstraint->rowHeight(i);
    mpTableConstraint->setMaximumHeight(minHeight + kExtend);
}

//! Retrieve item data
QVariant ConstraintRodComponentWidget::getItemData(int iRow, int iColumn)
{
    return mpTableConstraint->item(iRow, iColumn)->data(Qt::UserRole);
}

//! Specify names of constraints
void ConstraintRodComponentWidget::specifyConstraintNames()
{
    // Setting displacement types
    mTypeNames[ConstraintRodComponent::kDisplacementX] = "Displacement X";
    mTypeNames[ConstraintRodComponent::kDisplacementY] = "Displacement Y";
    mTypeNames[ConstraintRodComponent::kDisplacementZ] = "Displacement Z";
    // Setting rotation types
    mTypeNames[ConstraintRodComponent::kRotationX] = "Rotation X";
    mTypeNames[ConstraintRodComponent::kRotationY] = "Rotation Y";
    mTypeNames[ConstraintRodComponent::kRotationZ] = "Rotation Z";
    // Setting coordinate systems
    mCoordinateSystemNames[ConstraintRodComponent::kGlobal] = "Global";
    mCoordinateSystemNames[ConstraintRodComponent::kLocal] = "Local";
}
