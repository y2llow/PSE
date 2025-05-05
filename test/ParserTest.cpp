#include <cmath>
#include <fstream>
#include <gtest/gtest.h>

#include "../../src/simulatie/Simulator.h"
#include "../../src/simulatie/Parser.h"

class SimulationTestingParser : public testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
    Simulator* sim = new Simulator();
};

TEST_F(SimulationTestingParser, EmptyXMLFileTest) {
    Parser::parseElements("specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml", sim);

    EXPECT_EQ(int(sim->getBanen().size()), 1); // Moet 1 zijn anders zal onze assertion failen
    for (auto &v : sim->getBanen()){
        EXPECT_EQ(int(v->getVerkeerslichten().size()), 0);
        EXPECT_EQ(int(v->getVoertuigeneratoren().size()), 0);
        EXPECT_EQ(int(v->getKruispunten().size()), 0);
        EXPECT_EQ(int(v->getBushaltes().size()), 0);
    }
}

TEST_F(SimulationTestingParser, DifferentOrderOfElements) {
    Parser::parseElements("specificatie1/1_1_verkeerssituatie_inlezen/verschillende_volgorde_van_elementen.xml", sim);

    EXPECT_EQ(int(sim->getBanen().size()), 2);

    Baan* baan = sim->getBanen()[0];
        EXPECT_EQ(int(baan->getVoertuigen().size()), 2);
        EXPECT_EQ(int(baan->getVoertuigeneratoren().size()), 1);
        EXPECT_EQ(int(baan->getVerkeerslichten().size()), 1);
        EXPECT_EQ(int(baan->getBushaltes().size()), 1);
        EXPECT_EQ(int(baan->getKruispunten().size()), 1);

}

TEST_F(SimulationTestingParser, InvalidXMLFileTest) {
    testing::internal::CaptureStderr();

    // Test empty file
    EXPECT_FALSE(Parser::parseElements("xml/parserTests/empty_file.xml", sim));
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(output.find("Error loading file: Error document empty") != std::string::npos)
                        << "Should detect empty XML file";

    testing::internal::CaptureStderr();

    // Test malformed XML
    EXPECT_FALSE(Parser::parseElements("xml/parserTests/malformed.xml", sim));
    output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(output.find("Error loading file: Error reading end tag") != std::string::npos)
                        << "Should detect malformed XML with tag errors";
}

TEST_F(SimulationTestingParser, CombinedErrorTests) {
    testing::internal::CaptureStderr();

    // Parse het gecombineerde testbestand
    bool result = Parser::parseElements("xml/parserTests/gecombineerde_foute.xml", sim);
    std::string output = testing::internal::GetCapturedStderr();

    // Controleer alle verwachte foutmeldingen
    EXPECT_TRUE(result) << "Parsing should fail for invalid file";

    // Lijst van alle verwachte foutmeldingen
    std::vector<std::string> expected_errors = {
            "Er is een baan zonder naam!",
            "De lengte van een baan is geen integer!",
            "De snelheid van een voertuig is geen double!",
            "Er is een verkeerslicht waarvan de cyclus geen integer is!",
            "Er is een bushalte waarvan de wachttijd geen integer is!",
            "Er is een baan met een ongeldige lengte of naam!",
    };

    // Controleer of elke foutmelding voorkomt in de output
    for (const auto& error : expected_errors) {
        EXPECT_TRUE(output.find(error) != std::string::npos)
                            << "Expected error not found: " << error;
    }
}

