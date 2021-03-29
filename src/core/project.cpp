/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of the QRS::Project class
 */

#include <QDebug>
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDir>
#include <QDataStream>
#include <QDateTime>

#include "project.h"
#include "scalardataobject.h"
#include "vectordataobject.h"
#include "matrixdataobject.h"
#include "surfacedataobject.h"

using namespace QRS;

const QString Project::skProjectExtension = ".qrs";

AbstractDataObject* createDataObject(DataObjectType type, QString const& name);

//! Construct a clean project with the user specified name
Project::Project(QString const& name)
    : mID(QRandomGenerator::global()->generate())
    , mName(name)
{
    specifyConnections();
}

//! Retrieve a data object by identificator
std::shared_ptr<AbstractDataObject> Project::getDataObject(DataIDType id)
{
    if (mDataObjects.find(id) == mDataObjects.end())
        return nullptr;
    return mDataObjects[id];
}

//! Create a data object with the specified type
void Project::addDataObject(DataObjectType type)
{
    static const QString skName = "Object ";
    QString nameObj = skName + QString::number(AbstractDataObject::numberObjects() + 1);
    AbstractDataObject* pObject = createDataObject(type, nameObj);
    if (pObject)
    {
        DataIDType id = pObject->id();
        mDataObjects.emplace(id, std::shared_ptr<AbstractDataObject>(pObject));
        qInfo() << QString("New data object with ID %1 was added into the project").arg(QString::number(pObject->id()));
        emit dataObjectAdded(id);
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

//! Remove a data object by id
void Project::removeDataObject(DataIDType id)
{
    if (mDataObjects.find(id) != mDataObjects.end())
    {
        mDataObjects.erase(id);
        emit dataObjectRemoved(id);
    }
}

//! Substitute current data objects with new ones
void Project::setDataObjects(std::unordered_map<DataIDType, AbstractDataObject*> dataObjects)
{
    mDataObjects.clear();
    AbstractDataObject* pDataObject;
    for (auto& item : dataObjects)
    {
        pDataObject = item.second;
        mDataObjects.emplace(pDataObject->id(), std::shared_ptr<AbstractDataObject>(pDataObject->clone()));
    }
    emit allDataObjectsChanged();
}

//! Increase a number of modifications whenever a project is changed
void Project::setModified()
{
    ++mNumModified;
}

//! Specify connections between objects
void Project::specifyConnections()
{
    // While modified
    connect(this, &Project::allDataObjectsChanged, this, &Project::setModified);
    connect(this, &Project::dataObjectAdded, this, &Project::setModified);
    connect(this, &Project::dataObjectRemoved, this, &Project::setModified);
}

//! Save a project to a file
bool Project::save(QString const& path, QString const& fileName)
{
    // Formats
    const quint32 kFileVersion = 1;
    const QString kDateFormat = "dd.MM.yyyy - hh:mm:ss";
    // Retrieving the full path
    QString baseFileName = QFileInfo(fileName).baseName();
    QString filePath = QFileInfo(path + QDir::separator() + fileName + skProjectExtension).absoluteFilePath();
    // Opening file to write
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out.setFloatingPointPrecision(QDataStream::DoublePrecision);
    // Saving
    // 1. Header
    out << QDateTime::currentDateTime().toString(kDateFormat);
    out << kFileVersion;
    // 2. Project info
    out << mID;
    // 3. Data objects
    // TODO
    file.flush();
    file.close();
    // Renaming the project
    mName = baseFileName;
    mFilePath = filePath;
    qInfo() << tr("Project was saved to the file: %1").arg(mFilePath);
    return true;
}

//! Read a project from a file
Project::Project(QString const& path, QString const& fileName)
{
    QString baseFileName = QFileInfo(fileName).baseName();
    QString filePath = QFileInfo(path + QDir::separator() + fileName + skProjectExtension).absoluteFilePath();
    // Opening file to write
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qInfo() << tr("Project cannot be read from the file: %1").arg(filePath);
        return;
    }
    QDataStream in(&file);
    in.setFloatingPointPrecision(QDataStream::DoublePrecision);
    // Reading
    QString date;
    quint32 fileVersion;
    // 1. Header
    in >> date;
    in >> fileVersion;
    // 2. Project info
    in >> mID;
    // 3. Data objects
    // TODO
    file.close();
    // Renaming the project
    mName = baseFileName;
    mFilePath = filePath;
    qInfo() << tr("Project was read from the file: %1").arg(mFilePath);
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
    case DataObjectType::kVector:
        pObject = new VectorDataObject(name);
        break;
    case DataObjectType::kMatrix:
        pObject = new MatrixDataObject(name);
        break;
    case DataObjectType::kSurface:
        pObject = new SurfaceDataObject(name);
        break;
    }
    return pObject;
}





