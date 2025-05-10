//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//


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


void Parser::parseBanen(TiXmlElement* elem, Simulator* sim)
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
                cerr << "Er is een baan zonder naam!" << endl;
                geldig = false;
                break;
            }
            baan->setNaam(subElem->GetText());
        }
        else if (propertyName == "lengte")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een baan zonder lengte!" << endl;
                geldig = false;
                break;
            }
            try
            {
                baan->setLengte(stoi(subElem->GetText()));
            }
            catch (exception&)
            {
                cerr << "De lengte van een baan is geen integer!" << endl;
                geldig = false;
                break;
            }
        }
    }

    if (geldig)
    {
        assert(!baan->getNaam().empty());
        assert(baan->getLengte() > 0);

        sim->addBaan(baan);
        banenMap[baan->getNaam()] = baan;
    }
    else delete baan;
}

void Parser::parseVoertuigen(TiXmlElement* elem)
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
                cerr << "Er is een voertuig zonder baan!" << endl;
                geldig = false;
                break;
            }

            if (banenMap.find(subElem->GetText()) == banenMap.end())
            {
                cerr << "Baan niet gevonden voor voertuig!" << endl;
                geldig = false;
                break;
            }
            voertuig_baan = banenMap[subElem->GetText()];
        }
        else if (propertyName == "positie")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een voertuig zonder positie!" << endl;
                geldig = false;
                break;
            }

            try
            {
                positie = stoi(subElem->GetText());
                if (positie < 0)
                {
                    cerr << "Positie moet positief zijn!" << endl;
                    geldig = false;
                    break;
                }
            }
            catch (exception&)
            {
                cerr << "Er is een voertuig waarvan de positie geen integer is!" << endl;
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
            }
        }

        else if (propertyName == "snelheid")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een voertuig zonder snelheid!" << endl;
                geldig = false;
                break;
            }
            string snelheidstring = subElem->GetText();

            try
            {
                voertuig_snelheid = std::stod(snelheidstring);
                if (voertuig_snelheid < 0)
                {
                    cerr << "Snelheid moet positief zijn!" << endl;
                    geldig = false;
                    break;
                }
            }
            catch (exception&)
            {
                cerr << "De snelheid van een voertuig is geen double!" << endl;
                geldig = false;
                break;
            }
        }
    }

    // Als we geen geldige baan of positie hebben, sla over
    if (geldig && voertuig_baan != nullptr)
    {
        if (voertuig == nullptr)
            voertuig = Voertuig::createVoertuig("UNKNOWN");

        voertuig->setPositie(positie);
        voertuig->setSnelheid(voertuig_snelheid);
        voertuig->setBaan(voertuig_baan);

        voertuig_baan->addVoertuig(voertuig);

        // REQUIRE and ENSURE checks
        assert(voertuig != nullptr);
        assert(voertuig_baan != nullptr);
        assert(positie >= 0);
        assert(voertuig->getPositie() < voertuig_baan->getLengte());
        assert(voertuig_snelheid >= 0);
    }
    else
    {
        cerr << "Voertuig wordt overgeslagen vanwege ongeldige baan of positie." << endl;
        delete voertuig;
    }
}

void Parser::parseVerkeerslichten(TiXmlElement* elem)
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
        }
        else if (propertyName == "positie")
        {
            try
            {
                positie = stoi(subElem->GetText());
            }
            catch (exception&)
            {
            }
        }
        else if (propertyName == "cyclus")
        {
            try
            {
                cyclus = stoi(subElem->GetText());
            }
            catch (exception&)
            {
                cerr << "Er is een verkeerslicht waarvan de cyclus geen integer is!" << endl;
                return;
            }
        }
    }


    if (cyclus < 0)
    {
        cerr << "Er is een verkeerslicht waarvan de cyclus ongeldig is!" << endl;
        return;
    }

    auto* verkeerslicht = new Verkeerslicht();
    verkeerslicht->setBaan(banenMap[baan]);
    verkeerslicht->setCyclus(cyclus);
    verkeerslicht->setPositie(positie);

    banenMap[baan]->addVerkeerslicht(verkeerslicht);

}

