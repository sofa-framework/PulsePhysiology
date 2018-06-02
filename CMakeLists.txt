cmake_minimum_required(VERSION 3.7)
project(PulsePhysiology)

set(PULSEPHYSIOLOGY_MAJOR_VERSION 0)
set(PULSEPHYSIOLOGY_MINOR_VERSION 1)
set(PULSEPHYSIOLOGY_VERSION ${PULSEPHYSIOLOGY_MAJOR_VERSION}.${PULSEPHYSIOLOGY_MINOR_VERSION})

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install location")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  
endif()

# Set the possible values of build type for cmake-gui

if(MSVC OR XCode)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)
find_package(SofaGeneral REQUIRED)

find_package(Pulse REQUIRED)

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

# Set up our directory structure for output libraries and binaries
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
  else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")
  endif()
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib")
endif()

set(SOURCE_FILES
    initPulsePhysiology.cpp
)

#------------------------------------------------------------------------------

find_package(Pulse REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Pulse_INCLUDE_DIRS}
  )

  add_executable(${project_name} C CXX ${${project_name}_SOURCE_FILES})
  include_directories(${project_name} ${Pulse_INCLUDE_DIRS})
  
  
  # On Linux, you may want to link to the release libs while debugging
  
  target_link_libraries(${project_name} debug "${Pulse_DEBUG_LIBS}")
  target_link_libraries(${project_name} debug "${Pulse_LIB_ROOT_DIR/release}")
  target_link_libraries(${project_name} optimized "${Pulse_LIBS}")
  
install(TARGETS "${project_name}" 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

file(COPY data DESTINATION ${Pulse_INSTALL}/bin)

# Need to support debug still
if(WIN32)
  set(postfix)#for debug builds this should be "d"
  

