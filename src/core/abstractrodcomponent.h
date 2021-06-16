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
#include <QDataStream>
#include "datatypes.h"

namespace QRS::Core
{

class AbstractDataObject;
class AbstractRodComponent;

using DataObjectGetter = std::function<AbstractDataObject const*(DataIDType id)>;

//! Component of the rod structure which characterizes one of its properties
class AbstractRodComponent : public QObject
{
    Q_OBJECT

public:
    enum ComponentType
    {
        kGeometry
    };
    AbstractRodComponent(ComponentType componentType, QString name);
    virtual ~AbstractRodComponent() = 0;
    virtual AbstractRodComponent* clone() const = 0;
    virtual bool isDataComplete() const = 0;
    DataIDType id() const { return mID; }
    ComponentType componentType() const { return mComponentType; }
    static quint32 numberComponents() { return smNumComponents; }
    static void setNumberComponents(quint32 numComponents) { smNumComponents = numComponents; }
    virtual void serialize(QDataStream& stream) const;
    virtual void deserialize(QDataStream& stream, DataObjectGetter const& getDataObject) = 0;
    friend QDataStream& operator<<(QDataStream& stream, AbstractRodComponent const& component);

protected:
    void writeDataObjectPointer(QDataStream& stream, AbstractDataObject const* pDataObject) const;
    AbstractDataObject const* readDataObjectPointer(QDataStream& stream, DataObjectGetter const& getDataObject) const;

protected:
    const ComponentType mComponentType;
    QString mName;
    DataIDType mID;

private:
    static quint32 smNumComponents;
};

//! Print a rod component to a stream
inline QDataStream& operator<<(QDataStream& stream, AbstractRodComponent const& component)
{
    component.serialize(stream);
    return stream;
}

}

#endif // ABSTRACTRODCOMPONENT_H
