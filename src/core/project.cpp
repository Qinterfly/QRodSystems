/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Implementation of QRS::Project class
 */

#include "project.h"

using namespace QRS;

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
    AbstractDataObject* pObject;
    switch (type)
    {
    case DataObjectType::kScalar:
        pObject = new ScalarDataObject(nameObj);
        break;
    case DataObjectType::kVector:
        pObject = new VectorDataObject(nameObj);
        break;
    case DataObjectType::kMatrix:
        pObject = new MatrixDataObject(nameObj);
        break;
    case DataObjectType::kSurface:
        pObject = new SurfaceDataObject(nameObj);
        break;
    }
    mDataObjects.push_back(std::shared_ptr<AbstractDataObject>(pObject));
}

