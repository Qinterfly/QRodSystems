/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2021
 * \brief Definition of the LoadRodComponentWidget class
 */

#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include "loadrodcomponentwidget.h"
#include "dataobjectlineedit.h"
#include "core/scalardataobject.h"
#include "core/vectordataobject.h"

using namespace QRS::Core;
using namespace QRS::Managers;

LoadRodComponentWidget::LoadRodComponentWidget(Core::LoadRodComponent& loadRodComponent, QString const& mimeType, QWidget* parent)
    : AbstractRodComponentWidget(mimeType, parent)
    , mLoadRodComponent(loadRodComponent)
{
    createContent();
}

//! Create all the widgets
void LoadRodComponentWidget::createContent()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    // Load type
    pMainLayout->addLayout(createLoadTypeLayout());
    // Base
    pMainLayout->addLayout(createBaseLayout());
    // Time
    pMainLayout->addWidget(createTimeGroup());
    // Spacer
    pMainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(pMainLayout);
}

//! Create a layout consisted of widgets to set loading parameters
QLayout* LoadRodComponentWidget::createBaseLayout()
{
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    std::function<void(AbstractDataObject const*)> setFun;
    // Direction vector
    pEdit = new DataObjectLineEdit(mLoadRodComponent.directionVector(), AbstractDataObject::ObjectType::kVector, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty<VectorDataObject>(pData, &LoadRodComponent::setDirectionVector); };
    pLayout->addWidget(new QLabel(tr("Direction vector: ")), 0, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &LoadRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 0, 1);
    pLayout->addWidget(mpLoadRodUnits, 0, 2);
    // Longitudinal function
    pEdit = new DataObjectLineEdit(mLoadRodComponent.longitudinalFunction(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty<ScalarDataObject>(pData, &LoadRodComponent::setLongitudinalFunction); };
    pLayout->addWidget(new QLabel(tr("Longitudinal function: ")), 1, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &LoadRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 1, 1, 1, 2);
    // Multiplier
    double const kMaxMultiplier = std::numeric_limits<float>::max();
    pLayout->addWidget(new QLabel(tr("Multiplier: ")), 2, 0);
    QDoubleSpinBox* pSpinBox = new QDoubleSpinBox();
    pSpinBox->setValue(mLoadRodComponent.multiplier());
    pSpinBox->setMaximum(kMaxMultiplier);
    pSpinBox->setMinimum(-kMaxMultiplier);
    connect(pSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value)
    {
        mLoadRodComponent.setMultiplier(value);
        emit modified();
    });
    pLayout->addWidget(pSpinBox, 2, 1, 1, 2);
    return pLayout;
}

//! Create a group of widgets which depend on time
QWidget* LoadRodComponentWidget::createTimeGroup()
{
    QGroupBox* pGroupBox = new QGroupBox(tr("Time dependency"));
    QGridLayout* pLayout = new QGridLayout();
    DataObjectLineEdit* pEdit;
    std::function<void(AbstractDataObject const*)> setFun;
    // Coefficient
    pEdit = new DataObjectLineEdit(mLoadRodComponent.timeCoefficient(), AbstractDataObject::ObjectType::kScalar, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty<ScalarDataObject>(pData, &LoadRodComponent::setTimeCoefficient); };
    pLayout->addWidget(new QLabel(tr("Coefficient: ")), 0, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &LoadRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 0, 1);
    // Rotation vector
    pEdit = new DataObjectLineEdit(mLoadRodComponent.timeRotationVector(), AbstractDataObject::ObjectType::kVector, mkMimeType);
    setFun = [this](AbstractDataObject const * pData) { setProperty<VectorDataObject>(pData, &LoadRodComponent::setTimeRotationVector); };
    pLayout->addWidget(new QLabel(tr("Rotation vector: ")), 1, 0);
    connect(pEdit, &DataObjectLineEdit::selected, setFun);
    connect(pEdit, &DataObjectLineEdit::editRequested, this, &LoadRodComponentWidget::editDataObjectRequested);
    pLayout->addWidget(pEdit, 1, 1);
    pGroupBox->setLayout(pLayout);
    return pGroupBox;
}

//! Create a layout consisted of widgets to set a load type and following state
QLayout* LoadRodComponentWidget::createLoadTypeLayout()
{
    QHBoxLayout* pLayout = new QHBoxLayout();
    // Load type
    mpLoadRodUnits = new QLabel();
    pLayout->addWidget(new QLabel(tr("Load type: ")));
    pLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    pLayout->addWidget(createLoadTypeComboBox());
    // Following state
    QCheckBox* pCheckBox = new QCheckBox("Following");
    pCheckBox->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    pCheckBox->setChecked(mLoadRodComponent.isFollowing());
    connect(pCheckBox, &QCheckBox::toggled, [this](bool flag)
    {
        mLoadRodComponent.setFollowingState(flag);
        emit modified();
    });
    pLayout->addWidget(pCheckBox);
    return pLayout;
}

