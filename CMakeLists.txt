cmake_minimum_required(VERSION 3.7)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install location")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  
endif()

# Superbuild stuff
option(DO_SUPERBUILD "Download and build any project dependencies" ON)
if (DO_SUPERBUILD)
  include("CMake/SuperBuild.cmake")
  return()
endif()

# Set the possible values of build type for cmake-gui

if(MSVC OR XCode)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(project_name PulsePhysiology)
project(${project_name})

set(${project_name}_CMAKE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

find_package(Qt5 COMPONENTS Core Widgets Charts REQUIRED)


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

set(initPulsePhysiology.cpp
)

#------------------------------------------------------------------------------


find_package(Pulse REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Pulse_INCLUDE_DIRS}
  )

source_group("Generated" FILES
  ${MOC_BUILT_SOURCES}
  ${UI_BUILT_SOURCES}
)

set(${project_name}_SOURCE_FILES
  
)

  
#install (TARGETS "${project_name}" DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

file(COPY data DESTINATION ${Pulse_INSTALL}/bin)
# Need to support debug still
if(WIN32)
  set(postfix)#for debug builds this should be "d"
  
  install(FILES ${Qt_DLLs} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  install(DIRECTORY ${Qt5_DIR}/../../../plugins/platforms DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()

