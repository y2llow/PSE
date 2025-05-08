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
    Parser::initialize();
    Parser::parseElements("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml", sim);

    EXPECT_EQ(int(sim->getBanen().size()), 1); // Moet 1 zijn anders zal onze assertion failen

    for (auto &v : sim->getBanen()){
        EXPECT_EQ(int(v->getVerkeerslichten().size()), 0);
        EXPECT_EQ(int(v->getVoertuigeneratoren().size()), 0);
        EXPECT_EQ(int(v->getKruispunten().size()), 0);
        EXPECT_EQ(int(v->getBushaltes().size()), 0);
    }
}

TEST_F(SimulationTestingParser, DifferentOrderOfElements) {
    Parser::initialize();
    // Run parser and log relevant output to a file
    std::ofstream actual_file("../outputs/output_different_order.txt");

    Parser::parseElements("../test/specificatie1/1_1_verkeerssituatie_inlezen/verschillende_volgorde_van_elementen.xml", sim);

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

//TEST_F(SimulationTestingParser, InvalidXMLFileTest) {
//    Parser::initialize();
//    std::ofstream actual_file("outputs/output_invalid_xml.txt");
//
//    testing::internal::CaptureStderr();
//    EXPECT_FALSE(Parser::parseElements("xml/parserTests/empty_file.xml", sim));
//    std::string output1 = testing::internal::GetCapturedStderr();
//
//    actual_file << output1;
//
//    testing::internal::CaptureStderr();
//    EXPECT_FALSE(Parser::parseElements("xml/parserTests/malformed.xml", sim));
//    std::string output2 = testing::internal::GetCapturedStderr();
//
//    actual_file << output2;
//    actual_file.close();
//
//    std::ifstream actual("outputs/output_invalid_xml.txt");
//    std::ifstream expected("outputs/expected_invalid_xml.txt");
//
//    std::stringstream actual_ss, expected_ss;
//    actual_ss << actual.rdbuf();
//    expected_ss << expected.rdbuf();
//
//    EXPECT_EQ(actual_ss.str(), expected_ss.str());
//}

//TEST_F(SimulationTestingParser, CombinedErrorTests) {
//    Parser::initialize();
//    std::ofstream actual_file("outputs/output_combined_errors.txt");
//
//    testing::internal::CaptureStderr();
//    Parser::parseElements("xml/parserTests/gecombineerde_foute.xml", sim);
//    std::string output = testing::internal::GetCapturedStderr();
//
//    actual_file << output;
//    actual_file.close();
//
//    std::ifstream actual("outputs/output_combined_errors.txt");
//    std::ifstream expected("outputs/expected_combined_errors.txt");
//
//    std::stringstream actual_ss, expected_ss;
//    actual_ss << actual.rdbuf();
//    expected_ss << expected.rdbuf();
//
//    EXPECT_EQ(actual_ss.str(), expected_ss.str());
//}

//TEST(CompareOutput, MatchExpectedErrors) {
//    Parser::initialize();
//    std::ifstream actual_file("outputs/test_output.txt");
//    std::ifstream expected_file("outputs/expected_errors.txt");
//
//    std::stringstream actual_stream, expected_stream;
//    actual_stream << actual_file.rdbuf();
//    expected_stream << expected_file.rdbuf();
//
//    std::string actual = actual_stream.str();
//    std::string expected = expected_stream.str();
//
//    EXPECT_EQ(actual, expected);
//}

