include(ExternalProject)
project(SofaSuperBuild)

if(MSVC OR XCode)
# For multi configuration IDE environments start with release
  set(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE INTERNAL FORCE )
endif()

set(DEPENDENCIES)

set( Pulse_DIR "" CACHE PATH "Path to Pulse" )
set( Pulse_FOUND FALSE)
if(Pulse_DIR)
  if ( IS_DIRECTORY ${Pulse_DIR} )
    message(STATUS "Looking for your Pulse...")
    find_package( Pulse NO_MODULE )
    if ( Pulse_FOUND )
      message(STATUS "I found your Pulse!")
    else()
     message(STATUS "I could not find your Pulse!")
    endif()
  else()
    message(STATUS "Your Pulse_DIR is not a directory...")
    set( Pulse_FOUND FALSE)
  endif()
endif()

if(NOT Pulse_FOUND)
  set( Pulse_DIR "${CMAKE_BINARY_DIR}/Pulse-build" CACHE PATH "Path to Pulse" )
  message(STATUS "Downloading and build Pulse.")
  ExternalProject_Add(Pulse
    PREFIX Pulse
    GIT_REPOSITORY https://gitlab.kitware.com/physiology/engine.git
    GIT_TAG 75faa48ca4d788c579b815b1a36986f445d20d7e
    GIT_SHALLOW 1
    BINARY_DIR ${CMAKE_BINARY_DIR}/Pulse-build
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
      -DDOWNLOAD_BASELINES:BOOL=OFF
  )
  list(APPEND DEPENDENCIES "Pulse")
endif()

ExternalProject_Add(CustomApplication
  PREFIX CustomApplication
  DEPENDS ${DEPENDENCIES}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ${CMAKE_BINARY_DIR}/CustomApplication-build 
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  BUILD_AWAYS 1
  CMAKE_ARGS
    -DDO_SUPERBUILD:BOOL=OFF
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    ${CMAKE_CXX_COMPILER_LAUNCHER_FLAG}
    ${CMAKE_C_COMPILER_LAUNCHER_FLAG}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DMAKECOMMAND:STRING=${MAKECOMMAND}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}
    
   
    -DPulse_DIR:PATH=${Pulse_DIR}
  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
)
