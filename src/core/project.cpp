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

//! Add an instance of a scalar object
void Project::addScalar()
{
    static const QString skName = "Scalar ";
    QString nameObj = skName + QString::number(ScalarDataObject::getID() + 1);
    mScalars.push_back(std::shared_ptr<ScalarDataObject>(new ScalarDataObject(nameObj)));
}

//! Add an instance of a vector object
void Project::addVector()
{
    static const QString skName = "Vector ";
    QString nameObj = skName + QString::number(VectorDataObject::getID() + 1);
    mVectors.push_back(std::shared_ptr<VectorDataObject>(new VectorDataObject(nameObj)));
}

//! Add an instance of a matrix object
void Project::addMatrix()
{
    static const QString skName = "Matrix ";
    QString nameObj = skName + QString::number(MatrixDataObject::getID() + 1);
    mMatrices.push_back(std::shared_ptr<MatrixDataObject>(new MatrixDataObject(nameObj)));
}
