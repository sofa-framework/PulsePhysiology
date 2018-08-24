/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

//#include "EngineUse.h"

// Include the various types you will be using in your code
#include "scenario/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "compartment/SECompartmentManager.h"
#include "patient/actions/SECardiacArrest.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "patient/SEPatient.h"
#include "patient/actions/SEHemorrhage.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarForce.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "engine/SEEventHandler.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// A custom class for end users to listen to patient events
///
/// \details
/// It is possible during the course of the simulation for the patient or equipment to reach certain
/// states (hypoxia, tachycardia, etc.).  When a state change occurs, a physiology engine event
/// describing the state change is sent to the appropriate handlers.  Below is an example of a custom
/// event handler which we use to listen for patient and anesthesia machine events.
//--------------------------------------------------------------------------------------------------
class MyListener : public SEEventHandler, Loggable // We want to gain easy access to the engine logger
{
public:
  MyListener(Logger* logger) : Loggable(logger) {};
  virtual void HandlePatientEvent(cdm::ePatient_Event type, bool active, const SEScalarTime* time) override
  {
    GetLogger()->Info(std::stringstream() <<"Recieved Patient Event : " << cdm::ePatient_Event_Name(type));
  }

  virtual void HandleAnesthesiaMachineEvent(cdm::eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time) override
  {
    GetLogger()->Info(std::stringstream() <<"Recieved Anesthesia Machine Event : " << cdm::eAnesthesiaMachine_Event_Name(type));
  }
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for adminstering CPR to a patient
///
/// \details
/// Give patient Succinylcholine to stop heart, then give CPR
/// Refer to the SESubstanceBolus class
/// Refer to the SESubstanceManager class
/// This example also shows how to listen to patient events.
//--------------------------------------------------------------------------------------------------
void HowToCPR()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("CPR.log");
  pe->GetLogger()->Info("HowToCPR");
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
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartStrokeVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartEjectionFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput",VolumePerTimeUnit::mL_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Brain, "InFlow", VolumePerTimeUnit::mL_Per_min);

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("CPR.csv");

  // This is the total amount of time that CPR will be administered in seconds
  double durationOfCPR_Seconds = 120;
  
  // This is the frequency at which CPR is administered
  double compressionRate_BeatsPerMinute = 100;

  // This is where you specify how much force to apply to the chest. We have capped the applicable force at 600 N.
  double compressionForce_Newtons = 400;

  // This is the percent of time per period that the chest will be compressed e.g. if I have a 1 second period
  // (60 beats per minute) the chest will be compressed for 0.3 seconds
  double percentOn = .3;

  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Arterial Pressure : " << pe->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Ejection Fraction : " << pe->GetCardiovascularSystem()->GetHeartEjectionFraction());;

  tracker.AdvanceModelTime(50);

  // Put the patient into cardiac arrest
  SECardiacArrest c;
  c.SetState(cdm::eSwitch::On);
  pe->ProcessAction(c);
  
  pe->GetLogger()->Info("Giving the patient Cardiac Arrest.");

  // Let's add a listener which will print any state changes that patient undergoes
  MyListener l(pe->GetLogger());
  pe->GetPatient().ForwardEvents(&l);
  
  tracker.AdvanceModelTime(10);

  pe->GetLogger()->Info("It has been 10s since the administration, not doing well...");
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Arterial Pressure : " << pe->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Ejection Fraction : " << pe->GetCardiovascularSystem()->GetHeartEjectionFraction());;


  // After patient's heart is not beating, start doing CPR
  SEChestCompressionForce chestCompression;

  // The period is calculated via 1 / compressionRate.  Because the compression rate is given
  // in beats per minute it is divided by 60 to give a period in seconds.
  double pulsePeriod_s = 1.0 / (compressionRate_BeatsPerMinute / 60.0);

  // The amount of time the chest will be compressed, calculated from the period and percentOn
  double timeOn = percentOn * pulsePeriod_s;

  // The rest of the time there will be no force on the chest, this is calculated from the
  double timeOff = pulsePeriod_s - timeOn;

  // This timer is used to keep track of how long CPR has been administered
  double timer1 = 0;

  // This boolean is used to determine if the chest is compressed or not. It starts as true
  // so the chest will be compressed on the next calculation from the engine.
  bool pulseState = true;
  
  pe->GetLogger()->Info("Patient is in asystole. Begin performing CPR");
  while (timer1 < durationOfCPR_Seconds) // CPR is administered in this loop. It is time based, so after timer1 has exceeded the specified duration of CPR it will stop. set pulseState to true so that it will only exit AFTER a compression has been removed
  {
    if (pulseState) // check if the chest is supposed to be compressed. If yes...
    {
            // This calls the CPR function in the Cardiovascular system.  It sets the chest compression at the specified force.
      chestCompression.GetForce().SetValue(compressionForce_Newtons, ForceUnit::N);
      pe->ProcessAction(chestCompression);

            // Time is advanced until it is time to remove the compression
      tracker.AdvanceModelTime(timeOn);
            
            // Increment timer1 by the time the chest was compressed
      timer1 += timeOn;

            // Specify that the compression is to now be removed.
      pulseState = false;
    }
    else
    {
            // This removes the chest compression by specifying the applied force as 0 N
      chestCompression.GetForce().SetValue(0, ForceUnit::N);
      pe->ProcessAction(chestCompression);
            
            // Time is advanced until it is time to compress the chest again
      tracker.AdvanceModelTime(timeOff);
            
            // Increment timer1 by the time the chest was no compressed
      timer1 += timeOff;

            // Set pulse state back to true.
      pulseState = true;
    }
  }

  // Make sure that the chest is no longer being compressed
  if (chestCompression.GetForce().GetValue(ForceUnit::N) != 0)
  {
        // If it is compressed, set force to 0 to turn off
    chestCompression.GetForce().SetValue(0, ForceUnit::N);
    pe->ProcessAction(chestCompression);
  }

  // Do one last output to show status after CPR.
  pe->GetLogger()->Info("Check on the patient's status after CPR has been performed");
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Arterial Pressure : " << pe->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Ejection Fraction : " << pe->GetCardiovascularSystem()->GetHeartEjectionFraction());
  pe->GetLogger()->Info("Finished");
}