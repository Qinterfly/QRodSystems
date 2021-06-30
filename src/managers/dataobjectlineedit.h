/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the DataPointerLineEdit class
 */

#ifndef DATAOBJECTLINEEDIT_H
#define DATAOBJECTLINEEDIT_H

#include <QLineEdit>
#include "core/abstractdataobject.h"

namespace QRS
{

namespace Managers
{

//! Line edit widget to hold a pointer to a data object
class DataObjectLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    DataObjectLineEdit(Core::AbstractDataObject const* pDataObject, Core::AbstractDataObject::ObjectType type,
                       QString const& mimeType, QWidget* parent = nullptr);
    ~DataObjectLineEdit() = default;

signals:
    void dataObjectSelected(Core::AbstractDataObject const* pDataObject);

private slots:
    void showContextMenu(const QPoint& point);
    void resetDataObjectPointer();

private:
    void dragEnterEvent(QDragEnterEvent* pEvent) override;
    void dropEvent(QDropEvent* pEvent) override;
    void keyPressEvent(QKeyEvent* pEvent) override;

private:
    Core::AbstractDataObject const* mpDataObject;
    Core::AbstractDataObject::ObjectType mType;
    QString const mkMimeType;
};

}

}

#endif // DATAOBJECTLINEEDIT_H
