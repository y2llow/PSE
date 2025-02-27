cmake_minimum_required(VERSION 3.6)
project(ProjectTitle)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
# Remove the "-O2" flag if you have problems with the debugger.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)
# Set include dir
include_directories(./gtest/include)
# Set Library dir
link_directories(./gtest/lib)
# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES ./src/TicTacToe.cpp ./src/TicTacToeMain.cpp)
# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES ./src/TicTacToe.cpp ./src/TicTacToeTests.cpp
        Specificatie_1/1_1_Verkeerssituatie_inlezen/Verkeerssituatie_inlezen.cpp
        Specificatie_1/1_1_Verkeerssituatie_inlezen/Verkeerssituatie_inlezen.h
        Specificatie_1/Elementen/BAAN.cpp
        Specificatie_1/Elementen/BAAN.h
        Specificatie_1/Elementen/VERKEERSLICHT.cpp
        Specificatie_1/Elementen/VERKEERSLICHT.h
        Specificatie_1/Elementen/VOERTUIG.cpp
        Specificatie_1/Elementen/VOERTUIG.h
        Specificatie_1/Elementen/VOERTUIGGENERATOR.cpp
        Specificatie_1/Elementen/VOERTUIGGENERATOR.h)
# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})
# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})
# Link library
target_link_libraries(debug_target gtest)