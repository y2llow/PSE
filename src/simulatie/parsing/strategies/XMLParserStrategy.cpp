
#include "XMLParserStrategy.h"
#include "../../../simulatie/Simulator.h"
#include "../../../elementen/Voertuiggenerator.h"
#include "../../../elementen/Verkeerslicht.h"
#include "../../../elementen/Bushalte.h"
#include "../../../elementen/Baan.h"
#include "../../../elementen/Kruispunt.h"
#include "../../../elementen/Voertuig.h"

#include <algorithm>
#include <iostream>

// Static member initialization
std::map<std::string, Baan*> XMLParserStrategy::banenMap;


std::vector<std::string> XMLParserStrategy::getSupportedExtensions() const {
    return {".xml"};
}

std::string XMLParserStrategy::getStrategyName() const {
    return "XML Parser";
}

void XMLParserStrategy::parseBanen(TiXmlElement* elem, Simulator* sim, ErrorOutput& errorOutput) {
    const auto baan = new Baan();
    bool geldig = true;

    // Loop through sub-elements to get properties
    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement()) {
        if (std::string propertyName = subElem->Value(); propertyName == "naam") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een baan zonder naam!", "XMLParserStrategy::parseBanen");
                geldig = false;
                break;
            }
            baan->setNaam(subElem->GetText());
        } else if (propertyName == "lengte") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een baan zonder lengte!", "XMLParserStrategy::parseBanen");
                geldig = false;
                break;
            }
            try {
                int lengte = std::stoi(subElem->GetText());
                if (lengte <= 0) {
                    errorOutput.handleError("Baan lengte moet positief zijn: " + std::to_string(lengte), "XMLParserStrategy::parseBanen");
                    geldig = false;
                    break;
                }
                baan->setLengte(lengte);
            } catch (std::exception& e) {
                errorOutput.handleError("De lengte van een baan is geen integer: " + std::string(subElem->GetText()), "XMLParserStrategy::parseBanen");
                geldig = false;
                break;
            }
        }
    }

    if (geldig && !baan->getNaam().empty() && baan->getLengte() > 0) {
        sim->addBaan(baan);
        banenMap[baan->getNaam()] = baan;
    } else {
        errorOutput.handleError("Baan wordt overgeslagen vanwege ongeldige eigenschappen", "XMLParserStrategy::parseBanen");
        delete baan;
    }
}

void XMLParserStrategy::parseVoertuigen(TiXmlElement* elem, ErrorOutput& errorOutput) {
    bool geldig = true;

    Voertuig* voertuig = nullptr;
    Baan* voertuig_baan = nullptr;
    double voertuig_snelheid = 16.6; // default value
    int positie = 0;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement()) {
        if (std::string propertyName = subElem->Value(); propertyName == "baan") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een voertuig zonder baan!", "XMLParserStrategy::parseVoertuigen");
                geldig = false;
                break;
            }

            if (banenMap.find(subElem->GetText()) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor voertuig: " + std::string(subElem->GetText()), "XMLParserStrategy::parseVoertuigen");
                geldig = false;
                break;
            }
            voertuig_baan = banenMap[subElem->GetText()];
        } else if (propertyName == "positie") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een voertuig zonder positie!", "XMLParserStrategy::parseVoertuigen");
                geldig = false;
                break;
            }

            try {
                positie = std::stoi(subElem->GetText());
                if (positie < 0) {
                    errorOutput.handleError("Voertuig positie moet positief zijn: " + std::to_string(positie), "XMLParserStrategy::parseVoertuigen");
                    geldig = false;
                    break;
                }
            } catch (std::exception&) {
                errorOutput.handleError("Er is een voertuig waarvan de positie geen integer is: " + std::string(subElem->GetText()), "XMLParserStrategy::parseVoertuigen");
                geldig = false;
                break;
            }
        } else if (propertyName == "type") {
            if (subElem->GetText()) {
                std::string typeString = subElem->GetText();
                voertuig = Voertuig::createVoertuig(typeString);
                if (voertuig == nullptr) {
                    errorOutput.handleError("Ongeldig voertuigtype: " + typeString, "XMLParserStrategy::parseVoertuigen");
                    geldig = false;
                    break;
                }
            }
        } else if (propertyName == "snelheid") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een voertuig zonder snelheid!", "XMLParserStrategy::parseVoertuigen");
                geldig = false;
                break;
            }
            std::string snelheidstring = subElem->GetText();

            try {
                voertuig_snelheid = std::stod(snelheidstring);
                if (voertuig_snelheid < 0) {
                    errorOutput.handleError("Voertuig snelheid moet positief zijn: " + std::to_string(voertuig_snelheid), "XMLParserStrategy::parseVoertuigen");
                    geldig = false;
                    break;
                }
            } catch (std::exception&) {
                errorOutput.handleError("De snelheid van een voertuig is geen double: " + snelheidstring, "XMLParserStrategy::parseVoertuigen");
                geldig = false;
                break;
            }
        }
    }

    // Als we geen geldige baan of positie hebben, sla over
    if (geldig && voertuig_baan != nullptr) {
        // Check of positie binnen baan grenzen valt
        if (positie >= voertuig_baan->getLengte()) {
            errorOutput.handleError("Voertuig positie buiten baan grenzen: " + std::to_string(positie) + " >= " + std::to_string(voertuig_baan->getLengte()), "XMLParserStrategy::parseVoertuigen");
            delete voertuig;
            return;
        }

        if (voertuig == nullptr)
            voertuig = Voertuig::createVoertuig("auto"); // Default type

        voertuig->setPositie(positie);
        voertuig->setSnelheid(voertuig_snelheid);
        voertuig->setBaan(voertuig_baan);

        voertuig_baan->addVoertuig(voertuig);
    } else {
        errorOutput.handleError("Voertuig wordt overgeslagen vanwege ongeldige eigenschappen", "XMLParserStrategy::parseVoertuigen");
        delete voertuig;
    }
}

