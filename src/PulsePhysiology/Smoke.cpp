/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

//#include "EngineUse.h"

// Include the various types you will be using in your code
#include "scenario/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "system/environment/SEActiveConditioning.h"
#include "system/environment/actions/SEChangeEnvironmentConditions.h"
#include "system/environment/SEEnvironmentalConditions.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceCompound.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarFlowResistance.h"

#include "system/environment/conditions/SEInitialEnvironmentConditions.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a smokey environment to the patient
///
/// \details
/// Refer to the SEEnvironmentChange class
//--------------------------------------------------------------------------------------------------
void HowToSmoke()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("HowToSmoke.log");
  pe->GetLogger()->Info("HowToSmoke");
  /*
  // Smoke is made up of many things.
  // You will need to add 2 things to the environement to effectively model a smokey environment
  // A solid particle substance, and CarbonMonoxide
  // You can create your own environment file with these, the following would work

  SEInitialEnvironment ienv(pe->GetSubstanceManager());
  ienv.SetConditionsFile("./environments/CheyenneMountainFireFighter.pba");
  // You can set a file or the conditions object just like is shown below
  std::vector<const SECondition*> conditions;
  conditions.push_back(&ienv);

  if (!pe->InitializeEngine("StandardMale.pba", &conditions))
  {
    pe->GetLogger()->Error("Could not load initialize engine, check the error");
    return;
  }
  */
  
  if (!pe->LoadStateFile("./states/StandardMale@0s.pba"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Get some substances out we will use
  SESubstance* N2 = pe->GetSubstanceManager().GetSubstance("Nitrogen");
  SESubstance* O2 = pe->GetSubstanceManager().GetSubstance("Oxygen");
  SESubstance* CO2 = pe->GetSubstanceManager().GetSubstance("CarbonDioxide");
  SESubstance* CO = pe->GetSubstanceManager().GetSubstance("CarbonMonoxide");
  SESubstance* Particulate = pe->GetSubstanceManager().GetSubstance("ForestFireParticulate");


  


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
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SkinTemperature", TemperatureUnit::C);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CoreTemperature", TemperatureUnit::C);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalMetabolicRate", PowerUnit::W);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystemicVascularResistance", FlowResistanceUnit::mmHg_s_Per_mL);

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToEnvironmentChange.csv");

  // Advance some time to get some resting data
  tracker.AdvanceModelTime(5);

  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "CarbonDioxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Carbon Monoxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonMonoxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Pulse Oximetry : " << pe->GetBloodChemistrySystem()->GetPulseOximetry());
  // There are liquid compartments for each of the gas pulmonary compartments, these track the trasportation of liquid and solid substances through the pulmonary tract, and their deposition
  // Currently, since we have not changed the environment there is no Particulate or CO in the system, so the GetSubstanceQuantity call will return nullptr, so keep this commented
  //pe->GetLogger()->Info(std::stringstream() << "Particulate Deposition : " << pe->GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(*Particulate)->GetMassDeposited(MassUnit::ug) << MassUnit::ug);

  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Skin Temperature : " << pe->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  pe->GetLogger()->Info(std::stringstream() <<"Core Temperature : " << pe->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  pe->GetLogger()->Info(std::stringstream() <<"Total Metabolic Rate : " << pe->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W) << PowerUnit::W);
  pe->GetLogger()->Info(std::stringstream() <<"Systemic Vascular Resistance : " << pe->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL) << FlowResistanceUnit::mmHg_s_Per_mL);;

  // Here we will put this healty patient into a smokey environment.
  SEChangeEnvironmentConditions envChange(pe->GetSubstanceManager());
  // NOTE YOUR FRACTIONS MUST ADD UP TO 1.0
  envChange.GetConditions().GetAmbientGas(*N2).GetFractionAmount().SetValue(0.79008);
  envChange.GetConditions().GetAmbientGas(*O2).GetFractionAmount().SetValue(0.2095);
  envChange.GetConditions().GetAmbientGas(*CO2).GetFractionAmount().SetValue(4.0E-4);
  envChange.GetConditions().GetAmbientGas(*CO).GetFractionAmount().SetValue(2.0E-5);
  // Concentrations are independent and do not need to add up to 1.0
  envChange.GetConditions().GetAmbientAerosol(*Particulate).GetConcentration().SetValue(2.9, MassPerVolumeUnit::mg_Per_m3);
  pe->ProcessAction(envChange);
  tracker.AdvanceModelTime(30);

  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "CarbonDioxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Carbon Monoxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonMonoxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Pulse Oximetry : " << pe->GetBloodChemistrySystem()->GetPulseOximetry());
  // There are liquid compartments for each of the gas pulmonary compartments, these track the trasportation of liquid and solid substances through the pulmonary tract, and their deposition
  pe->GetLogger()->Info(std::stringstream() << "Particulate Deposition : " << pe->GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(*Particulate)->GetMassDeposited(MassUnit::ug) << MassUnit::ug);

  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Skin Temperature : " << pe->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  pe->GetLogger()->Info(std::stringstream() <<"Core Temperature : " << pe->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);
  pe->GetLogger()->Info(std::stringstream() <<"Total Metabolic Rate : " << pe->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W) << PowerUnit::W);
  pe->GetLogger()->Info(std::stringstream() <<"Systemic Vascular Resistance : " << pe->GetCardiovascularSystem()->GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL) << FlowResistanceUnit::mmHg_s_Per_mL);;

  // Here is the amount of particulate 

  pe->GetLogger()->Info("Finished");
}
