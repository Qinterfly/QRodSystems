/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
 * \brief Implementation of utilities
 */

#include <QDebug>
#include <QString>
#include <QFile>
#include <QDir>
#include <QPair>

#include "utilities.h"

using namespace Utilities;

//! Retrieve a pair consisted of a data object file and its type
QPair<QRS::DataObjectType, QSharedPointer<QFile>> Utilities::File::getDataObjectFile(QString const& path, QString const& fileName)
{
    const QString kScalarFileName = "w1.prn";
    const QString kVectorFileName = "w3.prn";
    const QString kMatrixFileName = "w9.prn";
    const QString kSurfaceFileName = "xy.prn";
    std::function <bool(QString const&)> isNameEqual = [&fileName](QString const& name) { return !fileName.compare(name, Qt::CaseInsensitive); };
    QRS::DataObjectType type;
    if (isNameEqual(kScalarFileName))
        type = QRS::kScalar;
    else if (isNameEqual(kVectorFileName))
        type = QRS::kVector;
    else if (isNameEqual(kMatrixFileName))
        type = QRS::kMatrix;
    else if (isNameEqual(kSurfaceFileName))
        type = QRS::kSurface;
    else
        return {type, nullptr};
    QString filePath = path + QDir::separator() + fileName;
    QSharedPointer<QFile> pFile(new QFile(filePath));
    if (!pFile->open(QIODevice::ReadOnly))
    {
        qWarning() << QString("Data object cannot be read from the file: %1").arg(filePath);
        return {type, nullptr};;
    }
    return {type, pFile};
}
