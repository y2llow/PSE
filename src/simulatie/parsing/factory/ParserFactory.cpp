
#include "ParserFactory.h"
#include "../strategies/XMLParserStrategy.h"
#ifdef JSON_SUPPORT_ENABLED
#include "../strategies/JSONParserStrategy.h"
#endif
#include <filesystem>
#include <algorithm>

// Static member initialization
std::map<std::string, std::unique_ptr<IParserStrategy>> ParserFactory::strategies;
bool ParserFactory::initialized = false;

void ParserFactory::initialize() {
    if (initialized) return;

    // Registreer alle standaard strategies
    registerStrategy(".xml", std::make_unique<XMLParserStrategy>());

    // TODO: Voeg hier andere strategies toe als ze beschikbaar zijn
    // registerStrategy(".json", std::make_unique<JSONParserStrategy>());
    // registerStrategy(".csv", std::make_unique<CSVParserStrategy>());

    initialized = true;
}

void ParserFactory::registerStrategy(const std::string& extension, std::unique_ptr<IParserStrategy> strategy) {
    if (!strategy) {
        return; // Ignore null strategies
    }

    // Normalize extension (ensure it starts with a dot and is lowercase)
    std::string normalizedExt = extension;
    if (!normalizedExt.empty() && normalizedExt[0] != '.') {
        normalizedExt = "." + normalizedExt;
    }
    std::transform(normalizedExt.begin(), normalizedExt.end(), normalizedExt.begin(), ::tolower);

    strategies[normalizedExt] = std::move(strategy);
}

IParserStrategy* ParserFactory::getParserForFile(const std::string& filename) {
    if (!initialized) {
        initialize();
    }

    // Extract extensie van filename
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return nullptr; // Geen extensie gevonden
    }

    std::string extension = filename.substr(dotPos);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    auto it = strategies.find(extension);
    return (it != strategies.end()) ? it->second.get() : nullptr;
}

std::vector<std::string> ParserFactory::getSupportedExtensions() {
    if (!initialized) {
        initialize();
    }

    std::vector<std::string> extensions;
    for (const auto& pair : strategies) {
        extensions.push_back(pair.first);
    }

    return extensions;
}

std::vector<std::string> ParserFactory::getRegisteredStrategyNames() {
    if (!initialized) {
        initialize();
    }

    std::vector<std::string> names;
    for (const auto& pair : strategies) {
        if (pair.second) {
            names.push_back(pair.second->getStrategyName());
        }
    }

    return names;
}

bool ParserFactory::isExtensionSupported(const std::string& extension) {
    if (!initialized) {
        initialize();
    }

    std::string normalizedExt = extension;
    if (!normalizedExt.empty() && normalizedExt[0] != '.') {
        normalizedExt = "." + normalizedExt;
    }
    std::transform(normalizedExt.begin(), normalizedExt.end(), normalizedExt.begin(), ::tolower);

    return strategies.find(normalizedExt) != strategies.end();
}

void ParserFactory::reset() {
    strategies.clear();
    initialized = false;
}