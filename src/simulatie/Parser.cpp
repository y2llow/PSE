#include "Parser.h"

#include "../elementen/Voertuiggenerator.h"
#include "../elementen/Verkeerslicht.h"
#include "../elementen/Bushalte.h"
#include "../elementen/Baan.h"

#include <algorithm>
#include <iostream>

#include "../elementen/Kruispunt.h"

map<string, Baan*> Parser::banenMap;
Parser* Parser::_initCheck = nullptr;

void Parser::parseBanen(TiXmlElement* elem, Simulator* sim, ErrorOutput& errorOutput)
{
    const auto baan = new Baan();
    bool geldig = true;
    // Loop through sub-elements to get properties
    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "naam")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een baan zonder naam!", "Parser::parseBanen");
                geldig = false;
                break;
            }
            baan->setNaam(subElem->GetText());
        }
        else if (propertyName == "lengte")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een baan zonder lengte!", "Parser::parseBanen");
                geldig = false;
                break;
            }
            try
            {
                int lengte = stoi(subElem->GetText());
                if (lengte <= 0) {
                    errorOutput.handleError("Baan lengte moet positief zijn: " + to_string(lengte), "Parser::parseBanen");
                    geldig = false;
                    break;
                }
                baan->setLengte(lengte);
            }
            catch (exception& e)
            {
                errorOutput.handleError("De lengte van een baan is geen integer: " + string(subElem->GetText()), "Parser::parseBanen");
                geldig = false;
                break;
            }
        }
    }

    if (geldig && !baan->getNaam().empty() && baan->getLengte() > 0)
    {
        sim->addBaan(baan);
        banenMap[baan->getNaam()] = baan;
    }
    else {
        errorOutput.handleError("Baan wordt overgeslagen vanwege ongeldige eigenschappen", "Parser::parseBanen");
        delete baan;
    }
}

void Parser::parseVoertuigen(TiXmlElement* elem, ErrorOutput& errorOutput)
{
    bool geldig = true;

    Voertuig* voertuig = nullptr;
    Baan* voertuig_baan = nullptr;
    double voertuig_snelheid = 16.6; // default value
    int positie = 0;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een voertuig zonder baan!", "Parser::parseVoertuigen");
                geldig = false;
                break;
            }

            if (banenMap.find(subElem->GetText()) == banenMap.end())
            {
                errorOutput.handleError("Baan niet gevonden voor voertuig: " + string(subElem->GetText()), "Parser::parseVoertuigen");
                geldig = false;
                break;
            }
            voertuig_baan = banenMap[subElem->GetText()];
        }
        else if (propertyName == "positie")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een voertuig zonder positie!", "Parser::parseVoertuigen");
                geldig = false;
                break;
            }

            try
            {
                positie = stoi(subElem->GetText());
                if (positie < 0 )
                {
                    errorOutput.handleError("Voertuig positie moet positief zijn: " + to_string(positie), "Parser::parseVoertuigen");
                    geldig = false;
                    break;
                }
            }
            catch (exception&)
            {
                errorOutput.handleError("Er is een voertuig waarvan de positie geen integer is: " + string(subElem->GetText()), "Parser::parseVoertuigen");
                geldig = false;
                break;
            }
        }
        else if (propertyName == "type")
        {
            if (subElem->GetText())
            {
                std::string typeString = subElem->GetText();
                voertuig = Voertuig::createVoertuig(typeString);
                if (voertuig == nullptr) {
                    errorOutput.handleError("Ongeldig voertuigtype: " + typeString, "Parser::parseVoertuigen");
                    geldig = false;
                    break;
                }
            }
        }
        else if (propertyName == "snelheid")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een voertuig zonder snelheid!", "Parser::parseVoertuigen");
                geldig = false;
                break;
            }
            string snelheidstring = subElem->GetText();

            try
            {
                voertuig_snelheid = std::stod(snelheidstring);
                if (voertuig_snelheid < 0)
                {
                    errorOutput.handleError("Voertuig snelheid moet positief zijn: " + to_string(voertuig_snelheid), "Parser::parseVoertuigen");
                    geldig = false;
                    break;
                }
            }
            catch (exception&)
            {
                errorOutput.handleError("De snelheid van een voertuig is geen double: " + snelheidstring, "Parser::parseVoertuigen");
                geldig = false;
                break;
            }
        }
    }

    // Als we geen geldige baan of positie hebben, sla over
    if (geldig && voertuig_baan != nullptr)
    {
        // Check of positie binnen baan grenzen valt
        if (positie >= voertuig_baan->getLengte()) {
            errorOutput.handleError("Voertuig positie buiten baan grenzen: " + to_string(positie) + " >= " + to_string(voertuig_baan->getLengte()), "Parser::parseVoertuigen");
            delete voertuig;
            return;
        }

        if (voertuig == nullptr)
            voertuig = Voertuig::createVoertuig("auto"); // Default type

        voertuig->setPositie(positie);
        voertuig->setSnelheid(voertuig_snelheid);
        voertuig->setBaan(voertuig_baan);

        voertuig_baan->addVoertuig(voertuig);
    }
    else
    {
        errorOutput.handleError("Voertuig wordt overgeslagen vanwege ongeldige eigenschappen", "Parser::parseVoertuigen");
        delete voertuig;
    }
}

