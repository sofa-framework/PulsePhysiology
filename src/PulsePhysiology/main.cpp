/* Distributed under the Apache License, Version 2.0.*/
// Include the various types you will be using in your code

/// The class in this file is here to demonstrate executing the engine
/// and populating a txt file with data from the engine 
/// This class will handle advancing time on the engine
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
#include <string.h>

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a Hemorrhage insult to the patient
///
/// \details
/// Refer to the SEHemorrhage class
/// Refer to the SESubstanceManager class
/// Refer to the SESubstanceIVFluids class for applying an IV to the patient
//--------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if ( strcmp( argv[1], "AirwayObstruction") == 0 )
      HowToAirwayObstruction();
  else if ( strcmp( argv[1], "AnesthesiaMachine") == 0)
      HowToAnesthesiaMachine();
  else if ( strcmp( argv[1], "Asthma") == 0)
      HowToAsthmaAttack();
  else if ( strcmp( argv[1], "BolusDrug") == 0)
      HowToBolusDrug();
  else if ( strcmp( argv[1], "BrainInjury") == 0)
      HowToBrainInjury();
  else if ( strcmp( argv[1], "COPD") == 0)
      HowToCOPD();
  else if ( strcmp( argv[1], "CPR") == 0)
      HowToCPR();
  else if ( strcmp( argv[1], "LobarPneumonia") == 0)
      HowToLobarPneumonia();
  else if ( strcmp( argv[1], "PulmonaryFunctionTest") == 0)
      HowToPulmonaryFunctionTest();
  else if ( strcmp( argv[1], "Smoke") == 0)
      HowToSmoke();
  else if ( strcmp( argv[1], "TensionPneumothorax") == 0)
      HowToTensionPneumothorax();
  else if ( strcmp( argv[1], "AnesthesiaMachine") == 0)
      HowToAnesthesiaMachine();  
  else {
      std::cout<<"\nUNKNOWN STATE ENTERED \n Try again";
        }
}
