/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Implementation of the Project class
 *
 * Implementation of the methods to operate with input/output streams
 */

#include <QFileInfo>
#include <QDir>
#include <QDataStream>
#include <QDateTime>

#include "project.h"
#include "scalardataobject.h"
#include "vectordataobject.h"
#include "matrixdataobject.h"
#include "surfacedataobject.h"
#include "geometryrodcomponent.h"
#include "utilities.h"

using namespace QRS::Core;

const QString Project::skProjectExtension = ".qrs";

void readDataObjects(QDataStream& inputStream, DataObjects& dataObjects);
void readRodComponents(QDataStream& inputStream, DataObjects const& dataObjects, RodComponents& rodComponents);
void readHierarchyTree(QDataStream& inputStream, HierarchyTree& hierarchy);

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
    out << (quint32)mDataObjects.size();                       // Number of objects to be written/read
    for (auto& item : mDataObjects)
        out << *item.second;                                   // Data object content
    // 4. Hierarchy of data objects
    out << (quint32)mHierarchyDataObjects.size();              // Number of nodes in a hierarchy
    out << mHierarchyDataObjects;                              // Hierarchy of data objects
    // 5. Rod components
    out << (quint32)AbstractRodComponent::numberComponents();  // Raw number of components of all types
    out << (quint32)GeometryRodComponent::numberInstances();   // Raw number of geometrical components
    out << (quint32)mRodComponents.size();                     // Number of components to be written
    for (auto& item : mRodComponents)
        out << *item.second;
    // 6. Hierarchy of rod components
    out << (quint32)mHierarchyRodComponents.size();            // Number of nodes in a hierarchy
    out << mHierarchyRodComponents;                            // Hierarchy of rod components
    // Closing the file
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
    // Opening file to read
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
    readDataObjects(in, mDataObjects);
    // 4. Hierarchy of data objects
    readHierarchyTree(in, mHierarchyDataObjects);
    // 5. Rod components
    readRodComponents(in, mDataObjects, mRodComponents);
    // 6. Hierarchy of rod components
    readHierarchyTree(in, mHierarchyRodComponents);
    // Closing the file
    file.close();
    // Renaming the project
    mName = baseFileName;
    mFilePath = filePath;
    qInfo() << tr("Project was read from the file: %1").arg(mFilePath);
}

//! Import several data objects from a file
void Project::importDataObjects(QString const& path, QString const& fileName)
{
    auto [type, pFile] = Utilities::File::getDataObjectFile(path, fileName);
    if (pFile == nullptr)
        return;
    QTextStream stream(pFile.data());
    quint32 numDataObjects;
    stream.readLine();
    stream >> numDataObjects;
    stream.readLine();
    for (quint32 iDataObject = 0; iDataObject != numDataObjects; ++iDataObject)
    {
        AbstractDataObject* pDataObject = addDataObject(type);
        pDataObject->import(stream);
    }
    pFile->close();
}

//! Helper function to read a set of data objects from a stream
void readDataObjects(QDataStream& inputStream, DataObjects& dataObjects)
{
    quint32 numObjects;
    inputStream >> numObjects;
    AbstractDataObject::setNumberObjects(numObjects);
    inputStream >> numObjects;
    ScalarDataObject::setNumberInstances(numObjects);
    inputStream >> numObjects;
    VectorDataObject::setNumberInstances(numObjects);
    inputStream >> numObjects;
    MatrixDataObject::setNumberInstances(numObjects);
    inputStream >> numObjects;
    SurfaceDataObject::setNumberInstances(numObjects);
    AbstractDataObject::ObjectType type;
    QString name;
    inputStream >> numObjects;
    for (quint32 i = 0; i != numObjects; ++i)
    {
        inputStream >> type;
        inputStream >> name;
        AbstractDataObject* pObject = nullptr;
        switch (type)
        {
        case (AbstractDataObject::ObjectType::kScalar):
            pObject = new ScalarDataObject(name);
            break;
        case (AbstractDataObject::ObjectType::kVector):
            pObject = new VectorDataObject(name);
            break;
        case (AbstractDataObject::ObjectType::kMatrix):
            pObject = new MatrixDataObject(name);
            break;
        case (AbstractDataObject::ObjectType::kSurface):
            pObject = new SurfaceDataObject(name);
            break;
        }
        pObject->deserialize(inputStream);
        dataObjects.emplace(pObject->id(), pObject);
    }
}

//! Helper function to read rod components from a stream
void readRodComponents(QDataStream& inputStream, DataObjects const& dataObjects, RodComponents& rodComponents)
{
    quint32 numComponents;
    inputStream >> numComponents;
    AbstractRodComponent::setNumberComponents(numComponents);
    inputStream >> numComponents;
    GeometryRodComponent::setNumberInstances(numComponents);
    inputStream >> numComponents;
    AbstractRodComponent::ComponentType type;
    QString name;
    DataObjectGetter getDataObject = [&dataObjects](DataIDType id)
    {
        AbstractDataObject const* pDataObject = nullptr;
        auto iterFound = dataObjects.find(id);
        if (iterFound != dataObjects.end())
            pDataObject = iterFound->second;
        return pDataObject;
    };
    for (quint32 i = 0; i != numComponents; ++i)
    {
        inputStream >> type;
        inputStream >> name;
        AbstractRodComponent* pComponent = nullptr;
        switch (type)
        {
        case (AbstractRodComponent::ComponentType::kGeometry):
            pComponent = new GeometryRodComponent(name);
            break;
        }
        pComponent->deserialize(inputStream, getDataObject);
        rodComponents.emplace(pComponent->id(), pComponent);
    }
}

//! Helper function to read a hierarchial tree from a stream
void readHierarchyTree(QDataStream& inputStream, HierarchyTree& hierarchy)
{
    quint32 numNodes;
    inputStream >> numNodes;
    hierarchy = HierarchyTree(inputStream, numNodes);
}