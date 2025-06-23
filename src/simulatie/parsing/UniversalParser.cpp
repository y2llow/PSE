#include "UniversalParser.h"
#include "../Simulator.h"
#include "../../logs_and_errors/Logger.h"
#include "../../logs_and_errors/ErrorOutput.h"

// Static member initialization
UniversalParser* UniversalParser::_initCheck = nullptr;

void UniversalParser::initialize() {
    static UniversalParser instance;
    _initCheck = &instance;
    ParserFactory::initialize();
}

bool UniversalParser::parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) {
    if (!sim) {
        errorOutput.handleError("Simulator is nullptr", "UniversalParser::parseFile");
        return false;
    }

    IParserStrategy* strategy = ParserFactory::getParserForFile(filename);
    if (!strategy) {
        errorOutput.handleError("Geen ondersteunde parser gevonden voor: " + filename, "UniversalParser::parseFile");
        return false;
    }

    errorOutput.handleInfo("Gebruik " + strategy->getStrategyName() + " voor: " + filename, "UniversalParser::parseFile");
    return strategy->parseFile(filename, sim, errorOutput);
}

bool UniversalParser::parseElements(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) {
    return parseFile(filename, sim, errorOutput);
}

// Fallback logger voor oude code
bool UniversalParser::parseElements(const std::string& filename, Simulator* sim) {
    static Logger fallbackLogger("logs/parser_fallback.txt", true, Logger::LogLevel::ERROR);
    static ErrorOutput fallbackErrorOutput(fallbackLogger);

    return parseFile(filename, sim, fallbackErrorOutput);
}
