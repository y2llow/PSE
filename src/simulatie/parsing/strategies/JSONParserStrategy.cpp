#include "JSONParserStrategy.h"
#include "../../../simulatie/Simulator.h"
#include "../../../elementen/Voertuiggenerator.h"
#include "../../../elementen/Verkeerslicht.h"
#include "../../../elementen/Bushalte.h"
#include "../../../elementen/Baan.h"
#include "../../../elementen/Kruispunt.h"
#include "../../../elementen/Voertuig.h"

#include <fstream>
#include <iostream>

// Static member initialization
std::map<std::string, Baan*> JSONParserStrategy::banenMap;

bool JSONParserStrategy::parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) {
#ifndef JSON_SUPPORT_ENABLED
    errorOutput.handleError("JSON support is niet gecompileerd - hercompileer met JSON library", "JSONParserStrategy::parseFile");
    return false;
#else
    if (!sim) {
        errorOutput.handleError("Simulator is nullptr", "JSONParserStrategy::parseFile");
        return false;
    }
    
    // Clear map voor nieuwe parse sessie
    banenMap.clear();
    
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            errorOutput.handleError("Kan JSON bestand niet openen: " + filename, "JSONParserStrategy::parseFile");
            return false;
        }
        
        nlohmann::json j;
        file >> j;
        
        // Check if root object exists
        if (!j.is_object()) {
            errorOutput.handleError("JSON root moet een object zijn", "JSONParserStrategy::parseFile");
            return false;
        }
        
        // Parse volgens JSON structuur - verwacht top-level object
        if (j.contains("banen") && j["banen"].is_array()) {
            parseBanen(j["banen"], sim, errorOutput);
        }
        
        if (j.contains("voertuigen") && j["voertuigen"].is_array()) {
            parseVoertuigen(j["voertuigen"], errorOutput);
        }
        
        if (j.contains("verkeerslichten") && j["verkeerslichten"].is_array()) {
            parseVerkeerslichten(j["verkeerslichten"], errorOutput);
        }
        
        if (j.contains("bushaltes") && j["bushaltes"].is_array()) {
            parseBushaltes(j["bushaltes"], errorOutput);
        }
        
        if (j.contains("kruispunten") && j["kruispunten"].is_array()) {
            parseKruisPunten(j["kruispunten"], errorOutput);
        }
        
        if (j.contains("voertuiggeneratoren") && j["voertuiggeneratoren"].is_array()) {
            parseVoertuiggeneratoren(j["voertuiggeneratoren"], errorOutput);
        }
        
        return true;
        
    } catch (const nlohmann::json::exception& e) {
        errorOutput.handleError("JSON parse error: " + std::string(e.what()), "JSONParserStrategy::parseFile");
        return false;
    } catch (const std::exception& e) {
        errorOutput.handleException(e, "JSONParserStrategy::parseFile");
        return false;
    }
#endif
}

std::vector<std::string> JSONParserStrategy::getSupportedExtensions() const {
#ifdef JSON_SUPPORT_ENABLED
    return {".json"};
#else
    return {}; // Geen support
#endif
}

std::string JSONParserStrategy::getStrategyName() const {
#ifdef JSON_SUPPORT_ENABLED
    return "JSON Parser";
#else
    return "JSON Parser (Disabled)";
#endif
}

#ifdef JSON_SUPPORT_ENABLED

void JSONParserStrategy::parseBanen(const nlohmann::json& banenArray, Simulator* sim, ErrorOutput& errorOutput) {
    for (const auto& baanJson : banenArray) {
        try {
            auto* baan = new Baan();
            bool geldig = true;
            
            std::string naam = safeGetString(baanJson, "naam");
            if (naam.empty()) {
                errorOutput.handleError("Baan heeft geen geldige naam", "JSONParserStrategy::parseBanen");
                delete baan;
                continue;
            }
            baan->setNaam(naam);
            
            int lengte = safeGetInt(baanJson, "lengte");
            if (lengte <= 0) {
                errorOutput.handleError("Baan lengte moet positief zijn: " + std::to_string(lengte), "JSONParserStrategy::parseBanen");
                delete baan;
                continue;
            }
            baan->setLengte(lengte);
            
            sim->addBaan(baan);
            banenMap[baan->getNaam()] = baan;
            
        } catch (const std::exception& e) {
            errorOutput.handleException(e, "JSONParserStrategy::parseBanen");
        }
    }
}

