/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

// Note that this project is set with the following Additional Include Paths: ../include;../include/cdm;../include/cdm/bind
// This will build an executable that is intended to execute a how-to method

#include "CommonDataModel.h"
#include "PulsePhysiologyEngine.h"

// The following how-to functions are defined in their own file
void HowToEngineUse();
void HowToCreateAPatient();

void HowToAirwayObstruction();
void HowToAnesthesiaMachine();
void HowToAsthmaAttack();
void HowToBrainInjury();
void HowToBolusDrug();
void HowToConsumeNutrients();
void HowToCOPD();
void HowToCPR();
void HowToEnvironmentChange();
void HowToExercise();
void HowToHemorrhage();
void HowToLobarPneumonia();
void HowToMechanicalVentilation();
void HowToPulmonaryFunctionTest();
void HowToSmoke();
void HowToTensionPneumothorax();

void HowToConcurrentEngines();
void HowToRunScenario();
void HowToDynamicHemorrhage();

void HowToSandbox();

class SEDataRequest;

/// This class is here to demonstrate executing the engine
/// and populating a csv file with data from the engine 
/// This class will handle advancing time on the engine
class HowToTracker
{
private:
  double m_dT_s;  // Cached Engine Time Step
  PhysiologyEngine& m_Engine;
public:
  HowToTracker(PhysiologyEngine& engine);
  ~HowToTracker();

  // This class will operate on seconds
  void AdvanceModelTime(double time_s);
};