void XMLParserStrategy::parseVerkeerslichten(TiXmlElement* elem, ErrorOutput& errorOutput) {
    const char* baan = nullptr;
    int cyclus = -1;
    int positie = -1;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement()) {
        if (std::string propertyName = subElem->Value(); propertyName == "baan") {
            baan = subElem->GetText();
            if (!baan) {
                errorOutput.handleError("Verkeerslicht heeft geen baan naam", "XMLParserStrategy::parseVerkeerslichten");
                return;
            }
        } else if (propertyName == "positie") {
            try {
                positie = std::stoi(subElem->GetText());
                if (positie < 0) {
                    errorOutput.handleError("Verkeerslicht positie moet positief zijn: " + std::to_string(positie), "XMLParserStrategy::parseVerkeerslichten");
                    return;
                }
            } catch (std::exception&) {
                errorOutput.handleError("Verkeerslicht positie is geen integer: " + std::string(subElem->GetText()), "XMLParserStrategy::parseVerkeerslichten");
                return;
            }
        } else if (propertyName == "cyclus") {
            try {
                cyclus = std::stoi(subElem->GetText());
                if (cyclus <= 0) {
                    errorOutput.handleError("Verkeerslicht cyclus moet positief zijn: " + std::to_string(cyclus), "XMLParserStrategy::parseVerkeerslichten");
                    return;
                }
            } catch (std::exception&) {
                errorOutput.handleError("Er is een verkeerslicht waarvan de cyclus geen integer is: " + std::string(subElem->GetText()), "XMLParserStrategy::parseVerkeerslichten");
                return;
            }
        }
    }

    if (cyclus < 0 || positie < 0 || !baan) {
        errorOutput.handleError("Verkeerslicht heeft ongeldige eigenschappen", "XMLParserStrategy::parseVerkeerslichten");
        return;
    }

    // Check of baan bestaat
    if (banenMap.find(baan) == banenMap.end()) {
        errorOutput.handleError("Baan niet gevonden voor verkeerslicht: " + std::string(baan), "XMLParserStrategy::parseVerkeerslichten");
        return;
    }

    // Check of positie binnen baan grenzen valt
    if (positie >= banenMap[baan]->getLengte()) {
        errorOutput.handleError("Verkeerslicht positie buiten baan grenzen: " + std::to_string(positie) + " >= " + std::to_string(banenMap[baan]->getLengte()), "XMLParserStrategy::parseVerkeerslichten");
        return;
    }

    auto* verkeerslicht = new Verkeerslicht();
    verkeerslicht->setBaan(banenMap[baan]);
    verkeerslicht->setCyclus(cyclus);
    verkeerslicht->setPositie(positie);

    banenMap[baan]->addVerkeerslicht(verkeerslicht);
}

