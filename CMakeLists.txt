cmake_minimum_required(VERSION 3.7)
project(PulsePhysiology VERSION 0.1)


find_package(SofaFramework REQUIRED)

find_package(Pulse REQUIRED)

set(HEADER_FILES config/PulsePhysiology.h)
set(SOURCE_FILES config/PulsePhysiology.cpp)

list(APPEND HEADER_FILES
	src/PulsePhysiology/EngineUse.h
)

list(APPEND SOURCE_FILES
    src/PulsePhysiology/main.cpp
)


#add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_PULSEPHYSIOLOGY")

message("CMAKE_THREAD_LIBS_INIT = ${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} SofaCore)
target_link_libraries(${PROJECT_NAME} debug "${Pulse_DEBUG_LIBS}")
target_link_libraries(${PROJECT_NAME} debug "${Pulse_LIB_ROOT_DIR/release}")
target_link_libraries(${PROJECT_NAME} optimized "${Pulse_LIBS}")
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
