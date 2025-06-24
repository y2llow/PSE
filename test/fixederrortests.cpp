#include <fstream>
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <vector>
#include <set>

#include "../src/simulatie/parsing/UniversalParser.h"
#include "../src/simulatie/Simulator.h"
#include "../src/logs_and_errors/Logger.h"
#include "../src/logs_and_errors/ErrorOutput.h"

class FixedErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove("test_errors.txt");
    }

    void TearDown() override {
        std::filesystem::remove("test_errors.txt");

        // Cleanup temporary files
        for (const auto& file : temp_files) {
            std::filesystem::remove(file);
        }
    }

    void createTestXML(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        file << content;
        file.close();
        temp_files.push_back(filename);
    }

    std::vector<std::string> getErrorMessages(const std::string& filename) {
        std::vector<std::string> messages;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            size_t errorPos = line.find("[ERROR]");
            if (errorPos != std::string::npos) {
                std::string message = line.substr(errorPos + 8);
                // Trim whitespace
                message.erase(0, message.find_first_not_of(" \t"));
                message.erase(message.find_last_not_of(" \t") + 1);
                messages.push_back(message);
            }
        }
        return messages;
    }

    // Helper om specifieke error patterns te tellen (exclusief "overgeslagen" messages)
    int countSpecificErrorPattern(const std::vector<std::string>& messages, const std::string& pattern) {
        int count = 0;
        for (const auto& message : messages) {
            // Skip "overgeslagen" messages
            if (message.find("wordt overgeslagen") != std::string::npos) {
                continue;
            }
            if (message.find(pattern) != std::string::npos) {
                count++;
            }
        }
        return count;
    }

    // Helper om "overgeslagen" messages te tellen
    int countSkippedMessages(const std::vector<std::string>& messages) {
        int count = 0;
        for (const auto& message : messages) {
            if (message.find("wordt overgeslagen") != std::string::npos) {
                count++;
            }
        }
        return count;
    }

    // Helper om te controleren of we exact de verwachte pattern + skip combinatie hebben
    bool hasExpectedErrorPattern(const std::vector<std::string>& messages,
                                 const std::string& specificError,
                                 int expectedSpecificCount,
                                 int expectedSkipCount) {
        int specificCount = countSpecificErrorPattern(messages, specificError);
        int skipCount = countSkippedMessages(messages);

        return (specificCount == expectedSpecificCount && skipCount == expectedSkipCount);
    }

    std::vector<std::string> runParserAndGetErrors(const std::string& xmlFile) {
        Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
        ErrorOutput errorOutput(logger);

        UniversalParser::initialize();
        auto sim = std::make_unique<Simulator>(errorOutput);
        UniversalParser::parseElements(xmlFile, sim.get(), errorOutput);

        logger.flush();
        return getErrorMessages("test_errors.txt");
    }

private:
    std::vector<std::string> temp_files;
};

// =============== TESTS MET CORRECTE VERWACHTINGEN ===============

