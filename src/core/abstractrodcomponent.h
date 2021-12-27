/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Declaration of the AbstractRodComponent class
 */

#ifndef ABSTRACTRODCOMPONENT_H
#define ABSTRACTRODCOMPONENT_H

#include <QObject>
#include <QString>
#include <QDataStream>
#include "aliasdataset.h"

namespace QRS::Core
{

//! Component of the rod structure which characterizes one of its properties
class AbstractRodComponent : public QObject
{
public:
    enum ComponentType
    {
        kGeometry,
        kSection,
        kMaterial,
        kLoad,
        kConstraint,
        kMechanical
    };
    AbstractRodComponent(ComponentType componentType, QString const& name);
    virtual ~AbstractRodComponent() = 0;
    virtual AbstractRodComponent* clone() const = 0;
    virtual bool isDataComplete() const = 0;
    DataIDType id() const { return mID; }
    ComponentType componentType() const { return mkComponentType; }
    QString const& name() const { return mName; }
    void setName(QString const& name) { mName = name; }
    static DataIDType maxComponentID() { return smMaxComponentID; }
    static void setMaxComponentID(DataIDType iMaxComponentID) { smMaxComponentID = iMaxComponentID; }
    virtual void serialize(QDataStream& stream) const = 0;
    virtual void deserialize(QDataStream& stream, DataObjects const& dataObjects) = 0;
    friend QDataStream& operator<<(QDataStream& stream, AbstractRodComponent const& component);
    virtual void resolveReferences(DataObjects const& dataObjects) = 0;

protected:
    void writeDataObjectPointer(QDataStream& stream, AbstractDataObject const* pDataObject) const;
    AbstractDataObject const* readDataObjectPointer(QDataStream& stream, DataObjects const& dataObjects) const;
    AbstractDataObject const* getDataObject(DataObjects const& dataObjects, DataIDType id) const;
    AbstractDataObject const* substituteDataObject(DataObjects const& dataObjects, AbstractDataObject const* pDataObject) const;

protected:
    ComponentType const mkComponentType;
    QString mName;
    DataIDType mID;

private:
    static DataIDType smMaxComponentID;
};

//! Print a rod component to a stream
inline QDataStream& operator<<(QDataStream& stream, AbstractRodComponent const& component)
{
    component.serialize(stream);
    return stream;
}

}

#endif // ABSTRACTRODCOMPONENT_H