void Parser::parseVerkeerslichten(TiXmlElement* elem, ErrorOutput& errorOutput)
{
    const char* baan = nullptr;
    int cyclus = -1;
    int positie = -1;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            baan = subElem->GetText();
            if (!baan) {
                errorOutput.handleError("Verkeerslicht heeft geen baan naam", "Parser::parseVerkeerslichten");
                return;
            }
        }
        else if (propertyName == "positie")
        {
            try
            {
                positie = stoi(subElem->GetText());
                if (positie < 0) {
                    errorOutput.handleError("Verkeerslicht positie moet positief zijn: " + to_string(positie), "Parser::parseVerkeerslichten");
                    return;
                }
            }
            catch (exception&)
            {
                errorOutput.handleError("Verkeerslicht positie is geen integer: " + string(subElem->GetText()), "Parser::parseVerkeerslichten");
                return;
            }
        }
        else if (propertyName == "cyclus")
        {
            try
            {
                cyclus = stoi(subElem->GetText());
                if (cyclus <= 0) {
                    errorOutput.handleError("Verkeerslicht cyclus moet positief zijn: " + to_string(cyclus), "Parser::parseVerkeerslichten");
                    return;
                }
            }
            catch (exception&)
            {
                errorOutput.handleError("Er is een verkeerslicht waarvan de cyclus geen integer is: " + string(subElem->GetText()), "Parser::parseVerkeerslichten");
                return;
            }
        }
    }

    if (cyclus < 0 || positie < 0 || !baan)
    {
        errorOutput.handleError("Verkeerslicht heeft ongeldige eigenschappen", "Parser::parseVerkeerslichten");
        return;
    }

    // Check of baan bestaat
    if (banenMap.find(baan) == banenMap.end()) {
        errorOutput.handleError("Baan niet gevonden voor verkeerslicht: " + string(baan), "Parser::parseVerkeerslichten");
        return;
    }

    // Check of positie binnen baan grenzen valt
    if (positie >= banenMap[baan]->getLengte()) {
        errorOutput.handleError("Verkeerslicht positie buiten baan grenzen: " + to_string(positie) + " >= " + to_string(banenMap[baan]->getLengte()), "Parser::parseVerkeerslichten");
        return;
    }

    auto* verkeerslicht = new Verkeerslicht();
    verkeerslicht->setBaan(banenMap[baan]);
    verkeerslicht->setCyclus(cyclus);
    verkeerslicht->setPositie(positie);

    banenMap[baan]->addVerkeerslicht(verkeerslicht);
}

