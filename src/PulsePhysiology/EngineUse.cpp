#include "EngineUse.h"

// Include the various types you will be using in your code
#include "scenario/SEDataRequestManager.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "patient/SEPatient.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstance.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEEventHandler.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarLength.h"
#include "engine/SEEventHandler.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to forward log messages for application specific handling logic
///
/// \details
/// The data model has a logging capability that the engine utilizes to log any info to a log file.
//--------------------------------------------------------------------------------------------------
class MyLogger : public LoggerForward
{
public:
  virtual void ForwardDebug  (const std::string& msg, const std::string& origin) {}
  virtual void ForwardInfo   (const std::string& msg, const std::string& origin) {}
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) {}
  virtual void ForwardError  (const std::string& msg, const std::string& origin) {}
  virtual void ForwardFatal  (const std::string& msg, const std::string& origin) {}
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to forward event activity for application specific handling logic

//--------------------------------------------------------------------------------------------------
class MyEventHandler : public SEEventHandler
{
public:
  MyEventHandler() : SEEventHandler() {}
  virtual void HandlePatientEvent(cdm::ePatient_Event type, bool active, const SEScalarTime* time = nullptr) {}
  virtual void HandleAnesthesiaMachineEvent(cdm::eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr) {}
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage of using a Physiology Engine
///
/// \details
/// Usage of the Logger class
/// Loading a SEPatient from disk
/// Advancing time
/// Retrieving data from a Physiology Engine
//--------------------------------------------------------------------------------------------------
void HowToEngineUse()
{
  // Create an engine object

  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine("HowToEngineUse.log");
  pe->GetLogger()->Info("HowToEngineUse");


  // You can tell the PulseEngine to also direct any output
  // to your own function. For example if you want to capture and process messages
  // and handle them in your own way, Give PulseEngine an instance of the LoggerForward class
  // and PulseEngine will log and also call the provided method with the message.
  // You can specify a set of functions to be called for any one of the log levels
  //MyLogger myLogger;
  //pe->GetLogger()->SetForward(&myLogger);

  // You can tell the PulseEngine to also notify you of any events as well
  // You can Poll objects for events, for example
  // pe->GetPatient().

  // The first order of business is to initialize the engine by loading a patient state.
  // Patient states provided in the SDK are the state of the engine at the time they stabilize
  // More details on creating a patient and stabilizing the engine can be found in HowTo-CreateAPatient.cpp
  SEScalarTime startTime;


  startTime.SetValue(0, TimeUnit::s);
  if (!pe->LoadStateFile("./states/StandardMale@0s.pba", &startTime))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  // See below on how to save a state

  // There are specific events that can occur while the engine runs and you submit various actions
  // You can either poll/query the patient object to see if it is in a specific state
  pe->GetPatient().IsEventActive(cdm::ePatient_Event_CardiacArrest);
  // You can also derive a callback class that will be called whenever an Event is entered or exited by the patient
  //MyEventHandler myEventHandler;
  //pe->SetEventHandler(&myEventHandler);

  SESubstance* O2 = pe->GetSubstanceManager().GetSubstance("Oxygen");
  SESubstance* CO2 = pe->GetSubstanceManager().GetSubstance("CarbonDioxide");

  // The tracker is responsible for advancing the engine time AND outputting the data requests below at each time step
  // If you do not wish to write data to a file, you do not need to make any data requests
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects 
  // defined in the Physiology.xsd file
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");  
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, *O2, "PartialPressure");
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, *CO2, "PartialPressure");
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Lungs, "Volume");
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest(pulse::PulmonaryCompartment::Carina, "InFlow");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToEngineUse.csv");

  // We are ready to execute the engine
  // simply tell the engine how long you would like it to execute
  tracker.AdvanceModelTime(5);// Note this tracker class takes in seconds
  // If not using the tracker call
  // pe->AdvanceModelTime(5,TimeUnit::s);
  // If you do not provide a time, the engine will execute 1 time step, the Pulse timestep can be retrieved with:
  //pe->GetTimeStep(TimeUnit::s);
  
  // Engine Data is available in 3 general forms
  // Assessments - Data formed at a level of a clinicians report. 
  //               This is high level data, such as a mean or generalized data 
  //               value intended to give a general overview of the state of the system.
  // Systems - System specific data properties that provide more specific information for a specific system
  //           A system can be a physiological system (cardiovascular, respiratory, etc.). 
  //           It could also be a piece of equipment (anesthesia machine, ECG, etc.)
  // Compartments - Compartments are a more detailed view into the components that make up a system (Anatomy or Equipment)
  //                compartments reflect the low level data such as Volume, Pressure, Flow, and Substance Quantities (Mass/Concentration Volume/VolumeFraction)
  //                Compartments are contained in a hierarchical object model that  encapsulates the body and its anatomy down to organs and organ components
  //                For example the lung compartment is made up of the left lung and right lung

  // Here we demostrate pulling data from various physiology systems
  
  pe->GetLogger()->Info(std::stringstream() << "HeartRate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "BloodVolume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "ArterialSystolicPressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "ArterialDiastolicPressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "MeanArterialPressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);

  // Here we demonstrate pulling data from a compartment

  pe->GetLogger()->Info(std::stringstream() << "Invasive Blood Pressure : " << pe->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta)->GetPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  
  pe->GetLogger()->Info(std::stringstream() << "RespirationRate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << pe->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "End Tidal CarbonDioxide Fraction : "  << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxideFraction());

  // Using the compartment we can get more detailed respiratory information
  // Here we will test the flow of air in the carina compartment. A positive flow is an inhale and a negative flow is an exhale.
  // We can get the amount of CO2 exhaled and O2 inhaled by looking at the volume fraction of the carina of a particular substance
    
  const SEGasCompartment* carina = pe->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina);
  if (carina->GetInFlow(VolumePerTimeUnit::L_Per_s)>0)
  {// We are inhaling, so let's grab the amount of O2 coming into the body
    pe->GetLogger()->Info(std::stringstream() << "O2 Inhaled " << carina->GetSubstanceQuantity(*O2)->GetVolume(VolumeUnit::mL) << VolumeUnit::mL);
  }
  else
  {// We are exhaling, so let's grab the amount of CO2 that is leaving the body
    pe->GetLogger()->Info(std::stringstream() << "CO2 Exhaled " << carina->GetSubstanceQuantity(*CO2)->GetVolume(VolumeUnit::mL) << VolumeUnit::mL);
  }

  pe->GetLogger()->Info(std::stringstream() << "OxygenSaturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Blood pH : " << pe->GetBloodChemistrySystem()->GetBloodPH());
  //  You should save off the SESubstanceQuantity* if you will need it more than once
  pe->GetLogger()->Info(std::stringstream() << "Lactate Concentration : " << pe->GetSubstanceManager().GetSubstance("Lactate")->GetBloodConcentration(MassPerVolumeUnit::mg_Per_dL) << MassPerVolumeUnit::mg_Per_dL);
  pe->GetLogger()->Info(std::stringstream() << "Core Body Temperature : " << pe->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);


  // Save the state of the engine
  pe->SaveState("./states/FinalEngineUseState.pba");

  pe->GetLogger()->Info("Finished");
}
