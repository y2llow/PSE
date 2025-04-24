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
set(RELEASE_SOURCE_FILES
        ./src/TinyXML/tinystr.cpp ./src/TinyXML/tinyxml.cpp ./src/TinyXML/tinyxmlerror.cpp ./src/TinyXML/tinyxmlparser.cpp
        src/elementen/Verkeerslicht.cpp
        src/elementen/Baan.cpp
        src/elementen/Voertuig.cpp
        src/elementen/Voertuiggenerator.cpp
        src/simulatie/simulation.cpp
        main.cpp
        src/elementen/Constants.h
        src/simulatie/Parser.cpp
        src/simulatie/Parser.h
        src/elementen/TypeVoertuig.cpp
        src/elementen/TypeVoertuig.h
        src/simulatie/SimPrinter.cpp    # Zorg ervoor dat deze is toegevoegd
        src/elementen/Bushalte.cpp
        src/elementen/Bushalte.h
)

# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES
        ./src/TinyXML/tinystr.cpp ./src/TinyXML/tinyxml.cpp ./src/TinyXML/tinyxmlerror.cpp ./src/TinyXML/tinyxmlparser.cpp
        src/elementen/Verkeerslicht.cpp
        src/elementen/Baan.cpp
        src/elementen/Voertuig.cpp
        src/elementen/Voertuiggenerator.cpp
        test/specificatie1/1_1_verkeerssituatie_inlezen/SimulationTESTS.cpp
        src/simulatie/simulation.cpp
        src/elementen/Constants.h
        src/simulatie/SimPrinter.cpp
        src/simulatie/SimPrinter.h
        src/simulatie/Parser.cpp
        src/simulatie/Parser.h
        src/elementen/TypeVoertuig.cpp
        src/elementen/TypeVoertuig.h
        src/elementen/Bushalte.cpp
        src/elementen/Bushalte.h
)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})
# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})
# Link library
target_link_libraries(debug_target gtest)

# Add the test command
add_test(
        NAME MyTest
        COMMAND debug_target
)