void Parser::parseVoertuiggeneratoren(TiXmlElement* elem, ErrorOutput& errorOutput)
{
    Baan* voertuigbaan = nullptr;
    double frequentie = 0;
    string type;

    bool geldig = true;

    // Verzamel alle benodigde eigenschappen
    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            if (!subElem->GetText())
            {
                exceptionFound(geldig, "Er is een VOERTUIGGENERATOR zonder baan!", errorOutput);
                break;
            }

            // ======== Baan isn't found, dus niet toevoegen aan voertuiggenerators ========
            if (banenMap.find(subElem->GetText()) == banenMap.end())
            {
                exceptionFound(geldig, "Baan niet gevonden voor voertuiggenerator: " + string(subElem->GetText()), errorOutput);
                return;
            }

            voertuigbaan = banenMap[subElem->GetText()];
        }
        else if (propertyName == "frequentie")
        {
            if (!subElem->GetText())
            {
                exceptionFound(geldig, "Er is een voertuiggenerator zonder frequentie!", errorOutput);
                break;
            }

            try
            {
                frequentie = stod(subElem->GetText());
                if (frequentie < 0)
                {
                    exceptionFound(geldig, "Frequentie moet positief zijn: " + to_string(frequentie), errorOutput);
                    break;
                }
            }
            catch (exception&)
            {
                exceptionFound(geldig, "De frequentie van een voertuiggenerator is geen getal: " + string(subElem->GetText()), errorOutput);
                break;
            }
        }
        else if (propertyName == "type")
        {
            if (subElem->GetText())
            {
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
    if (geldig && voertuigbaan != nullptr && !type.empty())
    {
        // Maak de generator met de verzamelde gegevens
        auto* generator = new Voertuiggenerator(voertuigbaan, frequentie, type);

        // Voeg de generator toe aan de simulatie
        voertuigbaan->addVoertuiggenerator(generator);
    }
    else {
        errorOutput.handleError("Voertuiggenerator wordt overgeslagen vanwege ongeldige eigenschappen", "Parser::parseVoertuiggeneratoren");
    }
}

void Parser::parseBushaltes(TiXmlElement* elem, ErrorOutput& errorOutput)
{
    auto* bushalte = new Bushalte();
    bool geldig = true;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            if (!subElem->GetText()) {
                errorOutput.handleError("Bushalte heeft geen baan naam", "Parser::parseBushaltes");
                geldig = false;
                break;
            }

            if (banenMap.find(subElem->GetText()) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor bushalte: " + string(subElem->GetText()), "Parser::parseBushaltes");
                geldig = false;
                break;
            }

            Baan* bushalte_baan = banenMap[subElem->GetText()];
            bushalte->setBaan(bushalte_baan);
        }
        else if (propertyName == "positie")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een bushalte zonder positie!", "Parser::parseBushaltes");
                geldig = false;
                break;
            }
            try
            {
                int positie = stoi(subElem->GetText());
                if (positie < 0) {
                    errorOutput.handleError("Bushalte positie moet positief zijn: " + to_string(positie), "Parser::parseBushaltes");
                    geldig = false;
                    break;
                }
                bushalte->setPositie(positie);
            }
            catch (exception&)
            {
                errorOutput.handleError("Er is een bushalte waarvan de positie geen integer is: " + string(subElem->GetText()), "Parser::parseBushaltes");
                geldig = false;
                break;
            }
        }
        else if (propertyName == "wachttijd")
        {
            if (!subElem->GetText())
            {
                errorOutput.handleError("Er is een bushalte zonder wachttijd!", "Parser::parseBushaltes");
                geldig = false;
                break;
            }
            try
            {
                int wachttijd = stoi(subElem->GetText());
                if (wachttijd < 0) {
                    errorOutput.handleError("Bushalte wachttijd moet positief zijn: " + to_string(wachttijd), "Parser::parseBushaltes");
                    geldig = false;
                    break;
                }
                bushalte->setWachttijd(wachttijd);
            }
            catch (exception&)
            {
                errorOutput.handleError("Er is een bushalte waarvan de wachttijd geen integer is: " + string(subElem->GetText()), "Parser::parseBushaltes");
                geldig = false;
                break;
            }
        }
    }

    if (geldig && bushalte->getBaan() != nullptr)
    {
        // Check of positie binnen baan grenzen valt
        if (bushalte->getPositie() >= bushalte->getBaan()->getLengte()) {
            errorOutput.handleError("Bushalte positie buiten baan grenzen: " + to_string(bushalte->getPositie()) + " >= " + to_string(bushalte->getBaan()->getLengte()), "Parser::parseBushaltes");
            delete bushalte;
            return;
        }

        Baan* baan = bushalte->getBaan();
        baan->addBushalte(bushalte);
    }
    else {
        errorOutput.handleError("Bushalte wordt overgeslagen vanwege ongeldige eigenschappen", "Parser::parseBushaltes");
        delete bushalte;
    }
}

