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
    , mIsModified(false)
{

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
        emit dataObjectAdded(id);
        setModified(true);
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
        setModified(true);
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
    setModified(true);
}

//! Set a modification state
void Project::setModified(bool modifiedState)
{
    mIsModified = modifiedState;
    emit modified(mIsModified);
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
    out << QDateTime::currentDateTime().toString(kDateFormat); // Current date and time
    out << kFileVersion;                                       // File version
    // 2. Project info
    out << mID;                                                // Unique identificator
    // 3. Data objects
    out << (quint32)AbstractDataObject::numberObjects();       // Raw number of objects of all types
    out << (quint32)ScalarDataObject::numberInstances();       // Raw number of scalars
    out << (quint32)VectorDataObject::numberInstances();       // Raw number of vectors
    out << (quint32)MatrixDataObject::numberInstances();       // Raw number of matrices
    out << (quint32)SurfaceDataObject::numberInstances();      // Raw number of surface
    out << (quint32)mDataObjects.size();                       // Number of object to be written/read
    for (auto& item : mDataObjects)
        out << *item.second;                                   // Data object content
    file.flush();
    file.close();
    // Renaming the project
    mName = baseFileName;
    mFilePath = filePath;
    qInfo() << tr("Project was saved to the file: %1").arg(mFilePath);
    setModified(false);
    return true;
}

//! Read a project from a file
Project::Project(QString const& path, QString const& fileName)
    : mIsModified(false)
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
    quint32 numObjects;
    in >> numObjects;
    AbstractDataObject::setNumberObjects(numObjects);
    in >> numObjects;
    ScalarDataObject::setNumberInstances(numObjects);
    in >> numObjects;
    VectorDataObject::setNumberInstances(numObjects);
    in >> numObjects;
    MatrixDataObject::setNumberInstances(numObjects);
    in >> numObjects;
    SurfaceDataObject::setNumberInstances(numObjects);
    mDataObjects.clear();
    DataObjectType type;
    QString name;
    ScalarDataObject* pScalar;
    VectorDataObject* pVector;
    MatrixDataObject* pMatrix;
    SurfaceDataObject* pSurface;
    in >> numObjects;
    for (quint32 i = 0; i != numObjects; ++i)
    {
        in >> type;
        in >> name;
        AbstractDataObject* pObject = nullptr;
        switch (type)
        {
        case (kScalar):
            pScalar = new ScalarDataObject(name);
            in >> *pScalar;
            pObject = pScalar;
            break;
        case (kVector):
            pVector = new VectorDataObject(name);
            in >> *pVector;
            pObject = pVector;
            break;
        case (kMatrix):
            pMatrix = new MatrixDataObject(name);
            in >> *pMatrix;
            pObject = pMatrix;
            break;
        case (kSurface):
            pSurface = new SurfaceDataObject(name);
            in >> *pSurface;
            pObject = pSurface;
            break;
        }
        mDataObjects.emplace(pObject->id(), std::shared_ptr<AbstractDataObject>(pObject));
    }
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





