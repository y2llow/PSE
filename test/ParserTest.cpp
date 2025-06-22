#include <cmath>
#include <fstream>
#include <gtest/gtest.h>

#include "../../src/simulatie/Simulator.h"
#include "../src/simulatie/parsing/UniversalParser.h"

class SimulationTestingParser : public testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
    Simulator* sim = new Simulator();
};

TEST_F(SimulationTestingParser, EmptyXMLFileTest) {
    UniversalParser::initialize();
    UniversalParser::parseElements("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml", sim);

    EXPECT_EQ(int(sim->getBanen().size()), 1); // Moet 1 zijn anders zal onze assertion failen

    for (auto &v : sim->getBanen()){
        EXPECT_EQ(int(v->getVerkeerslichten().size()), 0);
        EXPECT_EQ(int(v->getVoertuigeneratoren().size()), 0);
        EXPECT_EQ(int(v->getKruispunten().size()), 0);
        EXPECT_EQ(int(v->getBushaltes().size()), 0);
    }
}

TEST_F(SimulationTestingParser, DifferentOrderOfElements) {
    UniversalParser::initialize();
    // Run parser and log relevant output to a file
    std::ofstream actual_file("../outputs/output_different_order.txt");

    UniversalParser::parseElements("../test/specificatie1/1_1_verkeerssituatie_inlezen/verschillende_volgorde_van_elementen.xml", sim);

    actual_file << "Aantal banen: " << sim->getBanen().size() << "\n";

    Baan* baan = sim->getBanen()[0];
    actual_file << "Voertuigen: " << baan->getVoertuigen().size() << "\n";
    actual_file << "Generators: " << baan->getVoertuigeneratoren().size() << "\n";
    actual_file << "Verkeerslichten: " << baan->getVerkeerslichten().size() << "\n";
    actual_file << "Bushaltes: " << baan->getBushaltes().size() << "\n";
    actual_file << "Kruispunten: " << baan->getKruispunten().size() << "\n";

    actual_file.close();

    // Compare to expected file
    std::ifstream actual("../outputs/output_different_order.txt");
    std::ifstream expected("../outputs/expected_different_order.txt");
    std::stringstream actual_ss, expected_ss;
    actual_ss << actual.rdbuf();
    expected_ss << expected.rdbuf();

    EXPECT_EQ(actual_ss.str(), expected_ss.str());
}


TEST_F(SimulationTestingParser, InvalidVehiclePositionsTest) {
    UniversalParser::initialize();
    // Test parser handling of vehicles with invalid positions (outside baan boundaries)
    std::ofstream actual_file("../outputs/output_invalid_positions.txt");

    UniversalParser::parseElements("../test/specificatie1/1_1_verkeerssituatie_inlezen/ongeldige_voertuig_posities.xml", sim);

    actual_file << "Aantal banen: " << sim->getBanen().size() << "\n";

    if (!sim->getBanen().empty()) {
        Baan* baan = sim->getBanen()[0];
        actual_file << "Baan lengte: " << baan->getLengte() << "\n";
        actual_file << "Geldige voertuigen: " << baan->getVoertuigen().size() << "\n";

        // Count vehicles that should have been rejected due to invalid positions
        int validVehicleCount = 0;
        for (auto& voertuig : baan->getVoertuigen()) {
            if (voertuig->getPositie() >= 0 && voertuig->getPositie() < baan->getLengte()) {
                validVehicleCount++;
            }
        }
        actual_file << "Voertuigen binnen grenzen: " << validVehicleCount << "\n";
    }

    actual_file.close();

    // Compare to expected file
    std::ifstream actual("../outputs/output_invalid_positions.txt");
    std::ifstream expected("../outputs/expected_invalid_positions.txt");
    std::stringstream actual_ss, expected_ss;
    actual_ss << actual.rdbuf();
    expected_ss << expected.rdbuf();

    EXPECT_EQ(actual_ss.str(), expected_ss.str());
}

TEST_F(SimulationTestingParser, MissingRequiredFieldsTest) {
    UniversalParser::initialize();
    // Test parser handling of elements with missing required fields
    std::ofstream actual_file("../outputs/output_missing_fields.txt");

    UniversalParser::parseElements("../test/specificatie1/1_1_verkeerssituatie_inlezen/ontbrekende_velden.xml", sim);

    actual_file << "Aantal banen: " << sim->getBanen().size() << "\n";

    // Count elements that were successfully parsed despite some having missing fields
    int totalVerkeerslichten = 0;
    int totalVoertuiggeneratoren = 0;
    int totalBushaltes = 0;
    int totalVoertuigen = 0;

    for (auto& baan : sim->getBanen()) {
        totalVerkeerslichten += baan->getVerkeerslichten().size();
        totalVoertuiggeneratoren += baan->getVoertuigeneratoren().size();
        totalBushaltes += baan->getBushaltes().size();
        totalVoertuigen += baan->getVoertuigen().size();
    }

    actual_file << "Verkeerslichten: " << totalVerkeerslichten << "\n";
    actual_file << "Voertuiggeneratoren: " << totalVoertuiggeneratoren << "\n";
    actual_file << "Bushaltes: " << totalBushaltes << "\n";
    actual_file << "Voertuigen: " << totalVoertuigen << "\n";

    actual_file.close();

    // Compare to expected file
    std::ifstream actual("../outputs/output_missing_fields.txt");
    std::ifstream expected("../outputs/expected_missing_fields.txt");
    std::stringstream actual_ss, expected_ss;
    actual_ss << actual.rdbuf();
    expected_ss << expected.rdbuf();

    EXPECT_EQ(actual_ss.str(), expected_ss.str());
}
