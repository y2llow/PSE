#include <fstream>
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <vector>

#include "../src/simulatie/Parser.h"
#include "../src/simulatie/Simulator.h"
#include "../src/logs_and_errors/Logger.h"
#include "../src/logs_and_errors/ErrorOutput.h"

class ErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Cleanup test files
        std::filesystem::remove("test_errors.txt");
        std::filesystem::remove("expected_errors.txt");
    }

    void TearDown() override {
        // Cleanup test files
        std::filesystem::remove("test_errors.txt");
        std::filesystem::remove("expected_errors.txt");
    }

    // Helper functie om file content te lezen
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "";
        }

        std::string content;
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        return content;
    }

    // Helper functie om expected error file te maken
    void createExpectedErrorFile(const std::string& filename, const std::vector<std::string>& errors) {
        std::ofstream file(filename);
        for (const auto& error : errors) {
            file << error << std::endl;
        }
        file.close();
    }

    // Helper om alleen error messages te vergelijken (zonder timestamps)
    std::vector<std::string> extractErrorMessages(const std::string& content) {
        std::vector<std::string> messages;
        std::stringstream ss(content);
        std::string line;

        while (std::getline(ss, line)) {
            if (line.empty()) continue;

            // Extract message after timestamp and [ERROR] tag
            size_t errorPos = line.find("[ERROR]");
            if (errorPos != std::string::npos) {
                std::string message = line.substr(errorPos + 8); // Skip "[ERROR] "
                messages.push_back(message);
            }
        }
        return messages;
    }
};

TEST_F(ErrorTest, BasicErrorLogging) {
    // Setup Logger
    Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
    ErrorOutput errorOutput(logger);

    // Test basic error logging
    errorOutput.logError("Test error message");
    errorOutput.handleError("Test handled error", "TestContext");

    logger.flush(); // Ensure all data is written

    // Read actual output
    std::string actualContent = readFile("test_errors.txt");
    auto actualMessages = extractErrorMessages(actualContent);

    // Expected messages
    std::vector<std::string> expectedMessages = {
            "Test error message",
            "[TestContext] FOUT: Test handled error"
    };
    int size = actualMessages.size();
    EXPECT_EQ(size, 2) << "Should have 2 error messages";
    EXPECT_EQ(actualMessages[0], expectedMessages[0]) << "First error message should match";
    EXPECT_EQ(actualMessages[1], expectedMessages[1]) << "Second error message should match";
}

TEST_F(ErrorTest, ExceptionHandling) {
    Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
    ErrorOutput errorOutput(logger);

    // Test exception handling
    try {
        throw std::runtime_error("Test exception");
    } catch (const std::exception& e) {
        errorOutput.handleException(e, "ExceptionTest");
    }

    logger.flush();

    std::string actualContent = readFile("test_errors.txt");
    auto actualMessages = extractErrorMessages(actualContent);
    int size = actualMessages.size();
    EXPECT_EQ(size, 1) << "Should have 1 exception message";
    EXPECT_TRUE(actualMessages[0].find("EXCEPTION: Test exception") != std::string::npos)
                        << "Should contain exception message";
    EXPECT_TRUE(actualMessages[0].find("ExceptionTest") != std::string::npos)
                        << "Should contain context";
}

TEST_F(ErrorTest, ParserErrorsWithInvalidXML) {
    Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
    ErrorOutput errorOutput(logger);

    // Create invalid XML file for testing
    std::ofstream xmlFile("test_invalid.xml");
    xmlFile << R"(<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>150</positie>
        <type>auto</type>
    </VOERTUIG>
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>200</positie>
        <cyclus>10</cyclus>
    </VERKEERSLICHT>
</VERKEERSSITUATIE>)";
    xmlFile.close();

    // Test parsing with errors
    Parser::initialize();
    auto sim = std::make_unique<Simulator>(errorOutput);
    logger.flush();

    std::string actualContent = readFile("test_errors.txt");
    auto actualMessages = extractErrorMessages(actualContent);

    // Should have errors for out-of-bounds positions
    EXPECT_FALSE(actualMessages.empty()) << "Should have error messages for invalid XML";

    bool foundPositionError = false;
    for (const auto& message : actualMessages) {
        if (message.find("positie buiten baan grenzen") != std::string::npos) {
            foundPositionError = true;
            break;
        }
    }
    EXPECT_TRUE(foundPositionError) << "Should contain position bounds error";

    // Cleanup
    std::filesystem::remove("test_invalid.xml");
}

TEST_F(ErrorTest, SimulatorBoundsChecking) {
    Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
    ErrorOutput errorOutput(logger);

    // Create simple valid XML
    std::ofstream xmlFile("test_bounds.xml");
    xmlFile << R"(<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>50</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>10</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)";
    xmlFile.close();

    Parser::initialize();
    auto sim = std::make_unique<Simulator>(errorOutput);

    if (Parser::parseElements("test_bounds.xml", sim.get(), errorOutput)) {
        // Modify voertuig position to be out of bounds manually for testing
        auto banen = sim->getBanen();
        if (!banen.empty()) {
            auto voertuigen = banen[0]->getVoertuigen();
            if (!voertuigen.empty()) {
                voertuigen[0]->setPositie(100); // Out of bounds (baan length = 50)
            }
        }

        // Run simulation - should generate bounds errors
        sim->simulate(1);
    }

    logger.flush();

    std::string actualContent = readFile("test_errors.txt");
    auto actualMessages = extractErrorMessages(actualContent);

    // Should detect bounds violation during graphical impression generation
    bool foundBoundsError = false;
    for (const auto& message : actualMessages) {
        if (message.find("positie buiten baan grenzen") != std::string::npos) {
            foundBoundsError = true;
            break;
        }
    }
    EXPECT_TRUE(foundBoundsError) << "Should detect position bounds violation during simulation";

    // Cleanup
    std::filesystem::remove("test_bounds.xml");
}

