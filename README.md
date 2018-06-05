Inspired from https://github.com/sofa-framework/sofa/tree/master/applications/plugins/CImgPlugin

## Build instructions

- Build SOFA v17.12 and install it in a separate directory, let's say */home/user/sofa/install/v17.12*

- Build Pulse and install it in a separate directory, let's say */home/user/pulse/install*

- Create some build dir on the same level as the plugin: PulsePhysiology-build

- cd into this build dir and run `cmake-gui ../PulsePhysiology`

- Add a variable `CMAKE_PREFIX_PATH` and set it to both SOFA and Pulse install dirs.  
In the example case: `CMAKE_PREFIX_PATH=/home/user/sofa/install/v17.12;/home/user/pulse/install`

- Configure + Generate

- Build the plugin
