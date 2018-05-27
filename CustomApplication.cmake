# Create an executable with your code like this:
project(CustomApplication)

set(SOURCE "CustomApplication.cpp"
#          "AnotherFile.cpp"
#          "AndAnotherFile.cpp"
)
add_executable(PulseClient ${SOURCE})

## This is how you connect that executable to Pulse

# Find the Pulse package

# Again, set Pulse_DIR to the install directory with the PulseConfig.cmake file, or make the developer set this when building the code
find_package(Pulse REQUIRED NO_MODULE) # If you don't, CMake will halt here
# You will now have access to all the variables set in the PulseConfig.cmake in the Pulse install directory
# Go open it and look through to see what you have
target_include_directories(PulseClient PRIVATE ${Pulse_INCLUDE_DIRS})
# Some debug libraries have different names than the release libs
target_link_libraries(PulseClient debug "${Pulse_DEBUG_LIBS}")
target_link_libraries(PulseClient optimized "${Pulse_LIBS}")

# That is it, your program is now linked to Pulse
