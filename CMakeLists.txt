cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Dose)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB MACRO_FILES "*.mac")

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB SCRIPTS "*.sh") 
file(COPY ${SCRIPTS} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB PROCESSING "processing/*.py") 
file(COPY ${PROCESSING} DESTINATION ${PROJECT_BINARY_DIR}/processing)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/processing/dose/plot)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/processing/dose/table)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/processing/activity/plot)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/processing/activity/table)

add_executable(doseAndActivity doseAndActivity.cc ${sources} ${headers})
target_link_libraries(doseAndActivity ${Geant4_LIBRARIES})

add_custom_target(Dose DEPENDS doseDetector)