void JSONParserStrategy::parseVoertuigen(const nlohmann::json& voertuigenArray, ErrorOutput& errorOutput) {
    for (const auto& voertuigJson : voertuigenArray) {
        try {
            std::string baanNaam = safeGetString(voertuigJson, "baan");
            std::string type = safeGetString(voertuigJson, "type", "auto");
            int positie = safeGetInt(voertuigJson, "positie");
            double snelheid = safeGetDouble(voertuigJson, "snelheid", 16.6);
            
            // Validaties
            if (baanNaam.empty()) {
                errorOutput.handleError("Voertuig heeft geen geldige baan", "JSONParserStrategy::parseVoertuigen");
                continue;
            }
            
            if (banenMap.find(baanNaam) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor voertuig: " + baanNaam, "JSONParserStrategy::parseVoertuigen");
                continue;
            }
            
            if (positie < 0) {
                errorOutput.handleError("Voertuig positie moet positief zijn: " + std::to_string(positie), "JSONParserStrategy::parseVoertuigen");
                continue;
            }
            
            if (snelheid < 0) {
                errorOutput.handleError("Voertuig snelheid moet positief zijn: " + std::to_string(snelheid), "JSONParserStrategy::parseVoertuigen");
                continue;
            }
            
            // Valideer positie binnen baan grenzen
            Baan* baan = banenMap[baanNaam];
            if (positie >= baan->getLengte()) {
                errorOutput.handleError("Voertuig positie buiten baan grenzen: " + std::to_string(positie) + " >= " + std::to_string(baan->getLengte()), "JSONParserStrategy::parseVoertuigen");
                continue;
            }
            
            // Maak voertuig
            Voertuig* voertuig = Voertuig::createVoertuig(type);
            if (voertuig) {
                voertuig->setPositie(positie);
                voertuig->setSnelheid(snelheid);
                voertuig->setBaan(baan);
                baan->addVoertuig(voertuig);
            } else {
                errorOutput.handleError("Ongeldig voertuigtype: " + type, "JSONParserStrategy::parseVoertuigen");
            }
            
        } catch (const std::exception& e) {
            errorOutput.handleException(e, "JSONParserStrategy::parseVoertuigen");
        }
    }
}

void JSONParserStrategy::parseVerkeerslichten(const nlohmann::json& lichtenArray, ErrorOutput& errorOutput) {
    for (const auto& lichtJson : lichtenArray) {
        try {
            std::string baanNaam = safeGetString(lichtJson, "baan");
            int positie = safeGetInt(lichtJson, "positie");
            int cyclus = safeGetInt(lichtJson, "cyclus");
            
            // Validaties
            if (baanNaam.empty()) {
                errorOutput.handleError("Verkeerslicht heeft geen geldige baan", "JSONParserStrategy::parseVerkeerslichten");
                continue;
            }
            
            if (banenMap.find(baanNaam) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor verkeerslicht: " + baanNaam, "JSONParserStrategy::parseVerkeerslichten");
                continue;
            }
            
            if (positie < 0 || cyclus <= 0) {
                errorOutput.handleError("Verkeerslicht heeft ongeldige waarden", "JSONParserStrategy::parseVerkeerslichten");
                continue;
            }
            
            Baan* baan = banenMap[baanNaam];
            if (positie >= baan->getLengte()) {
                errorOutput.handleError("Verkeerslicht positie buiten baan grenzen", "JSONParserStrategy::parseVerkeerslichten");
                continue;
            }
            
            // Maak verkeerslicht
            auto* verkeerslicht = new Verkeerslicht();
            verkeerslicht->setBaan(baan);
            verkeerslicht->setPositie(positie);
            verkeerslicht->setCyclus(cyclus);
            
            baan->addVerkeerslicht(verkeerslicht);
            
        } catch (const std::exception& e) {
            errorOutput.handleException(e, "JSONParserStrategy::parseVerkeerslichten");
        }
    }
}

void JSONParserStrategy::parseBushaltes(const nlohmann::json& haltesArray, ErrorOutput& errorOutput) {
    for (const auto& halteJson : haltesArray) {
        try {
            std::string baanNaam = safeGetString(halteJson, "baan");
            int positie = safeGetInt(halteJson, "positie");
            int wachttijd = safeGetInt(halteJson, "wachttijd");
            
            // Validaties
            if (baanNaam.empty()) {
                errorOutput.handleError("Bushalte heeft geen geldige baan", "JSONParserStrategy::parseBushaltes");
                continue;
            }
            
            if (banenMap.find(baanNaam) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor bushalte: " + baanNaam, "JSONParserStrategy::parseBushaltes");
                continue;
            }
            
            if (positie < 0 || wachttijd < 0) {
                errorOutput.handleError("Bushalte heeft ongeldige waarden", "JSONParserStrategy::parseBushaltes");
                continue;
            }
            
            Baan* baan = banenMap[baanNaam];
            if (positie >= baan->getLengte()) {
                errorOutput.handleError("Bushalte positie buiten baan grenzen", "JSONParserStrategy::parseBushaltes");
                continue;
            }
            
            // Maak bushalte
            auto* bushalte = new Bushalte();
            bushalte->setBaan(baan);
            bushalte->setPositie(positie);
            bushalte->setWachttijd(wachttijd);
            
            baan->addBushalte(bushalte);
            
        } catch (const std::exception& e) {
            errorOutput.handleException(e, "JSONParserStrategy::parseBushaltes");
        }
    }
}

