/*!
 * \file
 * \author Pavel Lakiza
 * \date June 2021
 * \brief Declaration of the AbstractRodComponent class
 */

#ifndef ABSTRACTRODCOMPONENT_H
#define ABSTRACTRODCOMPONENT_H

#include <QObject>
#include <QString>
#include "datatypes.h"

namespace QRS::Core
{

//! Component of the rod structure which characterizes one of its properties
class AbstractRodComponent : public QObject
{
    Q_OBJECT

public:
    enum ComponentType
    {
        kGeometry
    };
    AbstractRodComponent(ComponentType type, QString name);
    virtual ~AbstractRodComponent() = 0;
    virtual AbstractRodComponent* clone() const = 0;
    virtual bool isDataComplete() const = 0;
    DataIDType id() const { return mID; }
    ComponentType type() const { return mType; }

protected:
    const ComponentType mType;
    QString mName;
    DataIDType mID;

private:
    static quint32 smNumObjects;
};

}

#endif // ABSTRACTRODCOMPONENT_H
