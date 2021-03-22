/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of QRS::Project class
 */

#include <QDebug>
#include "project.h"
#include "scalardataobject.h"

using namespace QRS;

AbstractDataObject* createDataObject(DataObjectType type, QString const& name);

//! Construct a clean project with the user specified name
Project::Project(QString name)
    : mID(QUuid::createUuid())
    , mName(name)
{

}

//! Create a data object with the specified type
void Project::addDataObject(DataObjectType type)
{
    static const QString skName = "Object ";
    QString nameObj = skName + QString::number(AbstractDataObject::numberObjects() + 1);
    AbstractDataObject* pObject = createDataObject(type, nameObj);
    if (pObject)
    {
        mDataObjects.emplace(pObject->id(), std::shared_ptr<AbstractDataObject>(pObject));
        qInfo() << QString("New data object with ID %1 was added into the project").arg(QString::number(pObject->id()));
        emit dataObjectAdded();
    }
}

//! Copy data objects
std::unordered_map<DataIDType, AbstractDataObject*> Project::getDataObjects()
{
    std::unordered_map<DataIDType, AbstractDataObject*> result;
    for (auto& pItem : mDataObjects)
    {
        AbstractDataObject* obj = pItem.second->clone();
        result.emplace(obj->id(), obj);
    }
    return result;
}

//! Helper function to create DataObject instance by a type and name
AbstractDataObject* createDataObject(DataObjectType type, QString const& name)
{
    AbstractDataObject* pObject = nullptr;
    switch (type)
    {
    case DataObjectType::kScalar:
        pObject = new ScalarDataObject(name);
        break;
//    case DataObjectType::kVector:
//        pObject = new VectorDataObject(name);
//        break;
//    case DataObjectType::kMatrix:
//        pObject = new MatrixDataObject(name);
//        break;
//    case DataObjectType::kSurface:
//        pObject = new SurfaceDataObject(name);
//        break;
    }
    return pObject;
}
