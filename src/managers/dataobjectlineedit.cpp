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
    , mMimeType(mimeType)
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
    if (pMimeData->hasFormat(mMimeType))
    {
        QDataStream dataStream = pMimeData->data(mMimeType);
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
    QDataStream dataStream = pEvent->mimeData()->data(mMimeType);
    int numObjects;
    dataStream >> numObjects;
    DataObjectsHierarchyItem* pItem = (DataObjectsHierarchyItem*)AbstractHierarchyItem::readPointer(dataStream);
    mpDataObject = pItem->getDataObject();
    setText(mpDataObject->name());
    emit dataObjectSelected(mpDataObject);
}

//! Erase the data object address
void DataObjectLineEdit::keyPressEvent(QKeyEvent* pEvent)
{
    if (pEvent->key() == Qt::Key_Delete)
        resetDataObjectPointer();
}

//! Show a menu to modify data
void DataObjectLineEdit::showContextMenu(const QPoint& point)
{
    QMenu* pMenu = new QMenu();
    QAction* pAction = pMenu->addAction(QIcon(":/icons/delete.svg"), "Remove", this, &DataObjectLineEdit::resetDataObjectPointer);
    pAction->setShortcut(Qt::Key_Delete);
    pMenu->exec(mapToGlobal(point));
    delete pMenu;
}

//! Erase the address of the data object
void DataObjectLineEdit::resetDataObjectPointer()
{
    mpDataObject = nullptr;
    setText("");
    emit dataObjectSelected(mpDataObject);
}