void XMLParserStrategy::parseVoertuiggeneratoren(TiXmlElement* elem, ErrorOutput& errorOutput) {
    Baan* voertuigbaan = nullptr;
    double frequentie = 0;
    std::string type;

    bool geldig = true;

    // Verzamel alle benodigde eigenschappen
    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement()) {
        if (std::string propertyName = subElem->Value(); propertyName == "baan") {
            if (!subElem->GetText()) {
                exceptionFound(geldig, "Er is een VOERTUIGGENERATOR zonder baan!", errorOutput);
                break;
            }

            // Baan isn't found, dus niet toevoegen aan voertuiggenerators
            if (banenMap.find(subElem->GetText()) == banenMap.end()) {
                exceptionFound(geldig, "Baan niet gevonden voor voertuiggenerator: " + std::string(subElem->GetText()), errorOutput);
                return;
            }

            voertuigbaan = banenMap[subElem->GetText()];
        } else if (propertyName == "frequentie") {
            if (!subElem->GetText()) {
                exceptionFound(geldig, "Er is een voertuiggenerator zonder frequentie!", errorOutput);
                break;
            }

            try {
                frequentie = std::stod(subElem->GetText());
                if (frequentie < 0) {
                    exceptionFound(geldig, "Frequentie moet positief zijn: " + std::to_string(frequentie), errorOutput);
                    break;
                }
            } catch (std::exception&) {
                exceptionFound(geldig, "De frequentie van een voertuiggenerator is geen getal: " + std::string(subElem->GetText()), errorOutput);
                break;
            }
        } else if (propertyName == "type") {
            if (subElem->GetText()) {
                type = subElem->GetText();
                // Validate type
                if (!geldigeTypen(type)) {
                    exceptionFound(geldig, "Ongeldig voertuigtype voor generator: " + type, errorOutput);
                    break;
                }
            }
        }
    }

    // Controleer of alle verplichte eigenschappen zijn ingesteld
    if (geldig && voertuigbaan != nullptr && !type.empty()) {
        // Maak de generator met de verzamelde gegevens
        auto* generator = new Voertuiggenerator(voertuigbaan, frequentie, type);

        // Voeg de generator toe aan de simulatie
        voertuigbaan->addVoertuiggenerator(generator);
    } else {
        errorOutput.handleError("Voertuiggenerator wordt overgeslagen vanwege ongeldige eigenschappen", "XMLParserStrategy::parseVoertuiggeneratoren");
    }
}

void XMLParserStrategy::parseBushaltes(TiXmlElement* elem, ErrorOutput& errorOutput) {
    auto* bushalte = new Bushalte();
    bool geldig = true;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement()) {
        if (std::string propertyName = subElem->Value(); propertyName == "baan") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Bushalte heeft geen baan naam", "XMLParserStrategy::parseBushaltes");
                geldig = false;
                break;
            }

            if (banenMap.find(subElem->GetText()) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor bushalte: " + std::string(subElem->GetText()), "XMLParserStrategy::parseBushaltes");
                geldig = false;
                break;
            }

            Baan* bushalte_baan = banenMap[subElem->GetText()];
            bushalte->setBaan(bushalte_baan);
        } else if (propertyName == "positie") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een bushalte zonder positie!", "XMLParserStrategy::parseBushaltes");
                geldig = false;
                break;
            }
            try {
                int positie = std::stoi(subElem->GetText());
                if (positie < 0) {
                    errorOutput.handleError("Bushalte positie moet positief zijn: " + std::to_string(positie), "XMLParserStrategy::parseBushaltes");
                    geldig = false;
                    break;
                }
                bushalte->setPositie(positie);
            } catch (std::exception&) {
                errorOutput.handleError("Er is een bushalte waarvan de positie geen integer is: " + std::string(subElem->GetText()), "XMLParserStrategy::parseBushaltes");
                geldig = false;
                break;
            }
        } else if (propertyName == "wachttijd") {
            if (!subElem->GetText()) {
                errorOutput.handleError("Er is een bushalte zonder wachttijd!", "XMLParserStrategy::parseBushaltes");
                geldig = false;
                break;
            }
            try {
                int wachttijd = std::stoi(subElem->GetText());
                if (wachttijd < 0) {
                    errorOutput.handleError("Bushalte wachttijd moet positief zijn: " + std::to_string(wachttijd), "XMLParserStrategy::parseBushaltes");
                    geldig = false;
                    break;
                }
                bushalte->setWachttijd(wachttijd);
            } catch (std::exception&) {
                errorOutput.handleError("Er is een bushalte waarvan de wachttijd geen integer is: " + std::string(subElem->GetText()), "XMLParserStrategy::parseBushaltes");
                geldig = false;
                break;
            }
        }
    }

    if (geldig && bushalte->getBaan() != nullptr) {
        // Check of positie binnen baan grenzen valt
        if (bushalte->getPositie() >= bushalte->getBaan()->getLengte()) {
            errorOutput.handleError("Bushalte positie buiten baan grenzen: " + std::to_string(bushalte->getPositie()) + " >= " + std::to_string(bushalte->getBaan()->getLengte()), "XMLParserStrategy::parseBushaltes");
            delete bushalte;
            return;
        }

        Baan* baan = bushalte->getBaan();
        baan->addBushalte(bushalte);
    } else {
        errorOutput.handleError("Bushalte wordt overgeslagen vanwege ongeldige eigenschappen", "XMLParserStrategy::parseBushaltes");
        delete bushalte;
    }
}