void Parser::parseVoertuiggeneratoren(TiXmlElement* elem)
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
                exceptionFound(geldig, "Er is een VOERTUIGGENERATOR zonder baan!");
                break;
            }

            // ======== Baan isn't found, dus niet toevoegen aan voertuiggenerators ========
            if (banenMap.find(subElem->GetText()) == banenMap.end())
            {
                exceptionFound(geldig, "Baan niet gevonden voor voertuiggenerator!");
                return;
            }

            voertuigbaan = banenMap[subElem->GetText()];
        }
        else if (propertyName == "frequentie")
        {
            if (!subElem->GetText())
            {
                exceptionFound(geldig, "Er is een voertuiggenerator zonder frequentie!");
                break;
            }

            try
            {
                frequentie = stod(subElem->GetText());
                if (frequentie < 0)
                {
                    exceptionFound(geldig, "Frequentie moet positief zijn!");
                    break;
                }
            }
            catch (exception&)
            {
                exceptionFound(geldig, "De frequentie van een voertuiggenerator is geen getal!");
                break;
            }
        }
        else if (propertyName == "type")
        {
            if (subElem->GetText())
            {
                type = subElem->GetText();
            }
        }
    }

    // Controleer of alle verplichte eigenschappen zijn ingesteld
    if (geldig)
    {
        // Maak de generator met de verzamelde gegevens
        auto* generator = new Voertuiggenerator(voertuigbaan, frequentie, type);

        // Voeg de generator toe aan de simulatie
        voertuigbaan->addVoertuiggenerator(generator);
    }
}

void Parser::parseBushaltes(TiXmlElement* elem)
{
    auto* bushalte = new Bushalte();
    bool geldig = true;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            // ======= Elke bushalte staat op een bestaande baan =======
            // if (banenMap.find(subElem->GetText()) == banenMap.end())

            Baan* bushalte_baan = banenMap[subElem->GetText()];
            bushalte->setBaan(bushalte_baan);
        }
        else if (propertyName == "positie")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een verkeerslicht zonder positie!" << endl;
                geldig = false;
                break;
            }
            try
            {
                int positie = stoi(subElem->GetText());
                bushalte->setPositie(positie);
                geldig = positie >= 0; // Positie moet positief zijn
            }
            catch (exception&)
            {
                cerr << "Er is een bushalte waarvan de positie geen integer is!" << endl;
                geldig = false;
                break;
            }
        }
        else if (propertyName == "wachttijd")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een bushalte zonder wachttijd!" << endl;
                geldig = false;
                break;
            }
            try
            {
                bushalte->setWachttijd(stoi(subElem->GetText()));
                geldig = stoi(subElem->GetText()) >= 0; // Cyclus moet positief zijn
            }
            catch (exception&)
            {
                cerr << "Er is een bushalte waarvan de wachttijd geen integer is!" << endl;
                geldig = false;
                break;
            }
        }
    }
    if (geldig)
    {
        Baan* baan = bushalte->getBaan();
        baan->addBushalte(bushalte);

    }
}

void Parser::parseKruisPunten(TiXmlElement* elem)
{
    vector<pair<int, Baan*>> kruisPunten;

    vector<Baan*> bannen;
    map<Baan*, int> positions;

    vector<int> positions_int;

    for (TiXmlElement* baanElem = elem->FirstChildElement(
             "baan"); baanElem; baanElem = baanElem->NextSiblingElement("baan"))
    {
        // Attributen ophalen
        const string positie = baanElem->Attribute("positie");
        const int positieInt = stoi(positie);

        const string baanNaam = baanElem->GetText();

        // ========== Here we're not told to make het verkeersituatie inconsistent ============
        // ====== so we're just gonna parse other things and ignore this if it's invalid ======
        if (banenMap.find(baanNaam) == banenMap.end())
            continue;
        if (positieInt > banenMap[baanNaam]->getLengte())
            continue;

        positions[banenMap[baanNaam]] = positieInt;
        bannen.push_back(banenMap[baanNaam]);

        positions_int.push_back(positieInt);
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


bool Parser::parseElements(const std::string& filename, Simulator* sim)
{
    TiXmlDocument doc;

    if (!doc.LoadFile(filename.c_str()))
    {
        cerr << "Error loading file: " << doc.ErrorDesc() << endl;
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root)
    {
        cerr << "Failed to load file: No root element." << endl;
        return false;
    }

    // =========== First parse banen because they are essential for the implementation of other elements =============
    for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
         ti_xml_element->NextSiblingElement())
    {
        if (string elementType = ti_xml_element->Value(); elementType != "BAAN")
            continue;
        parseBanen(ti_xml_element, sim);
    }

    // =========== Just then parse the other elements =============
    for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
         ti_xml_element->NextSiblingElement())
    {
        if (string elementType = ti_xml_element->Value(); elementType == "VOERTUIG")
            parseVoertuigen(ti_xml_element);
        else if (elementType == "VOERTUIGGENERATOR")
            parseVoertuiggeneratoren(ti_xml_element);
        else if (elementType == "VERKEERSLICHT")
            parseVerkeerslichten(ti_xml_element);
        else if (elementType == "BUSHALTE")
            parseBushaltes(ti_xml_element);
        else if (elementType == "KRUISPUNT")
            parseKruisPunten(ti_xml_element);
    }


    // Parse alle elementen in aparte functies
    // sim->sortVoertuigenByPosition();
    // sim->sortVerkeersLichtByPosition();
    return true;
}

void Parser::exceptionFound(bool& geldig, const string& message)
{
    cerr << message << endl;
    geldig = false;
}
