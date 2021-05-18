/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Implementation of utilities
 */

#include <QDebug>
#include <QString>
#include <QFile>
#include <QDir>
#include <QPair>

#include "utilities.h"

using namespace QRS::Utilities;
using namespace QRS::Core;

//! Retrieve a pair consisted of a data object file and its type
QPair<DataObjectType, QSharedPointer<QFile>> File::getDataObjectFile(QString const& path, QString const& fileName)
{
    const QString kScalarFileName = "w1.prn";
    const QString kVectorFileName = "w3.prn";
    const QString kMatrixFileName = "w9.prn";
    const QString kSurfaceFileName = "xy.prn";
    std::function <bool(QString const&)> isNameEqual = [&fileName](QString const& name) { return !fileName.compare(name, Qt::CaseInsensitive); };
    DataObjectType type;
    if (isNameEqual(kScalarFileName))
        type = kScalar;
    else if (isNameEqual(kVectorFileName))
        type = kVector;
    else if (isNameEqual(kMatrixFileName))
        type = kMatrix;
    else if (isNameEqual(kSurfaceFileName))
        type = kSurface;
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

//! Load a style sheet
QString File::loadFileContent(QString const& path)
{
    QFile fileStyleSheet(path);
    QString content;
    if (!fileStyleSheet.exists())
    {
        qDebug() << QString("File located at %1 was not found").arg(path) << Qt::endl;
    }
    else
    {
        fileStyleSheet.open(QFile::ReadOnly | QFile::Text);
        QTextStream streamStyle(&fileStyleSheet);
        content = streamStyle.readAll();
        fileStyleSheet.close();
    }
    return content;
}
