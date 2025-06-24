// #include <fstream>
// #include <gtest/gtest.h>
// #include <filesystem>
// #include <string>
// #include <vector>
// #include <set>
//
// #include "../src/simulatie/parsing/UniversalParser.h"
// #include "../src/simulatie/Simulator.h"
// #include "../src/logs_and_errors/Logger.h"
// #include "../src/logs_and_errors/ErrorOutput.h"
//
// class ImprovedErrorTest : public ::testing::Test {
// protected:
//     void SetUp() override {
//         std::filesystem::remove("test_errors.txt");
//     }
//
//     void TearDown() override {
//         std::filesystem::remove("test_errors.txt");
//
//         // Cleanup temporary files
//         for (const auto& file : temp_files) {
//             std::filesystem::remove(file);
//         }
//     }
//
//     // Helper om XML files te maken
//     void createTestXML(const std::string& filename, const std::string& content) {
//         std::ofstream file(filename);
//         file << content;
//         file.close();
//         temp_files.push_back(filename);
//     }
//
//     // Helper om error messages uit file te halen
//     std::vector<std::string> getErrorMessages(const std::string& filename) {
//         std::vector<std::string> messages;
//         std::ifstream file(filename);
//         std::string line;
//
//         while (std::getline(file, line)) {
//             if (line.empty()) continue;
//
//             size_t errorPos = line.find("[ERROR]");
//             if (errorPos != std::string::npos) {
//                 std::string message = line.substr(errorPos + 8);
//                 // Trim whitespace
//                 message.erase(0, message.find_first_not_of(" \t"));
//                 message.erase(message.find_last_not_of(" \t") + 1);
//                 messages.push_back(message);
//             }
//         }
//         return messages;
//     }
//
//     // Helper om te checken of exact de verwachte errors aanwezig zijn
//     bool hasExactErrors(const std::vector<std::string>& actualMessages,
//                        const std::vector<std::string>& expectedErrors) {
//         if (actualMessages.size() != expectedErrors.size()) {
//             return false;
//         }
//
//         // Check of elke verwachte error exact 1 keer voorkomt
//         for (const auto& expected : expectedErrors) {
//             int count = 0;
//             for (const auto& actual : actualMessages) {
//                 if (actual.find(expected) != std::string::npos) {
//                     count++;
//                 }
//             }
//             if (count != 1) {
//                 return false;
//             }
//         }
//         return true;
//     }
//
//     // Helper om te checken of specifieke error pattern voorkomt
//     int countErrorPattern(const std::vector<std::string>& messages, const std::string& pattern) {
//         int count = 0;
//         for (const auto& message : messages) {
//             if (message.find(pattern) != std::string::npos) {
//                 count++;
//             }
//         }
//         return count;
//     }
//
//     // Helper om parser te draaien en errors te verzamelen
//     std::vector<std::string> runParserAndGetErrors(const std::string& xmlFile) {
//         Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
//         ErrorOutput errorOutput(logger);
//
//         UniversalParser::initialize();
//         auto sim = std::make_unique<Simulator>(errorOutput);
//         UniversalParser::parseElements(xmlFile, sim.get(), errorOutput);
//
//         logger.flush();
//         return getErrorMessages("test_errors.txt");
//     }
//
// private:
//     std::vector<std::string> temp_files;
// };
//
// // =============== EXACTE ERROR COUNT TESTS ===============
//
// TEST_F(ImprovedErrorTest, ExactlyOneNegativePositionError) {
//     createTestXML("single_negative.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
// <VERKEERSSITUATIE>
//     <BAAN>
//         <naam>TestBaan</naam>
//         <lengte>100</lengte>
//     </BAAN>
//     <VOERTUIG>
//         <baan>TestBaan</baan>
//         <positie>-10</positie>
//         <type>auto</type>
//     </VOERTUIG>
// </VERKEERSSITUATIE>)");
//
//     auto errors = runParserAndGetErrors("single_negative.xml");
//
//     // Exacte check: moet precies 1 error zijn over negatieve positie
//     int negativeErrors = countErrorPattern(errors, "positie moet positief zijn");
//     EXPECT_EQ(negativeErrors, 1) << "Moet exact 1 negatieve positie error hebben";
//
//     // Mag geen andere soorten errors hebben
//     int otherErrors = errors.size() - negativeErrors;
//     EXPECT_EQ(otherErrors, 0) << "Mag geen andere errors hebben, maar vond " << otherErrors;
//
//     // [2025-06-24 10:13:56] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig positie moet positief zijn: -10
//     // [2025-06-24 10:13:56] [ERROR] [XMLParserStrategy::parseVoertuigen]FOUT:Voertuig wordt overgeslagen vanwege ongeldige eigenschappen
// }
//
// TEST_F(ImprovedErrorTest, ExactlyThreeNegativePositionErrors) {
//     createTestXML("triple_negative.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
// <VERKEERSSITUATIE>
//     <BAAN>
//         <naam>TestBaan</naam>
//         <lengte>100</lengte>
//     </BAAN>
//     <VOERTUIG>
//         <baan>TestBaan</baan>
//         <positie>-5</positie>
//         <type>auto</type>
//     </VOERTUIG>
//     <VOERTUIG>
//         <baan>TestBaan</baan>
//         <positie>-15</positie>
//         <type>bus</type>
//     </VOERTUIG>
//     <VOERTUIG>
//         <baan>TestBaan</baan>
//         <positie>-1</positie>
//         <type>auto</type>
//     </VOERTUIG>
// </VERKEERSSITUATIE>)");
//
//     auto errors = runParserAndGetErrors("triple_negative.xml");
//
//     // Moet exact 3 negatieve positie errors zijn
//     int negativeErrors = countErrorPattern(errors, "positie moet positief zijn");
//     EXPECT_EQ(negativeErrors, 3) << "Moet exact 3 negatieve positie errors hebben";
//
//     // Totaal aantal errors moet ook 3 zijn (geen extra errors)
//     int size = errors.size();
//     EXPECT_EQ(size, 3) << "Totaal moet exact 3 errors zijn";
//     /*
//     [2025-06-24 10:15:31] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig positie moet positief zijn: -5
//     [2025-06-24 10:15:31] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig wordt overgeslagen vanwege ongeldige eigenschappen
//     [2025-06-24 10:15:31] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig positie moet positief zijn: -15
//     [2025-06-24 10:15:31] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig wordt overgeslagen vanwege ongeldige eigenschappen
//     [2025-06-24 10:15:31] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig positie moet positief zijn: -1
//     [2025-06-24 10:15:31] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig wordt overgeslagen vanwege ongeldige eigenschappen
//     */
// }
//
// TEST_F(ImprovedErrorTest, ExactlyOneMissingNameError) {
//     createTestXML("missing_name.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
// <VERKEERSSITUATIE>
//     <BAAN>
//         <lengte>100</lengte>
//     </BAAN>
// </VERKEERSSITUATIE>)");
//
//     auto errors = runParserAndGetErrors("missing_name.xml");
//
//     // Moet exact 1 missing name error zijn
//     int nameErrors = countErrorPattern(errors, "baan zonder naam");
//     EXPECT_EQ(nameErrors, 1) << "Moet exact 1 missing name error hebben";
//
//     // Totaal moet 1 zijn
//     int size = errors.size();
//     EXPECT_EQ(size, 1) << "Moet exact 1 error hebben totaal";
//     // [2025-06-24 10:16:27] [ERROR] [XMLParserStrategy::parseBanen] FOUT: Baan wordt overgeslagen vanwege ongeldige eigenschappen
//
// }
//
// TEST_F(ImprovedErrorTest, ExactlyOneInvalidLengthError) {
//     createTestXML("invalid_length.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
// <VERKEERSSITUATIE>
//     <BAAN>
//         <naam>TestBaan</naam>
//         <lengte>honderd</lengte>
//     </BAAN>
// </VERKEERSSITUATIE>)");
//
//     auto errors = runParserAndGetErrors("invalid_length.xml");
//
//     // Moet exact 1 invalid integer error zijn
//     int lengthErrors = countErrorPattern(errors, "geen integer");
//     EXPECT_EQ(lengthErrors, 1) << "Moet exact 1 invalid integer error hebben";
//     int size = errors.size();
//     EXPECT_EQ(size, 1) << "Moet exact 1 error hebben totaal";
//     // [2025-06-24 10:16:48] [ERROR] [XMLParserStrategy::parseBanen] FOUT: De lengte van een baan is geen integer: honderd
//     // [2025-06-24 10:16:48] [ERROR] [XMLParserStrategy::parseBanen] FOUT: Baan wordt overgeslagen vanwege ongeldige eigenschappen
// }
//
// TEST_F(ImprovedErrorTest, ExactlyOneOutOfBoundsError) {
//     createTestXML("out_of_bounds.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
// <VERKEERSSITUATIE>
//     <BAAN>
//         <naam>TestBaan</naam>
//         <lengte>50</lengte>
//     </BAAN>
//     <VOERTUIG>
//         <baan>TestBaan</baan>
//         <positie>75</positie>
//         <type>auto</type>
//     </VOERTUIG>
// </VERKEERSSITUATIE>)");
//
//     auto errors = runParserAndGetErrors("out_of_bounds.xml");
//
//     // Moet exact 1 bounds error zijn
//     int boundsErrors = countErrorPattern(errors, "buiten baan grenzen");
//     EXPECT_EQ(boundsErrors, 1) << "Moet exact 1 bounds error hebben";
//     int size = errors.size();
//     EXPECT_EQ(size, 1) << "Moet exact 1 error hebben totaal";
//     // [2025-06-24 10:17:21] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig positie buiten baan grenzen: 75 >= 50
//
// }
//
// // =============== COMBINATIE ERROR TESTS ===============
//
// TEST_F(ImprovedErrorTest, ExactCombinationOfTwoErrorTypes) {
//     createTestXML("two_error_types.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
// <VERKEERSSITUATIE>
//     <BAAN>
//         <naam>TestBaan</naam>
//         <lengte>abc</lengte>
//     </BAAN>
//     <VOERTUIG>
//         <baan>TestBaan</baan>
//         <positie>-5</positie>
//         <type>auto</type>
//     </VOERTUIG>
// </VERKEERSSITUATIE>)");
//
//     auto errors = runParserAndGetErrors("two_error_types.xml");
//
//     // Moet exact 1 van elke type error hebben
//     int lengthErrors = countErrorPattern(errors, "geen integer");
//     int positionErrors = countErrorPattern(errors, "positie moet positief zijn");
//
//     EXPECT_EQ(lengthErrors, 1) << "Moet exact 1 invalid length error hebben";
//     EXPECT_EQ(positionErrors, 1) << "Moet exact 1 negatieve positie error hebben";
//
//     int size = errors.size();
//     // Totaal moet 2 zijn
//     EXPECT_EQ(size, 2) << "Moet exact 2 errors hebben totaal";
//     /*
// [2025-06-24 10:17:48] [ERROR] [XMLParserStrategy::parseBanen] FOUT: De lengte van een baan is geen integer: abc
// [2025-06-24 10:17:48] [ERROR] [XMLParserStrategy::parseBanen] FOUT: Baan wordt overgeslagen vanwege ongeldige eigenschappen
// [2025-06-24 10:17:48] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Baan niet gevonden voor voertuig: TestBaan
// [2025-06-24 10:17:48] [ERROR] [XMLParserStrategy::parseVoertuigen] FOUT: Voertuig wordt overgeslagen vanwege ongeldige eigenschappen
//      */
// }
