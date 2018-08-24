/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineUse.h"

// Include the various types you will be using in your code
#include "scenario/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
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
/// Usage for applying an Asthma Attack insult to the patient
///
/// \details
/// Refer to the SEAsthmaAttack class
//--------------------------------------------------------------------------------------------------
void HowToAsthmaAttack() 
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("Asthma.log");
  pe->GetLogger()->Info("HowToAsthmaAttack");
  if (!pe->LoadStateFile("./states/StandardMale@0s.pba"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
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
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HemoglobinContent",MassUnit::g);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Carina, "InFlow");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("Asthma.csv");

  // Advance some time to get some healthy data
  tracker.AdvanceModelTime(50);
  // Cache off compartments of interest!
  const SEGasCompartment* carina = pe->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina);
  
  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"InspiratoryExpiratoryRatio : " << pe->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio());
  pe->GetLogger()->Info(std::stringstream() <<"Carina InFlow : " << carina->GetInFlow(VolumePerTimeUnit::L_Per_s) << VolumePerTimeUnit::L_Per_s);;

  // Asthma Attack Starts - instantiate an asthma attack action and have the engine process it
  // Asthma is a common inflammatory disease of the airways where air flow into the lungs is partially obstructed. 
  // Acute asthma is an exacerbation of asthma that does not respond to standard treatments. 
  // Pulse models asthma by increasing the airway flow resistance in the circuit model. 
  // The higher the severity, the more severe the asthma attack.
  SEAsthmaAttack asthmaAttack;
  asthmaAttack.GetSeverity().SetValue(0.3);
  pe->ProcessAction(asthmaAttack);

  tracker.AdvanceModelTime(550);

  pe->GetLogger()->Info("The patient has been having an asthma attack for 550s");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"InspiratoryExpiratoryRatio : " << pe->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio());
  pe->GetLogger()->Info(std::stringstream() <<"Carina InFlow : " << carina->GetInFlow(VolumePerTimeUnit::L_Per_s) << VolumePerTimeUnit::L_Per_s);;

  // Asthma Attack Stops
  asthmaAttack.GetSeverity().SetValue(0.0);
  pe->ProcessAction(asthmaAttack);
  
  // Advance some time while the patient catches their breath
  tracker.AdvanceModelTime(200);

  pe->GetLogger()->Info("The patient has NOT had an asthma attack for 200s");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"InspiratoryExpiratoryRatio : " << pe->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio());
  pe->GetLogger()->Info(std::stringstream() <<"Carina InFlow : " << carina->GetInFlow(VolumePerTimeUnit::L_Per_s) << VolumePerTimeUnit::L_Per_s);;
  pe->GetLogger()->Info("Finished");
}
