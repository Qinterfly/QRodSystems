/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Definition of the DataPointerLineEdit class
 */

#include <QMimeData>
#include <QDragEnterEvent>
#include <QMenu>
#include "dataobjectlineedit.h"
#include "models/hierarchy/dataobjectshierarchyitem.h"

using namespace QRS::Managers;
using namespace QRS::Core;
using namespace QRS::HierarchyModels;

DataObjectLineEdit::DataObjectLineEdit(AbstractDataObject const* pDataObject, AbstractDataObject::ObjectType type,
                                       QString const& mimeType, QWidget* parent)
    : QLineEdit(parent)
    , mpDataObject(pDataObject)
    , mType(type)
    , mkMimeType(mimeType)
{
    if (mpDataObject)
        setText(mpDataObject->name());
    setReadOnly(true);
    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, &QLineEdit::customContextMenuRequested, this, &DataObjectLineEdit::showContextMenu);
}

//! Check if the type of the dropped item is correct
void DataObjectLineEdit::dragEnterEvent(QDragEnterEvent* pEvent)
{
    QMimeData const* pMimeData = pEvent->mimeData();
    if (pMimeData->hasFormat(mkMimeType))
    {
        QDataStream dataStream = pMimeData->data(mkMimeType);
        int numObjects;
        dataStream >> numObjects;
        if (numObjects == 0)
            return;
        DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)AbstractHierarchyItem::readPointer(dataStream);
        AbstractDataObject const* pDataObject = pItem->getDataObject();
        if (pDataObject && pDataObject->type() == mType)
            pEvent->acceptProposedAction();
    }
}

//! Process dropping of the approved item
void DataObjectLineEdit::dropEvent(QDropEvent* pEvent)
{
    QDataStream dataStream = pEvent->mimeData()->data(mkMimeType);
    int numObjects;
    dataStream >> numObjects;
    DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)AbstractHierarchyItem::readPointer(dataStream);
    mpDataObject = pItem->getDataObject();
    setText(mpDataObject->name());
    emit selected(mpDataObject);
}

//! Erase the data object address
void DataObjectLineEdit::keyPressEvent(QKeyEvent* pEvent)
{
    switch (pEvent->key())
    {
    case Qt::Key_E:
        edit();
        break;
    case Qt::Key_Delete:
        reset();
        break;
    }
}

//! Start the editing session when a double click event occurs
void DataObjectLineEdit::mouseDoubleClickEvent(QMouseEvent* /*pEvent*/)
{
    edit();
}

//! Show a menu to modify data
void DataObjectLineEdit::showContextMenu(const QPoint& point)
{
    QMenu* pMenu = new QMenu();
    QAction* pAction;
    // Edit a data object
    pAction = pMenu->addAction(QIcon(":/icons/edit-edit.svg"), "Edit", this, &DataObjectLineEdit::edit);
    pAction->setShortcut(Qt::Key_E);
    pMenu->addSeparator();
    // Remove a data object
    pAction = pMenu->addAction(QIcon(":/icons/delete.svg"), "Remove", this, &DataObjectLineEdit::reset);
    pAction->setShortcut(Qt::Key_Delete);
    // Show the menu
    pMenu->setEnabled(mpDataObject != nullptr);
    pMenu->exec(mapToGlobal(point));
    delete pMenu;
}

//! Erase the address of the data object
void DataObjectLineEdit::reset()
{
    mpDataObject = nullptr;
    setText("");
    emit selected(mpDataObject);
}

//! Try to edit a data object through managers
void DataObjectLineEdit::edit()
{
    if (mpDataObject)
        emit editRequested(mpDataObject->id());
}