TEST_F(FixedErrorTest, ExactlyOneNegativePositionError) {
    createTestXML("single_negative.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-10</positie>
        <type>auto</type>
    </VOERTUIG>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>20</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("single_negative.xml");

    // Verwacht: 1 specifieke error + 1 "overgeslagen" message = totaal 2 errors
    EXPECT_TRUE(hasExpectedErrorPattern(errors, "positie moet positief zijn", 1, 1))
        << "Moet exact 1 negatieve positie error + 1 skip message hebben";
    int size = errors.size();
    EXPECT_EQ(size, 2) << "Totaal moet 2 errors zijn (1 specifiek + 1 skip)";
}

TEST_F(FixedErrorTest, ExactlyThreeNegativePositionErrors) {
    createTestXML("triple_negative.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-5</positie>
        <type>auto</type>
    </VOERTUIG>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-15</positie>
        <type>bus</type>
    </VOERTUIG>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-1</positie>
        <type>auto</type>
    </VOERTUIG>


</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("triple_negative.xml");

    // Verwacht: 3 specifieke errors + 3 "overgeslagen" messages = totaal 6 errors
    EXPECT_TRUE(hasExpectedErrorPattern(errors, "positie moet positief zijn", 3, 3))
        << "Moet exact 3 negatieve positie errors + 3 skip messages hebben";
    int size = errors.size();
    EXPECT_EQ(size, 6) << "Totaal moet 6 errors zijn (3 specifiek + 3 skip)";
}

TEST_F(FixedErrorTest, ExactlyOneMissingNameError) {
    createTestXML("missing_name.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <lengte>100</lengte>
    </BAAN>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("missing_name.xml");

    // Voor missing naam krijgen we alleen "overgeslagen" message, geen specifieke "baan zonder naam"
    // Dus verwacht: 0 specifieke "baan zonder naam" + 1 "overgeslagen" = totaal 1
    int skipCount = countSkippedMessages(errors);
    EXPECT_EQ(skipCount, 1) << "Moet exact 1 skip message hebben";
    int size = errors.size();
    EXPECT_EQ(size, 1) << "Totaal moet 1 error zijn";
}

TEST_F(FixedErrorTest, ExactlyOneInvalidLengthError) {
    createTestXML("invalid_length.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>honderd</lengte>
    </BAAN>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("invalid_length.xml");

    // Verwacht: 1 specifieke "geen integer" error + 1 "overgeslagen" message = totaal 2
    EXPECT_TRUE(hasExpectedErrorPattern(errors, "geen integer", 1, 1))
        << "Moet exact 1 invalid integer error + 1 skip message hebben";
    int size = errors.size();
    EXPECT_EQ(size, 2) << "Totaal moet 2 errors zijn";
}

TEST_F(FixedErrorTest, ExactlyOneOutOfBoundsError) {
    createTestXML("out_of_bounds.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>50</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>75</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("out_of_bounds.xml");

    // Voor bounds error krijgen we alleen de specifieke error, geen "overgeslagen" message
    // omdat het voertuig direct wordt weggegooid
    int boundsErrors = countSpecificErrorPattern(errors, "buiten baan grenzen");
    EXPECT_EQ(boundsErrors, 1) << "Moet exact 1 bounds error hebben";
    int size = errors.size();
    EXPECT_EQ(size, 1) << "Totaal moet 1 error zijn";
}

TEST_F(FixedErrorTest, ComplexCombinationError) {
    createTestXML("complex_errors.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>abc</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-5</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("complex_errors.xml");

    /*
    Verwachte output:
    1. "De lengte van een baan is geen integer: abc"
    2. "Baan wordt overgeslagen vanwege ongeldige eigenschappen"
    3. "Baan niet gevonden voor voertuig: TestBaan"
    4. "Voertuig wordt overgeslagen vanwege ongeldige eigenschappen"
    */

    int integerErrors = countSpecificErrorPattern(errors, "geen integer");
    int notFoundErrors = countSpecificErrorPattern(errors, "Baan niet gevonden");
    int skipMessages = countSkippedMessages(errors);

    EXPECT_EQ(integerErrors, 1) << "Moet exact 1 invalid integer error hebben";
    EXPECT_EQ(notFoundErrors, 1) << "Moet exact 1 'baan niet gevonden' error hebben";
    EXPECT_EQ(skipMessages, 2) << "Moet exact 2 skip messages hebben";
    int size = errors.size();
    EXPECT_EQ(size, 4) << "Totaal moet 4 errors zijn";
}

// =============== POSITIVE TESTS (geen errors verwacht) ===============

TEST_F(FixedErrorTest, NoErrorsForValidInput) {
    createTestXML("valid_input.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>50</positie>
        <type>auto</type>
    </VOERTUIG>
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>75</positie>
        <cyclus>30</cyclus>
    </VERKEERSLICHT>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("valid_input.xml");

    // Geldige input mag GEEN errors produceren
    int size = errors.size();
    EXPECT_EQ(size, 0) << "Geldige input mag geen errors produceren. Gevonden errors:";

    // Print alle gevonden errors voor debugging
    for (size_t i = 0; i < errors.size(); ++i) {
        std::cout << "Error " << i << ": " << errors[i] << std::endl;
    }
}

// =============== SPECIFIC ERROR TYPE TESTS ===============

TEST_F(FixedErrorTest, OnlySpecificErrorType_NegativePosition) {
    createTestXML("only_negative.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-10</positie>
        <type>auto</type>
    </VOERTUIG>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-10</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("only_negative.xml");

    // Test dat ALLEEN negatieve positie errors voorkomen
    int negativeErrors = countSpecificErrorPattern(errors, "positie moet positief zijn");
    int integerErrors = countSpecificErrorPattern(errors, "geen integer");
    int doubleErrors = countSpecificErrorPattern(errors, "geen double");
    int boundsErrors = countSpecificErrorPattern(errors, "buiten baan grenzen");

    EXPECT_GT(negativeErrors, 0) << "Moet meer als 0 negatieve positie error hebben";
    EXPECT_EQ(integerErrors, 0) << "Mag geen integer errors hebben";
    EXPECT_EQ(doubleErrors, 0) << "Mag geen double errors hebben";
    EXPECT_EQ(boundsErrors, 0) << "Mag geen bounds errors hebben";
}

TEST_F(FixedErrorTest, OnlySpecificErrorType_InvalidInteger) {
    createTestXML("only_invalid_int.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>honderd</lengte>
    </BAAN>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("only_invalid_int.xml");

    // Test dat ALLEEN invalid integer errors voorkomen
    int integerErrors = countSpecificErrorPattern(errors, "geen integer");
    int negativeErrors = countSpecificErrorPattern(errors, "positie moet positief zijn");
    int doubleErrors = countSpecificErrorPattern(errors, "geen double");

    EXPECT_GT(integerErrors, 0) << "Moet meer als 0 negatieve positie error hebben";
    EXPECT_EQ(negativeErrors, 0) << "Mag geen negatieve positie errors hebben";
    EXPECT_EQ(doubleErrors, 0) << "Mag geen double errors hebben";
}