TEST_F(ErrorTest, NoErrorsWithValidInput) {
    Logger logger("test_errors.txt", false, Logger::LogLevel::ERROR, true);
    ErrorOutput errorOutput(logger);

    // Create completely valid XML
    std::ofstream xmlFile("test_valid.xml");
    xmlFile << R"(<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>500</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>100</positie>
        <type>auto</type>
    </VOERTUIG>
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>200</positie>
        <cyclus>30</cyclus>
    </VERKEERSLICHT>
</VERKEERSSITUATIE>)";
    xmlFile.close();

    Parser::initialize();
    auto sim = std::make_unique<Simulator>(errorOutput);

    bool result = Parser::parseElements("test_valid.xml", sim.get(), errorOutput);
    EXPECT_TRUE(result) << "Valid XML should parse successfully";

    if (result) {
        sim->simulate(5); // Run short simulation
    }

    logger.flush();

    std::string actualContent = readFile("test_errors.txt");
    auto actualMessages = extractErrorMessages(actualContent);

    EXPECT_TRUE(actualMessages.empty()) << "Valid input should produce no errors";

    // Cleanup
    std::filesystem::remove("test_valid.xml");
}

TEST_F(ErrorTest, DebugParserErrors) {
    Logger logger("test_errors.txt", true, Logger::LogLevel::ERROR, true); // ENABLE console output for debugging
    ErrorOutput errorOutput(logger);

    // Create invalid XML file for testing
    std::ofstream xmlFile("test_invalid.xml");
    xmlFile << R"(<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>100</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>150</positie>
        <type>auto</type>
    </VOERTUIG>
    <VERKEERSLICHT>
        <baan>TestBaan</baan>
        <positie>200</positie>
        <cyclus>10</cyclus>
    </VERKEERSLICHT>
</VERKEERSSITUATIE>)";
    xmlFile.close();


    // Test manual error first
    errorOutput.logError("MANUAL TEST ERROR");

    // Test parsing with errors
    Parser::initialize();
    auto sim = std::make_unique<Simulator>(errorOutput);


    logger.flush();

    // Read and print actual file content
    std::string actualContent = readFile("test_errors.txt");


    auto actualMessages = extractErrorMessages(actualContent);
    for (size_t i = 0; i < actualMessages.size(); ++i) {
    }

    // Cleanup
    std::filesystem::remove("test_invalid.xml");
}

TEST_F(ErrorTest, DebugSimulatorBounds) {
    Logger logger("test_errors.txt", true, Logger::LogLevel::ERROR, true); // Enable console
    ErrorOutput errorOutput(logger);

    std::cout << "=== DEBUGGING SIMULATOR BOUNDS ===" << std::endl;

    // Create simple valid XML
    std::ofstream xmlFile("test_bounds.xml");
    xmlFile << R"(<VERKEERSSITUATIE>
    <BAAN>
        <naam>TestBaan</naam>
        <lengte>50</lengte>
    </BAAN>
    <VOERTUIG>
        <baan>TestBaan</baan>
        <positie>10</positie>
        <type>auto</type>
    </VOERTUIG>
</VERKEERSSITUATIE>)";
    xmlFile.close();

    Parser::initialize();
    auto sim = std::make_unique<Simulator>(errorOutput);

    if (Parser::parseElements("test_bounds.xml", sim.get(), errorOutput)) {

        auto banen = sim->getBanen();

        if (!banen.empty()) {
            auto voertuigen = banen[0]->getVoertuigen();

            if (!voertuigen.empty()) {
                voertuigen[0]->setPositie(100); // Out of bounds (baan length = 50)
            }
        }

        sim->simulate(1);
    } else {
    }

    logger.flush();

    std::string actualContent = readFile("test_errors.txt");

    // Cleanup
    std::filesystem::remove("test_bounds.xml");
}

TEST_F(ErrorTest, TestErrorOutputDirectly) {
    Logger logger("test_errors.txt", true, Logger::LogLevel::ERROR, true);
    ErrorOutput errorOutput(logger);


    // Test all error types
    errorOutput.logError("Direct log error");
    errorOutput.handleError("Handled error", "TestContext");
    errorOutput.handleWarning("Warning message", "TestContext");  // Should be ignored (only ERROR level)

    try {
        throw std::runtime_error("Test exception");
    } catch (const std::exception& e) {
        errorOutput.handleException(e, "TestContext");
    }

    logger.flush();

    std::string actualContent = readFile("test_errors.txt");


    auto messages = extractErrorMessages(actualContent);

    int size = messages.size();
    EXPECT_GE(size, 3) << "Should have at least 3 error messages (warning should be ignored)";
}