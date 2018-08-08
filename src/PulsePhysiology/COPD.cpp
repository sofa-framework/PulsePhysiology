/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

//#include "EngineUse.h"

// Include the various types you will be using in your code
#include "scenario/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a COPD condition to the patient
///
/// \details
/// Refer to the SEChronicObstructivePulmonaryDisease class
//--------------------------------------------------------------------------------------------------
void HowToCOPD()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("HowToCOPD.log");
  pe->GetLogger()->Info("HowToCOPD");
  
  // Since this is a condition, we do not provide a starting state
  // You will need to initialize the engine to this condition
  // You could then save out your own state and use it in the future

  SEChronicObstructivePulmonaryDisease COPD;
  COPD.GetBronchitisSeverity().SetValue(0.5);
  COPD.GetEmphysemaSeverity().SetValue(0.7);
  std::vector<const SECondition*> conditions;
  conditions.push_back(&COPD);

  if (!pe->InitializeEngine("StandardMale.pba", &conditions))
  {
    pe->GetLogger()->Error("Could not load initialize engine, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects 
  // defined in the Physiology.xsd file
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HemoglobinContent", MassUnit::g);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Carina, "InFlow");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToCOPD.csv");

  // Advance some time to get some data
  tracker.AdvanceModelTime(500);

  pe->GetLogger()->Info("The patient is not very healthy");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"InspiratoryExpiratoryRatio : " << pe->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio());
  pe->GetLogger()->Info(std::stringstream() <<"Carina InFlow : " << pe->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina)->GetInFlow(VolumePerTimeUnit::L_Per_s) << VolumePerTimeUnit::L_Per_s);;
  pe->GetLogger()->Info("Finished");
}
