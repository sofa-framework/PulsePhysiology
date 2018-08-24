#include <PulsePhysiology/PulseWidget.h>

#include "AirwayObstruction.cpp"
#include "AnesthesiaMachine.cpp"
#include "Asthma.cpp"
#include "BolusDrug.cpp"
#include "BrainInjury.cpp"
#include "COPD.cpp"
#include "CPR.cpp"
#include "LobarPneumonia.cpp"
#include "PulmonaryFunctionTest.cpp"
#include "Smoke.cpp"
#include "TensionPneumothorax.cpp"

#include <iostream>

namespace sofa
{
namespace pulsephysiology
{

PulseWidget::PulseWidget(QObject* parent)
    : QObject(parent)
{
    connect(this, &PulseWidget::scenarioChanged, this, &PulseWidget::onScenarioChanged);
}

PulseWidget::~PulseWidget()
{

}

void PulseWidget::onScenarioChanged()
{
    std::cout << "New scenario: " << m_scenario.toStdString() << std::endl;

    if ( m_scenario == "AirwayObstruction")
        HowToAirwayObstruction();
    else if ( m_scenario == "AnesthesiaMachine")
        HowToAnesthesiaMachine();
    else if ( m_scenario == "Asthma")
        HowToAsthmaAttack();
    else if ( m_scenario == "BolusDrug")
        HowToBolusDrug();
    else if ( m_scenario == "BrainInjury")
        HowToBrainInjury();
    else if ( m_scenario == "COPD")
        HowToCOPD();
    else if ( m_scenario == "CPR")
        HowToCPR();
    else if ( m_scenario == "LobarPneumonia")
        HowToLobarPneumonia();
    else if ( m_scenario == "PulmonaryFunctionTest")
        HowToPulmonaryFunctionTest();
    else if ( m_scenario == "Smoke")
        HowToSmoke();
    else if ( m_scenario == "TensionPneumothorax")
        HowToTensionPneumothorax();
    else if ( m_scenario == "AnesthesiaMachine")
        HowToAnesthesiaMachine();
    else if ( m_scenario == "")
        return;
    else {
        std::cout << "UNKNOWN STATE ENTERED" << std::endl;
        m_scenario.clear();
        emit scenarioChanged();
    }
}



} // namespace pulsephysiology
} // namespace sofa
