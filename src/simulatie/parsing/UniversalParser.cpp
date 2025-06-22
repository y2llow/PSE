//============================================================================
// Name        : UniversalParser.cpp
// Author      : Universal Parser System
// Description : Implementation of Universal Parser with backwards compatibility
//============================================================================

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
        printSupportedFormats(errorOutput);
        return false;
    }

    errorOutput.handleInfo("Gebruik " + strategy->getStrategyName() + " voor: " + filename, "UniversalParser::parseFile");
    return strategy->parseFile(filename, sim, errorOutput);
}

bool UniversalParser::parseElements(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) {
    return parseFile(filename, sim, errorOutput);
}

bool UniversalParser::parseElements(const std::string& filename, Simulator* sim) {
    // Fallback logger voor oude code
    static Logger fallbackLogger("logs/parser_fallback.txt", true, Logger::LogLevel::ERROR);
    static ErrorOutput fallbackErrorOutput(fallbackLogger);

    return parseFile(filename, sim, fallbackErrorOutput);
}

void UniversalParser::printSupportedFormats(ErrorOutput& errorOutput) {
    auto extensions = ParserFactory::getSupportedExtensions();
    auto strategies = ParserFactory::getRegisteredStrategyNames();

    std::string extensionList = "Ondersteunde formaten: ";
    for (size_t i = 0; i < extensions.size(); ++i) {
        extensionList += extensions[i];
        if (i < extensions.size() - 1) {
            extensionList += ", ";
        }
    }

    std::string strategyList = "Beschikbare parsers: ";
    for (size_t i = 0; i < strategies.size(); ++i) {
        strategyList += strategies[i];
        if (i < strategies.size() - 1) {
            strategyList += ", ";
        }
    }

    errorOutput.handleInfo(extensionList, "UniversalParser");
    errorOutput.handleInfo(strategyList, "UniversalParser");
}

bool UniversalParser::isFileSupported(const std::string& filename) {
    IParserStrategy* strategy = ParserFactory::getParserForFile(filename);
    return strategy != nullptr;
}