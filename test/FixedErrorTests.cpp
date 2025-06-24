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
    bool hasExpectedErrorPattern(const std::vector<std::string>& messages,const std::string& specificError,int expectedSpecificCount,int expectedSkipCount) {
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

    bool containsOnlySpecificErrorType(const std::vector<std::string>& errors,const std::vector<std::string>& allowedPatterns,const std::vector<std::string>& alwaysAllowedPatterns = {"wordt overgeslagen"}) {

        for (const auto& error : errors) {
            bool isAllowed = false;

            // Check always allowed patterns first (zoals "overgeslagen")
            for (const auto& alwaysAllowed : alwaysAllowedPatterns) {
                if (error.find(alwaysAllowed) != std::string::npos) {
                    isAllowed = true;
                    break;
                }
            }

            // Als nog niet toegestaan, check specifieke patterns
            if (!isAllowed) {
                for (const auto& pattern : allowedPatterns) {
                    if (error.find(pattern) != std::string::npos) {
                        isAllowed = true;
                        break;
                    }
                }
            }

            if (!isAllowed) {
                std::cout << "Onverwachte error gevonden: " << error << std::endl;
                return false;
            }
        }

        return true;
    }

    bool hasAtLeastOneError(const std::vector<std::string>& errors, const std::string& expectedPattern) {
        for (const auto& error : errors) {
            if (error.find(expectedPattern) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    void debugPrintErrors(const std::vector<std::string>& errors, const std::string& testName) {
        std::cout << "\n=== DEBUG ERRORS VOOR " << testName << " ===" << std::endl;
        if (errors.empty()) {
            std::cout << "Geen errors gevonden." << std::endl;
        } else {
            for (size_t i = 0; i < errors.size(); ++i) {
                std::cout << i << ": " << errors[i] << std::endl;
            }
        }
        std::cout << "Totaal: " << errors.size() << " errors" << std::endl;
        std::cout << "==========================================\n" << std::endl;
    }

    void testOnlySpecificErrorType(const std::string& xmlContent,const std::vector<std::string>& allowedPatterns,const std::string& testName,int expectedMinCount = 1) {

        std::string filename = testName + ".xml";
        createTestXML(filename, xmlContent);

        auto errors = runParserAndGetErrors(filename);

        // Debug output indien gewenst
        if (errors.size() > 10) { // Alleen bij veel errors
            debugPrintErrors(errors, testName);
        }

        // Check dat alleen toegestane errors voorkomen
        EXPECT_TRUE(containsOnlySpecificErrorType(errors, allowedPatterns))
            << "Test " << testName << ": Alleen specifieke error types toegestaan";

        // Check dat we minstens het verwachte aantal errors hebben
        int totalAllowedErrors = 0;
        for (const auto& pattern : allowedPatterns) {
            totalAllowedErrors += countSpecificErrorPattern(errors, pattern);
        }

        EXPECT_GE(totalAllowedErrors, expectedMinCount)
            << "Test " << testName << ": Moet minstens " << expectedMinCount << " errors hebben";
    }

private:
    std::vector<std::string> temp_files;
};

// =============== TESTS MET CORRECTE VERWACHTINGEN ===============

TEST_F(FixedErrorTest, ExactlyOneNegativeVoertuigPositionError) {
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

TEST_F(FixedErrorTest, ExactlyOneMissingNameError) {
    createTestXML("missing_name.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <lengte>100</lengte>
    </BAAN>
    <BAAN>
        <naam>TestBaan2</naam>
        <lengte>100</lengte>
    </BAAN>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("missing_name.xml");

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
    <BAAN>
        <naam>TestBaan2</naam>
        <lengte>100</lengte>
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
        <positie>10</positie>
        <type>auto</type>
    </VOERTUIG>
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>1000</positie>
        <cyclus>2</cyclus>
    </VERKEERSLICHT>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>0</positie>
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

// =============== SPECIFIC ERROR TYPE TESTS ===============

TEST_F(FixedErrorTest, OnlySpecificErrorType_NegativePositions) {
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
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>-10</positie>
        <cyclus>100</cyclus>
    </VERKEERSLICHT>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>-5</positie>
        <type>auto</type>
    </VOERTUIG>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>5</positie>
        <type>auto</type>
    </VOERTUIG>
    <BAAN>
        <naam>TestBaan2</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan2</baan>
        <positie>5</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("only_negative.xml");

    // Check dat ALLE errors over negatieve positie gaan (of "overgeslagen" zijn)
    bool onlyNegativePositionErrors = true;
    std::vector<std::string> unexpectedErrors;

    for (const auto& error : errors) {
        // Skip "overgeslagen" messages - die zijn verwacht als gevolg van andere errors
        if (error.find("wordt overgeslagen") != std::string::npos) {
            continue;
        }

        // Check of dit een negatieve positie error is
        if (error.find("positie moet positief zijn") == std::string::npos) {
            onlyNegativePositionErrors = false;
            unexpectedErrors.push_back(error);
        }
    }

    EXPECT_TRUE(onlyNegativePositionErrors) << "Alle errors moeten over negatieve positie gaan. Onverwachte errors gevonden:";
    // Extra check: we moeten wel minstens 1 negatieve positie error hebben
    int negativeErrors = countSpecificErrorPattern(errors, "positie moet positief zijn");
    EXPECT_GT(negativeErrors, 0) << "Moet minstens 1 negatieve positie error hebben";
}

TEST_F(FixedErrorTest, OnlySpecificErrorType_InvalidIntegers) {
    createTestXML("only_invalid_int.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan1</naam>
        <lengte>honderd</lengte>
    </BAAN>
    <BAAN>
        <naam>TestBaan2</naam>
        <lengte>vijftig</lengte>
    </BAAN>
    <VERKEERSLICHT>
        <baan>TestBaan1</baan>
        <positie>huh</positie>
        <cyclus>100</cyclus>
    </VERKEERSLICHT>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("only_invalid_int.xml");

    // Check dat ALLE errors over invalid integers gaan (of "overgeslagen" zijn)
    bool onlyIntegerErrors = true;
    std::vector<std::string> unexpectedErrors;

    for (const auto& error : errors) {
        // Skip "overgeslagen" messages
        if (error.find("wordt overgeslagen") != std::string::npos) {
            continue;
        }

        // Check of dit een integer error is
        if (error.find("geen integer") == std::string::npos) {
            onlyIntegerErrors = false;
            unexpectedErrors.push_back(error);
        }
    }

    EXPECT_TRUE(onlyIntegerErrors)
        << "Alle errors moeten over invalid integers gaan. Onverwachte errors gevonden:";

    // Print onverwachte errors voor debugging
    for (const auto& unexpected : unexpectedErrors) {
        std::cout << "Onverwacht: " << unexpected << std::endl;
    }

    // Extra check: we moeten wel minstens 1 integer error hebben
    int integerErrors = countSpecificErrorPattern(errors, "geen integer");
    EXPECT_GT(integerErrors, 0) << "Moet minstens 1 integer error hebben";
}

TEST_F(FixedErrorTest, OnlySpecificErrorType_OutOfBounds) {
    createTestXML("only_bounds.xml", R"(<?xml version="1.0" encoding="UTF-8"?>
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
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>100</positie>
        <type>bus</type>
    </VOERTUIG>
    <BAAN>
        <naam>TestBaan2</naam>
        <lengte>150</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan2</baan>
        <positie>250</positie>
        <type>bus</type>
    </VOERTUIG>
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>1000</positie>
        <cyclus>100</cyclus>
    </VERKEERSLICHT>
</VERKEERSSITUATIE>)");

    auto errors = runParserAndGetErrors("only_bounds.xml");

    // Check dat ALLE errors over bounds gaan
    bool onlyBoundsErrors = true;
    std::vector<std::string> unexpectedErrors;

    for (const auto& error : errors) {
        // Check of dit een bounds error is
        if (error.find("buiten baan grenzen") == std::string::npos) {
            onlyBoundsErrors = false;
            unexpectedErrors.push_back(error);
        }
    }

    EXPECT_TRUE(onlyBoundsErrors)
        << "Alle errors moeten over bounds gaan. Onverwachte errors gevonden:";

    // Print onverwachte errors voor debugging
    for (const auto& unexpected : unexpectedErrors) {
        std::cout << "Onverwacht: " << unexpected << std::endl;
    }

    // Extra check: we moeten wel minstens 1 bounds error hebben
    int boundsErrors = countSpecificErrorPattern(errors, "buiten baan grenzen");
    EXPECT_GT(boundsErrors, 0) << "Moet minstens 1 bounds error hebben";
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