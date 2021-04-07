/*!
 * \file
 * \author Pavel Lakiza
 * \date April 2021
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
}

}

#endif // UTILITIES_H