void Parser::parseKruisPunten(TiXmlElement* elem, ErrorOutput& errorOutput)
{
    vector<Baan*> bannen;
    map<Baan*, int> positions;

    for (TiXmlElement* baanElem = elem->FirstChildElement("baan"); baanElem; baanElem = baanElem->NextSiblingElement("baan"))
    {
        // Attributen ophalen
        const char* positieAttr = baanElem->Attribute("positie");
        if (!positieAttr) {
            errorOutput.handleError("Kruispunt baan heeft geen positie attribuut", "Parser::parseKruisPunten");
            continue;
        }

        const char* baanNaam = baanElem->GetText();
        if (!baanNaam) {
            errorOutput.handleError("Kruispunt heeft lege baan naam", "Parser::parseKruisPunten");
            continue;
        }

        try {
            const int positieInt = stoi(positieAttr);

            if (banenMap.find(baanNaam) == banenMap.end()) {
                errorOutput.handleError("Baan niet gevonden voor kruispunt: " + string(baanNaam), "Parser::parseKruisPunten");
                continue;
            }

            if (positieInt < 0) {
                errorOutput.handleError("Kruispunt positie moet positief zijn: " + to_string(positieInt), "Parser::parseKruisPunten");
                continue;
            }

            if (positieInt >= banenMap[baanNaam]->getLengte()) {
                errorOutput.handleError("Kruispunt positie buiten baan grenzen: " + to_string(positieInt) + " >= " + to_string(banenMap[baanNaam]->getLengte()), "Parser::parseKruisPunten");
                continue;
            }

            positions[banenMap[baanNaam]] = positieInt;
            bannen.push_back(banenMap[baanNaam]);

        } catch (exception&) {
            errorOutput.handleError("Kruispunt positie is geen integer: " + string(positieAttr), "Parser::parseKruisPunten");
            continue;
        }
    }

    if (bannen.size() < 2) {
        errorOutput.handleError("Kruispunt moet minstens 2 geldige banen hebben", "Parser::parseKruisPunten");
        return;
    }

    const auto kruispunt = Kruispunt::createNewKruispunt(bannen, positions);

    for (const auto b : kruispunt->getBannen())
        b->addKruispunt(kruispunt);

    for (const auto b : bannen)
    {
        for (const auto v : b->getVerkeerslichten())
        {
            if (positions[b] == v->getPositie())
                v->setKruispunt(kruispunt);
        }
    }
}

bool Parser::parseElements(const std::string& filename, Simulator* sim, ErrorOutput& errorOutput)
{
    if (!sim) {
        errorOutput.handleError("Simulator is nullptr", "Parser::parseElements");
        return false;
    }

    TiXmlDocument doc;

    if (!doc.LoadFile(filename.c_str()))
    {
        errorOutput.handleError("Error loading file: " + string(doc.ErrorDesc()), "Parser::parseElements");
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root)
    {
        errorOutput.handleError("Failed to load file: No root element", "Parser::parseElements");
        return false;
    }

    try {
        // =========== First parse banen because they are essential for the implementation of other elements =============
        for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
                                                                                                 ti_xml_element->NextSiblingElement())
        {
            if (string elementType = ti_xml_element->Value(); elementType != "BAAN")
                continue;
            parseBanen(ti_xml_element, sim, errorOutput);
        }

        // =========== Just then parse the other elements =============
        for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
                                                                                                 ti_xml_element->NextSiblingElement())
        {
            if (string elementType = ti_xml_element->Value(); elementType == "VOERTUIG")
                parseVoertuigen(ti_xml_element, errorOutput);
            else if (elementType == "VOERTUIGGENERATOR")
                parseVoertuiggeneratoren(ti_xml_element, errorOutput);
            else if (elementType == "VERKEERSLICHT")
                parseVerkeerslichten(ti_xml_element, errorOutput);
            else if (elementType == "BUSHALTE")
                parseBushaltes(ti_xml_element, errorOutput);
            else if (elementType == "KRUISPUNT")
                parseKruisPunten(ti_xml_element, errorOutput);
        }

        return true;

    } catch (const std::exception& e) {
        errorOutput.handleException(e, "Parser::parseElements");
        return false;
    }
}

// Backwards compatibility
bool Parser::parseElements(const std::string& filename, Simulator* sim)
{
    // Fallback logger voor oude code
    static Logger fallbackLogger("logs/parser_fallback.txt", true, Logger::LogLevel::ERROR);
    static ErrorOutput fallbackErrorOutput(fallbackLogger);

    return parseElements(filename, sim, fallbackErrorOutput);
}

void Parser::exceptionFound(bool& geldig, const string& message, ErrorOutput& errorOutput)
{
    errorOutput.handleError(message, "Parser");
    geldig = false;
}

bool Parser::geldigeTypen(const string& type)
{
    return type == "auto" || type == "bus" || type == "brandweerwagen" ||
           type == "politiecombi" || type == "ziekenwagen";
}