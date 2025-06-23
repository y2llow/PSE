#include <iostream>
#include <memory>

#include "src/simulatie/parsing/UniversalParser.h"
#include "src/simulatie/Simulator.h"
#include "src/logs_and_errors/Logger.h"
#include "src/logs_and_errors/ErrorOutput.h"

using namespace std;

int main()
{
    // Setup logging system
    Logger logger("../output/errors.txt",
                  false,                    // Geen console output
                  Logger::LogLevel::ERROR,      // Alleen errors
                  true);                        // Clear file eerst
    ErrorOutput errorOutput(logger);

    try {
        UniversalParser::initialize();

        // Gebruik ErrorOutput constructor
        auto sim = std::make_unique<Simulator>(errorOutput);

        // Test eerst met je originele XML - nu met automatische format detectie!
        if (UniversalParser::parseElements("../src/voorbeeldXML/voorbeeld10.xml", sim.get(), errorOutput)) {
            sim->simulate(10000);
        } else {
            errorOutput.logError("XML parsing gefaald");
            return 1;
        }

    } catch (const std::exception& e) {
        errorOutput.handleException(e, "main");
        return 1;
    } catch (...) {
        errorOutput.logError("Onbekende fout opgetreden in main");
        return 1;
    }

    return 0;
}