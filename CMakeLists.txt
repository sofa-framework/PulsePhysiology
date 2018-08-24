cmake_minimum_required(VERSION 3.7)
project(PulsePhysiology VERSION 0.1)


find_package(SofaFramework REQUIRED)
find_package(Pulse REQUIRED)
find_package(SofaQtQuickGUI REQUIRED)


set(HEADER_FILES
    config/PulsePhysiology.h
    src/PulsePhysiology/PulseWidget.h
    )
set(MOC_HEADER_FILES
    src/PulsePhysiology/PulseWidget.h
    src/PulsePhysiology/EngineUse.h
    )

set(SOURCE_FILES
    config/PulsePhysiology.cpp
    src/PulsePhysiology/PulseWidget.cpp
#    src/PulsePhysiology/main.cpp
    )

set(QML_FILES
    src/PulsePhysiology/qml/SofaWidgets/PulseWidget.qml
    )

set(QRC_FILES
    src/PulsePhysiology/qml/PulseWidget_qrc.qrc
    )

# to fix a bug when one is compiling a debug version of the code
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG)
endif()
add_definitions(-DQT_PLUGIN)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    if(_variableName MATCHES "SofaQtQuickGUI")
#        message("${_variableName} = ${${_variableName}}")
#    endif()
#endforeach()

#set(QML_DIRS
#    ${SofaQtQuickGUI_INCLUDE_DIR}/../../../include/SofaQtQuickGUI/data/qml
#    ${SofaQtQuickGUI_INCLUDE_DIR}/../../../include/SofaQtQuickGUI/data/qml/SofaBasics
#    ${SofaQtQuickGUI_INCLUDE_DIR}/../../../include/SofaQtQuickGUI/data/qml/SofaApplication
#    ${SofaQtQuickGUI_INCLUDE_DIR}/../../../include/SofaQtQuickGUI/data/qml/SofaSceneItemModel
#    ${SofaQtQuickGUI_INCLUDE_DIR}/../../../include/SofaQtQuickGUI/data/qml/SofaWidgets
#    ${CMAKE_CURRENT_LIST_DIR}/data/qml
#    ${CMAKE_CURRENT_LIST_DIR}/data/qml/SofaWidgets
#    )

#list(APPEND QML_IMPORT_PATH
#    src/PulsePhysiology/qml
#    src/PulsePhysiology/qml/SofaWidgets
#    )

message("QML_IMPORT_PATH = ${QML_IMPORT_PATH}")
set(QML_IMPORT_PATH "${QML_IMPORT_PATH}" CACHE STRING "Qt Creator extra qml import paths" FORCE)

qt5_wrap_cpp(MOC_FILES ${MOC_HEADER_FILES}) # do not use "set(CMAKE_AUTOMOC ON)" since all the mocced files will be compiled by a single compilation unit leading to a "out of heap space" issue on MSVC
qt5_add_resources(RESOURCE_FILES ${QRC_FILES})

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${MOC_FILES} ${RESOURCE_FILES} ${QML_FILES} ${QRC_FILES})
#add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${QML_FILES} ${QRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_PULSEPHYSIOLOGY")

message("CMAKE_THREAD_LIBS_INIT = ${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} SofaCore SofaQtQuickGUI)
#target_link_libraries(${PROJECT_NAME} debug "${Pulse_DEBUG_LIBS}")
#target_link_libraries(${PROJECT_NAME} debug "${Pulse_LIB_ROOT_DIR/release}")
target_link_libraries(${PROJECT_NAME} "${Pulse_LIBS}")
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

message("Pulse_INCLUDE_DIRS = ${Pulse_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/config>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>")
target_include_directories(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${Pulse_INCLUDE_DIRS}>")

# install pulse components
install(FILES     "${Pulse_DIR}/bin/UCEDefs.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/config" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/ecg" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/environments" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/nutrition" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/patients" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
# install(DIRECTORY "${Pulse_DIR}/bin/resource" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/states" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/substances" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
install(DIRECTORY "${Pulse_DIR}/bin/verification/scenarios" OPTIONAL DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


## Install rules for the library; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})
