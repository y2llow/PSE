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

# Don't use local SFML files
# Remove or comment out these:
 set(SFML_STATIC_LIBRARIES TRUE)
 set(SFML_DIR "${CMAKE_SOURCE_DIR}/utils/sfml-files/lib/cmake/SFML")

# Use system SFML
find_package(SFML 2.5 REQUIRED graphics window system)

# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES
        ./src/TinyXML/tinystr.cpp ./src/TinyXML/tinyxml.cpp ./src/TinyXML/tinyxmlerror.cpp ./src/TinyXML/tinyxmlparser.cpp
        src/elementen/Verkeerslicht.cpp
        src/elementen/Verkeerslicht.h
        src/elementen/Baan.cpp
        src/elementen/Voertuig.cpp
        src/elementen/Voertuiggenerator.cpp
        src/simulatie/Simulator.cpp
        main.cpp
        src/elementen/Constants.h
        src/simulatie/Parser.cpp
        src/simulatie/Parser.h
        src/elementen/Bushalte.cpp
        src/elementen/Bushalte.h
        src/elementen/voertuigen/Auto.cpp
        src/elementen/voertuigen/Auto.h
        src/elementen/voertuigen/Brandweerwagen.cpp
        src/elementen/voertuigen/Brandweerwagen.h
        src/elementen/voertuigen/Bus.cpp
        src/elementen/voertuigen/Bus.h
        src/elementen/voertuigen/Politiecombi.cpp
        src/elementen/voertuigen/Politiecombi.h
        src/elementen/voertuigen/Ziekenwagen.cpp
        src/elementen/voertuigen/Ziekenwagen.h
        utils/Game.cpp
        utils/Game.h
        src/DesignByContract.cpp

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
        src/simulatie/Simulator.cpp
        src/elementen/Constants.h
        src/simulatie/Parser.cpp
        src/simulatie/Parser.h
        src/elementen/Bushalte.cpp
        src/elementen/Bushalte.h
        src/elementen/voertuigen/Auto.cpp
        src/elementen/voertuigen/Auto.h
        src/elementen/voertuigen/Brandweerwagen.cpp
        src/elementen/voertuigen/Brandweerwagen.h
        src/elementen/voertuigen/Bus.cpp
        src/elementen/voertuigen/Bus.h
        src/elementen/voertuigen/Politiecombi.cpp
        src/elementen/voertuigen/Politiecombi.h
        src/elementen/voertuigen/Ziekenwagen.cpp
        src/elementen/voertuigen/Ziekenwagen.h
        test/VoertuigTest.cpp
        test/ParserTest.cpp
        test/tests.cpp
        test/VerkeerslichtTest.cpp
        test/VoertuiggeneratorTest.cpp
        test/BushalteTest.cpp
        src/DesignByContract.cpp
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


target_link_libraries(release_target sfml-graphics sfml-window sfml-system)