void JSONParserStrategy::parseKruisPunten(const nlohmann::json& kruispuntenArray, ErrorOutput& errorOutput) {
    for (const auto& kruispuntJson : kruispuntenArray) {
        try {
            if (!kruispuntJson.contains("banen") || !kruispuntJson["banen"].is_array()) {
                errorOutput.handleError("Kruispunt moet 'banen' array hebben", "JSONParserStrategy::parseKruisPunten");
                continue;
            }
            
            std::vector<Baan*> bannen;
            std::map<Baan*, int> positions;
            
            for (const auto& baanJson : kruispuntJson["banen"]) {
                std::string baanNaam = safeGetString(baanJson, "naam");
                int positie = safeGetInt(baanJson, "positie");
                
                if (baanNaam.empty()) {
                    errorOutput.handleError("Kruispunt baan heeft geen naam", "JSONParserStrategy::parseKruisPunten");
                    continue;
                }
                
                if (banenMap.find(baanNaam) == banenMap.end()) {
                    errorOutput.handleError("Baan niet gevonden voor kruispunt: " + baanNaam, "JSONParserStrategy::parseKruisPunten");
                    continue;
                }
                
                if (positie < 0) {
                    errorOutput.handleError("Kruispunt positie moet positief zijn: " + std::to_string(positie), "JSONParserStrategy::parseKruisPunten");
                    continue;
                }
                
                Baan* baan = banenMap[baanNaam];
                if (positie >= baan->getLengte()) {
                    errorOutput.handleError("Kruispunt positie buiten baan grenzen", "JSONParserStrategy::parseKruisPunten");
                    continue;
                }
                
                positions[baan] = positie;
                bannen.push_back(baan);
            }
            
            if (bannen.size() < 2) {
                errorOutput.handleError("Kruispunt moet minstens 2 geldige banen hebben", "JSONParserStrategy::parseKruisPunten");
                continue;
            }
            
            // Maak kruispunt
            const auto kruispunt = Kruispunt::createNewKruispunt(bannen, positions);
            
            for (const auto b : kruispunt->getBannen())
                b->addKruispunt(kruispunt);
            
            for (const auto b : bannen) {
                for (const auto v : b->getVerkeerslichten()) {
                    if (positions[b] == v->getPositie())
                        v->setKruispunt(kruispunt);
                }
            }
            
        } catch (const std::exception& e) {
            errorOutput.handleException(e, "JSONParserStrategy::parseKruisPunten");
        }
    }
}

void JSONParserStrategy::parseVoertuiggeneratoren(const nlohmann::json& generatorsArray, ErrorOutput& errorOutput) {
    for (const auto& generatorJson : generatorsArray) {
        try {
            std::string baanNaam = safeGetString(generatorJson, "baan");
            double frequentie = safeGetDouble(generatorJson, "frequentie");
            std::string type = safeGetString(generatorJson, "type", "auto");
            
            // Validaties
            if (baanNaam.empty()) {
                errorOutput.handleError("Voertuiggenerator heeft geen geldige baan", "JSONParserStrategy::parseVoertuiggeneratoren");
                continue;
            }
            
            if (banenMap.find(baanNaam) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor voertuiggenerator: " + baanNaam, "JSONParserStrategy::parseVoertuiggeneratoren");
                continue;
            }
            
            if (frequentie < 0) {
                errorOutput.handleError("Frequentie moet positief zijn: " + std::to_string(frequentie), "JSONParserStrategy::parseVoertuiggeneratoren");
                continue;
            }
            
            if (!geldigeTypen(type)) {
                errorOutput.handleError("Ongeldig voertuigtype voor generator: " + type, "JSONParserStrategy::parseVoertuiggeneratoren");
                continue;
            }
            
            // Maak generator
            Baan* baan = banenMap[baanNaam];
            auto* generator = new Voertuiggenerator(baan, frequentie, type);
            baan->addVoertuiggenerator(generator);
            
        } catch (const std::exception& e) {
            errorOutput.handleException(e, "JSONParserStrategy::parseVoertuiggeneratoren");
        }
    }
}

// Helper methods
std::string JSONParserStrategy::safeGetString(const nlohmann::json& obj, const std::string& key, const std::string& defaultValue) {
    if (obj.contains(key) && obj[key].is_string()) {
        return obj[key];
    }
    return defaultValue;
}

int JSONParserStrategy::safeGetInt(const nlohmann::json& obj, const std::string& key, int defaultValue) {
    if (obj.contains(key) && obj[key].is_number_integer()) {
        return obj[key];
    }
    return defaultValue;
}

double JSONParserStrategy::safeGetDouble(const nlohmann::json& obj, const std::string& key, double defaultValue) {
    if (obj.contains(key) && obj[key].is_number()) {
        return obj[key];
    }
    return defaultValue;
}

void JSONParserStrategy::exceptionFound(bool& geldig, const std::string& message, ErrorOutput& errorOutput) {
    errorOutput.handleError(message, "JSONParserStrategy");
    geldig = false;
}

bool JSONParserStrategy::geldigeTypen(const std::string& type) {
    return type == "auto" || type == "bus" || type == "brandweerwagen" ||
           type == "politiecombi" || type == "ziekenwagen";
}

#endif // JSON_SUPPORT_ENABLED