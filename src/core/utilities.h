/*!
 * \file
 * \author Pavel Lakiza
 * \date May 2021
 * \brief Declaration of utilities
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSharedPointer>
#include "datatypes.h"

class QFile;
class QString;

namespace Utilities
{

namespace File
{
QPair<QRS::DataObjectType, QSharedPointer<QFile>> getDataObjectFile(QString const& path, QString const& fileName);
QString loadFileContent(QString const& path);
}

}

#endif // UTILITIES_H