//! Create a combobox to specify a type of load
QComboBox* LoadRodComponentWidget::createLoadTypeComboBox()
{
    QComboBox* pComboBox = new QComboBox();
    pComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    // Forced displacements and rotations
    pComboBox->addItem(tr("Forced displacements"), LoadRodComponent::LoadType::kForcedDisplacements);
    pComboBox->addItem(tr("Forced rotations"), LoadRodComponent::LoadType::kForcedRotations);
    pComboBox->insertSeparator(pComboBox->count());
    // Point force and moment
    pComboBox->addItem(tr("Point force"), LoadRodComponent::LoadType::kPointForce);
    pComboBox->addItem(tr("Point moment"), LoadRodComponent::LoadType::kPointMoment);
    pComboBox->insertSeparator(pComboBox->count());
    // Point mass and inertia
    pComboBox->addItem(tr("Point mass"), LoadRodComponent::LoadType::kPointMass);
    pComboBox->addItem(tr("Point moment of inertia"), LoadRodComponent::LoadType::kPointInertiaMoment);
    pComboBox->insertSeparator(pComboBox->count());
    // Linear and rotational damper
    pComboBox->addItem(tr("Point linear damper"), LoadRodComponent::LoadType::kPointLinearDamper);
    pComboBox->addItem(tr("Point rotational damper"), LoadRodComponent::LoadType::kPointRotationalDamper);
    pComboBox->insertSeparator(pComboBox->count());
    // Distributed force and moment
    pComboBox->addItem(tr("Distributed force"), LoadRodComponent::LoadType::kDistributedForce);
    pComboBox->addItem(tr("Distributed moment"), LoadRodComponent::LoadType::kDistributedMoment);
    pComboBox->insertSeparator(pComboBox->count());
    // Dynamic
    pComboBox->addItem(tr("Aerodynamic flow"), LoadRodComponent::LoadType::kAerodynamicFlow);
    pComboBox->addItem(tr("Acceleration"), LoadRodComponent::LoadType::kAcceleration);
    pComboBox->addItem(tr("Inner liquid flow"), LoadRodComponent::LoadType::kInnerLiquidFlow);
    pComboBox->insertSeparator(pComboBox->count());
    // Displacement and rotation damping
    pComboBox->addItem(tr("Displacement damping"), LoadRodComponent::LoadType::kDisplacementDamping);
    pComboBox->addItem(tr("Rotation damping"), LoadRodComponent::LoadType::kRotationDamping);
    pComboBox->insertSeparator(pComboBox->count());
    // Selecting the current type
    int numItems = pComboBox->count();
    LoadRodComponent::LoadType currentLoadType = mLoadRodComponent.loadType();
    for (int i = 0; i != numItems; ++i)
    {
        if ((LoadRodComponent::LoadType)pComboBox->itemData(i).toInt() == currentLoadType)
        {
            pComboBox->setCurrentIndex(i);
            break;
        }
    }
    setLoadUnits(currentLoadType);
    // Specifying connections
    connect(pComboBox, QOverload<int>::of(&QComboBox::activated), [this, pComboBox](int index)
    {
        LoadRodComponent::LoadType type = (LoadRodComponent::LoadType)pComboBox->itemData(index).toInt();
        mLoadRodComponent.setType(type);
        setLoadUnits(type);
        emit modified();
    });
    return pComboBox;
}

//! Set a property of a rod load
template<typename T>
void LoadRodComponentWidget::setProperty(AbstractDataObject const* pDataObject, auto setFun)
{
    (mLoadRodComponent.*setFun)((T const*)pDataObject);
    emit modified();
}

//! Set load units to show
void LoadRodComponentWidget::setLoadUnits(LoadRodComponent::LoadType type)
{
    QString units = "";
    switch (type)
    {
    case LoadRodComponent::LoadType::kForcedDisplacements:
        units = tr("(m)");
        break;
    case LoadRodComponent::LoadType::kForcedRotations:
        units = tr("(rad)");
        break;
    case LoadRodComponent::LoadType::kPointForce:
        units = tr("(N)");
        break;
    case LoadRodComponent::LoadType::kPointMoment:
        units = tr("(N*m)");
        break;
    case LoadRodComponent::LoadType::kPointMass:
        units = tr("(kg, m, m)");
        break;
    case LoadRodComponent::LoadType::kPointInertiaMoment:
        units = tr("(kg*m<sup>4</sup>)");
        break;
    case LoadRodComponent::LoadType::kPointLinearDamper:
        units = tr("(N/m*s)");
        break;
    case LoadRodComponent::LoadType::kPointRotationalDamper:
        units = tr("(N*m*s)");
        break;
    case LoadRodComponent::LoadType::kDistributedForce:
        units = tr("(N/m)");
        break;
    case LoadRodComponent::LoadType::kDistributedMoment:
        units = tr("(N)");
        break;
    case LoadRodComponent::LoadType::kAerodynamicFlow:
        units = tr("(m/s)");
        break;
    case LoadRodComponent::LoadType::kAcceleration:
        units = tr("(m/s<sup>2</sup>)");
        break;
    case LoadRodComponent::LoadType::kInnerLiquidFlow:
        units = tr("(m/s)");
        break;
    default:
        break;
    }
    mpLoadRodUnits->setText(units);
}
