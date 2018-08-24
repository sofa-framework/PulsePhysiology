#ifndef SOFA_PULSEPHYSIOLOGY_PULSEWIDGET_H_
#define SOFA_PULSEPHYSIOLOGY_PULSEWIDGET_H_

#define M_PI 3.14159265358979323846

#include <PulsePhysiology.h>

#include <QObject>
#include <QQmlParserStatus>
#include <QMap>
#include <QList>
#include <QString>
#include <QVariant>

#include <SofaQtQuickGUI/SofaScene.h>

#include <PulsePhysiologyEngine.h>
#include <system/physiology/SEBloodChemistrySystem.h>
#include <system/physiology/SECardiovascularSystem.h>
#include <system/physiology/SEEnergySystem.h>
#include <system/physiology/SERespiratorySystem.h>

namespace sofa
{
namespace pulsephysiology
{

class SOFA_PULSEPHYSIOLOGY_API PulseWidget : public QObject
{
    Q_OBJECT

public:
    PulseWidget(QObject* parent = nullptr);
    ~PulseWidget();
    sofa::qtquick::SofaScene* m_sofaScene;

    Q_PROPERTY(QString scenario READ scenario WRITE setScenario NOTIFY scenarioChanged)
    QString m_scenario;
    QString scenario() { return m_scenario; }
    void setScenario(const QString& s) { m_scenario = s; emit scenarioChanged(); }

signals:
    void scenarioChanged();
public slots:
    void onScenarioChanged();
};

} // namespace pulsephysiology
} // namespace sofa

#endif // SOFA_PULSEPHYSIOLOGY_PULSEWIDGET_H_
