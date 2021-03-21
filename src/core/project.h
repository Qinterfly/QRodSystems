/*!
 * \file
 * \author Pavel Lakiza
 * \date March 2021
 * \brief Declaration of QRS::Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QUuid>
#include <memory>
#include <list>
#include "dataobject.h"
#include "matrix.h"

class QString;

namespace QRS
{
using ScalarDataObject = DataObject<double>;
using VectorDataObject = DataObject<std::vector<ScalarDataType>>;
using MatrixDataObject = DataObject<Matrix<ScalarDataType>>;

template <typename A>
using ListPointers = std::list<std::shared_ptr<A>>;
using ListScalars  = ListPointers<ScalarDataObject>;
using ListVectors  = ListPointers<VectorDataObject>;
using ListMatrices = ListPointers<MatrixDataObject>;

//! Project class to interact with a created system of rods
class Project : public QObject
{
    Q_OBJECT

public:
    Project(QString name);
    virtual ~Project() = default;
    // Data objects
    ListScalars& getScalars() { return mScalars; }

public slots:
    void addScalar();
    void addVector();
    void addMatrix();

private:
    //! Unique project identifier
    const QUuid mID;
    //! Project name
    QString mName;
    //! Path to a file where a project is stored
    QString mFilePath;
    //! Number of modifications since last saving
    uint mNumModified = 0;
    //! Data objects
    ListScalars mScalars;
    ListVectors mVectors;
    ListMatrices mMatrices;
};

}

#endif // PROJECT_H
