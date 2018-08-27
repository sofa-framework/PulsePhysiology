Inspired from https://github.com/sofa-framework/sofa/tree/master/applications/plugins/CImgPlugin

## Build instructions

- Build SOFA master with SofaQtQuick and install it in a separate directory, let's say */home/user/sofa/installWithQtQuick*
    - Clone master branch of https://github.com/sofa-framework/SofaQtQuick
    - Open CMake GUI on SOFA
    - Set `SOFA_EXTERNAL_DIRECTORIES=/path/to/SofaQtQuick`
    - Configure/generate, build and install SOFA

- Build Pulse and install it in a separate directory, let's say */home/user/pulse/install*

- Create some build dir on the same level as the plugin: PulsePhysiology-build

- cd into this build dir and run `cmake-gui ../PulsePhysiology`

- Add a variable `CMAKE_PREFIX_PATH` and set it to both SOFA and Pulse install dirs.  
In the example case: `CMAKE_PREFIX_PATH=/home/user/sofa/installWithQtQuick;/home/user/pulse/install`

- Also set `CMAKE_INSTALL_PREFIX` to the build dir.
In the example case : `CMAKE_INSTALL_PREFIX= /PulsePhysiology-build`

- Configure + Generate

- Build the plugin

## Using the built plugin

- Various patient states are available in *./states* directory inside PulsePhysiology-build. The default is StandardMale@0s.pba but can be modified while calling the LoadStateFile() function.

- cd into the build directory : PulsePhsyiology-build

- Type command `make` followed by `make install`

- Now to run simulation for a particular condition, run command `bin/PulsePhysiology condition`

- Lets say you want to run simulation for Pulmonary Function, the command will be `bin/PulsePhysiology PulmonaryFunctionTest"`

- Some of the other conditions are : `AirwayObstruction Asthma BrainInjury CPR TensionPneumothorax`

- The data from the simulations is stored in the `/PulsePhysiology-build` with the name as `condition`.log