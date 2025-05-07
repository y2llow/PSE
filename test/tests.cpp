#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include "../src/DesignByContract.h"

int main(int argc, char **argv) {
    std::ofstream error_log("outputs/expected_invalids_xml.txt");
    std::streambuf* old_cerr = std::cerr.rdbuf(error_log.rdbuf());

    // Schakel contractcontroles uit voor testen
    g_enforceContracts = false;

    // Initialiseer de Google Test omgeving
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    std::cerr.rdbuf(old_cerr);

    return result;
}