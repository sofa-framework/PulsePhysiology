/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineUse.h"

// Include the various types you will be using in your code
#include "scenario/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SEPupillaryResponse.h"
#include "patient/actions/SEBrainInjury.h"
#include "properties/SEScalarFlowCompliance.h"
#include "properties/SEScalarFlowResistance.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalar0To1.h"

int GlasgowEstimator(double);

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for the Brain Injury Patient Insult
/// An acute, non-localized traumatic brain injury that results in increased flow resistance in the brain
///
/// \details
/// Refer to the SEBrainInjury class
//--------------------------------------------------------------------------------------------------
void HowToBrainInjury()
{
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("BrainInjury.log");
  
  pe->GetLogger()->Info("HowToBrainInjury");
  if (!pe->LoadStateFile("./states/StandardMale@0s.pba"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects 
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("rainInjury.csv");

  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << pe->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << pe->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min)));
  // You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated.   
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  tracker.AdvanceModelTime(30);
  
  // Create an SEBrainInjury object
  // Set the severity (a fraction between 0 and 1; for maximal injury, use 1.)  
  SEBrainInjury tbi;
  tbi.SetType(cdm::eBrainInjury_Type_Diffuse);// Can also be LeftFocal or RightFocal, and you will get pupillary effects in only one eye 
  tbi.GetSeverity().SetValue(0.2);
  pe->ProcessAction(tbi);
  pe->GetLogger()->Info("Giving the patient a brain injury.");

  // Advance time to see how the injury affects the patient
  tracker.AdvanceModelTime(90);

  pe->GetLogger()->Info(std::stringstream() << "The patient has had a brain injury for 90s, not doing well...");
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << pe->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << pe->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min)));
  // You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated. 
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  // You can remove a brain injury by setting the severity to 0, this will instantly remove the flow resistance in the brain, and the patient will recover.
  tbi.GetSeverity().SetValue(0.0);
  pe->ProcessAction(tbi);

  pe->GetLogger()->Info("Removing the brain injury.");

  tracker.AdvanceModelTime(90);

  pe->GetLogger()->Info(std::stringstream() << "The patient's brain injury has been removed for 90s; patient is much better");
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << pe->GetCardiovascularSystem()->GetIntracranialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << pe->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min)));// You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated. 
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  // A more severe injury has more pronounced effects
  tbi.GetSeverity().SetValue(1);
  pe->ProcessAction(tbi);
  pe->GetLogger()->Info("Giving the patient a severe brain injury.");

  tracker.AdvanceModelTime(300);

  // You can also get information from the compartment rather than the system, in case you want other metrics
  const SELiquidCompartment* brain = pe->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Brain);

  pe->GetLogger()->Info(std::stringstream() << "The patient has had a severe brain injury for 5 minutes");
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Carbon Dioxide Saturation : " << pe->GetBloodChemistrySystem()->GetCarbonDioxideSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Intracranial Pressure : " << brain->GetPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Perfusion Pressure : " << pe->GetCardiovascularSystem()->GetCerebralPerfusionPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cerebral Blood Flow : " << brain->GetInFlow(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Instantaneous GCS value : " << GlasgowEstimator(pe->GetCardiovascularSystem()->GetCerebralBloodFlow(VolumePerTimeUnit::mL_Per_min)));// You can get the following pupillary effects
  // Reactivity Change in pupil recation time to light. -1 complete reduction/no response, 0 is normal, and 1 is the fastest reaction time.
  // Pupil size change from normal. -1 is fully constricted, 0 is no change, +1 is fully dilated. 
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Left Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetLeftEyePupillaryResponse()->GetReactivityModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Size Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetSizeModifier());
  pe->GetLogger()->Info(std::stringstream() << "Right Eye Pupil Reactivity Modifier : " << pe->GetNervousSystem()->GetRightEyePupillaryResponse()->GetReactivityModifier());

  pe->GetLogger()->Info("Finished");
}

// The Glasgow Coma Scale (GCS) is commonly used to classify patient consciousness after traumatic brain injury.
// The scale runs from 3 to 15, with brain injury classified as:
// Severe (GCS < 9)
// Moderate (GCS 9-12)
// Mild (GCS > 12)
// Since Cerebral Blood Flow directly influences the amount of oxygen delivered to the brain, we can tie CBF to
// an expected GCS outcome. CBF below 8 mL per 100 g of brain tissue per minute results in infarction. CBF between
// 8 and 20 mL per 100 g of brain tissue per minute is the "penumbra" where recovery is possible if treatment is
// quick. From 20 to the normal range of 50 to 65 mL per 100 g of brain tissue per minute is the range where
// irreversible damage isn't as likely. Thus, we can estimate a GCS value using CBF (estimated using the 1450 g
// StandardMale brain):
// 
// CBF Value (mL/min)     GCS score
// ------------------     ----------
// <116                   3
// <151                   4
// <186                   5
// <220                   6
// <255                   7
// <290                   8
// <363                   9
// <435                   10
// <508                   11
// <580                   12
// <628                   13
// <725                   14
// 725-943                15
// Note that in Pulse, CBF is pulsatile
int GlasgowEstimator(double cbf)
{
  if (cbf < 116)
    return 3;
  else if (cbf < 151)
    return 4;
  else if (cbf < 186)
    return 5;
  else if (cbf < 220)
    return 6;
  else if (cbf < 255)
    return 7;
  else if (cbf < 290)
    return 8;
  else if (cbf < 363)
    return 9;
  else if (cbf < 435)
    return 10;
  else if (cbf < 508)
    return 11;
  else if (cbf < 580)
    return 12;
  else if (cbf < 628)
    return 13;
  else if (cbf < 725)
    return 14;
  else
    return 15;
}