void XMLParserStrategy::parseKruisPunten(TiXmlElement* elem, ErrorOutput& errorOutput) {
    std::vector<Baan*> bannen;
    std::map<Baan*, int> positions;

    for (TiXmlElement* baanElem = elem->FirstChildElement("baan"); baanElem; baanElem = baanElem->NextSiblingElement("baan")) {
        // Attributen ophalen
        const char* positieAttr = baanElem->Attribute("positie");
        if (!positieAttr) {
            errorOutput.handleError("Kruispunt baan heeft geen positie attribuut", "XMLParserStrategy::parseKruisPunten");
            continue;
        }

        const char* baanNaam = baanElem->GetText();
        if (!baanNaam) {
            errorOutput.handleError("Kruispunt heeft lege baan naam", "XMLParserStrategy::parseKruisPunten");
            continue;
        }

        try {
            const int positieInt = std::stoi(positieAttr);

            if (banenMap.find(baanNaam) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor kruispunt: " + std::string(baanNaam), "XMLParserStrategy::parseKruisPunten");
                continue;
            }

            if (positieInt < 0) {
                errorOutput.handleError("Kruispunt positie moet positief zijn: " + std::to_string(positieInt), "XMLParserStrategy::parseKruisPunten");
                continue;
            }

            if (positieInt >= banenMap[baanNaam]->getLengte()) {
                errorOutput.handleError("Kruispunt positie buiten baan grenzen: " + std::to_string(positieInt) + " >= " + std::to_string(banenMap[baanNaam]->getLengte()), "XMLParserStrategy::parseKruisPunten");
                continue;
            }

            positions[banenMap[baanNaam]] = positieInt;
            bannen.push_back(banenMap[baanNaam]);

        } catch (std::exception&) {
            errorOutput.handleError("Kruispunt positie is geen integer: " + std::string(positieAttr), "XMLParserStrategy::parseKruisPunten");
            continue;
        }
    }

    if (bannen.size() < 2) {
        errorOutput.handleError("Kruispunt moet minstens 2 geldige banen hebben", "XMLParserStrategy::parseKruisPunten");
        return;
    }

    const auto kruispunt = Kruispunt::createNewKruispunt(bannen, positions);

    for (const auto b : kruispunt->getBannen())
        b->addKruispunt(kruispunt);

    for (const auto b : bannen) {
        for (const auto v : b->getVerkeerslichten()) {
            if (positions[b] == v->getPositie())
                v->setKruispunt(kruispunt);
        }
    }
}

void XMLParserStrategy::exceptionFound(bool& geldig, const std::string& message, ErrorOutput& errorOutput) {
    errorOutput.handleError(message, "XMLParserStrategy");
    geldig = false;
}

bool XMLParserStrategy::geldigeTypen(const std::string& type) {
    return type == "auto" || type == "bus" || type == "brandweerwagen" ||
           type == "politiecombi" || type == "ziekenwagen";
}


bool XMLParserStrategy::parseFile(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput) {
    if (!sim) {
        errorOutput.handleError("Simulator is nullptr", "XMLParserStrategy::parseFile");
        return false;
    }

    // Clear map voor nieuwe parse sessie
    banenMap.clear();

    TiXmlDocument doc;
    if (!doc.LoadFile(filename.c_str())) {
        errorOutput.handleError("Error loading XML file: " + std::string(doc.ErrorDesc()), "XMLParserStrategy::parseFile");
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root) {
        errorOutput.handleError("Failed to load file: No root element", "XMLParserStrategy::parseFile");
        return false;
    }

    try {
        // Parse banen eerst omdat ze essentieel zijn voor andere elementen
        for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr;
             ti_xml_element = ti_xml_element->NextSiblingElement()) {
            if (std::string elementType = ti_xml_element->Value(); elementType == "BAAN") {
                parseBanen(ti_xml_element, sim, errorOutput);
            }
        }

        // Parse andere elementen
        for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr;
             ti_xml_element = ti_xml_element->NextSiblingElement()) {
            if (std::string elementType = ti_xml_element->Value(); elementType == "VOERTUIG") {
                parseVoertuigen(ti_xml_element, errorOutput);
            } else if (elementType == "VOERTUIGGENERATOR") {
                parseVoertuiggeneratoren(ti_xml_element, errorOutput);
            } else if (elementType == "VERKEERSLICHT") {
                parseVerkeerslichten(ti_xml_element, errorOutput);
            } else if (elementType == "BUSHALTE") {
                parseBushaltes(ti_xml_element, errorOutput);
            } else if (elementType == "KRUISPUNT") {
                parseKruisPunten(ti_xml_element, errorOutput);
            }
        }

        return true;

    } catch (const std::exception& e) {
        errorOutput.handleException(e, "XMLParserStrategy::parseFile");
        return false;
    }
}