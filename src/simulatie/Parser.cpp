//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//


#include "Parser.h"

#include "../elementen/Voertuiggenerator.h"
#include "../elementen/Verkeerslicht.h"
#include "../elementen/Bushalte.h"
#include "../elementen/Baan.h"
#include "../DesignByContract.h"

#include <iostream>

#include "../elementen/Constants.h"

map<string, Baan*> Parser::banenMap;


void Parser::parseBanen(TiXmlElement* elem, Simulator* sim)
{
    const char* name = nullptr;
    int length = -1;

    // Loop through sub-elements to get properties
    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "naam")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een baan zonder naam!" << endl;
                return;
            }

            name = subElem->GetText();
        }
        else if (propertyName == "lengte")
        {
            if (!subElem->GetText())
            {
                cerr << "Er is een baan zonder lengte!" << endl;
                return;
            }
            try
            {
                length = stoi(subElem->GetText());
            }
            catch (exception&)
            {
                cerr << "De lengte van een baan is geen integer!" << endl;
                return;
            }
        }
    }

    if (length > 0 && name != nullptr)
    {
        const auto baan = new Baan();
        baan->setNaam(name);
        baan->setLengte(length);
        sim->addBaan(baan);

        banenMap[name] = baan;
    }
    else
    {
        cerr << "Er is een baan met een ongeldige lengte of naam!" << endl;
    }
}

void Parser::parseVoertuigen(TiXmlElement* elem, Simulator* sim)
{
    const char* type = nullptr;
    const char* voertuig_baan = nullptr;
    // if the voertuig in the xml file doesn't have the "snelheid" element, then it begins from this value
    double voertuig_snelheid = 0;
    int positie = -1;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            voertuig_baan = subElem->GetText();
        }
        else if (propertyName == "positie")
        {
            try
            {
                positie = stoi(subElem->GetText());
            }
            catch (exception&)
            {
                REQUIRE(false,
                        "Verkeersituatie is inconsistent! De positie van een voertuig is ongeldig.");
            }
        }

        else if (propertyName == "type")
        {
            type = subElem->GetText();
        }

        else if (propertyName == "snelheid")
        {
            try
            {
                voertuig_snelheid = std::stod(subElem->GetText());
            }
            catch (exception&)
            {
                cerr << "De snelheid van een voertuig is geen double!" << endl;
                return;;
            }
        }
    }

    // Als we geen geldige baan of positie hebben, sla over
    REQUIRE(voertuig_baan != nullptr && banenMap.find(voertuig_baan) != banenMap.end(),
        "Verkeersituatie is inconsistent. Een van de voertuigen heeft geen bestaande baan.");

    REQUIRE(type != nullptr && geldigeTypen(type),
            "Verkeersituatie is inconsistent! De type van een voertuig is ongeldig.");

    REQUIRE(positie < banenMap[voertuig_baan]->getLengte() && positie >= 0,
            "Verkeersituatie is inconsistent! Een van de voertuigen heeft een ongeldige positie.");

    if (voertuig_snelheid < 0)
    {
        //cout << "De snelheid van een auto kan niet negatief zijn! New Value=0" << endl;
        voertuig_snelheid = 0;
    }

    const auto voertuig = Voertuig::createVoertuig(type);
    voertuig->setPositie(positie);
    voertuig->setSnelheid(voertuig_snelheid);
    voertuig->setBaan(banenMap[voertuig_baan]);
    banenMap[voertuig_baan]->addVoertuig(voertuig);
}

void Parser::parseVerkeerslichten(TiXmlElement* elem, Simulator* sim)
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
                REQUIRE(false,
                        "Verkeersituatie is inconsistent! De positie van een verkeerslicht is ongeldig.");
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


    REQUIRE(positie >= 0 && positie < banenMap[baan]->getLengte(),
            "Verkeersituatie is inconsistent! De positie van een van de verkeerslichten is ongeldig.");
    REQUIRE(baan != nullptr && (banenMap.find(baan) == banenMap.end()) == false,
            "Verkeersituatie is inconsistent! Een van de verkeerslichten heeft geen bestaande baan.");
    if (cyclus < 0)
    {
        cerr << "Er is een verkeerslicht waarvan de cyclus ongeldig is!" << endl;
        return;
    }

    auto* verkeerslicht = new Verkeerslicht();
    verkeerslicht->setBaan(banenMap[baan]);
    verkeerslicht->setCyclus(cyclus);
    verkeerslicht->setOranjeCyclus(cyclus*0.1);
    verkeerslicht->setPositie(positie);

    banenMap[baan]->addVerkeerslicht(verkeerslicht);
}

void Parser::parseVoertuiggeneratoren(TiXmlElement* elem, Simulator* sim)
{
    const char* voertuigbaan = nullptr;
    const char* type = nullptr;
    double frequentie = -1;


    // Verzamel alle benodigde eigenschappen
    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            voertuigbaan = subElem->GetText();
        }
        else if (propertyName == "frequentie")
        {
            try
            {
                frequentie = stod(subElem->GetText());
            }
            catch (exception&)
            {
                cerr << "De frequentie van een voertuiggenerator is geen getal!" << endl;
                break;
            }
        }
        else if (propertyName == "type")
        {
            type = subElem->GetText();
        }
    }

    // Controleer of alle verplichte eigenschappen zijn ingesteld
    REQUIRE(type != nullptr && geldigeTypen(type),
            "Verkeersituatie is inconsistent! De type van een voertuiggenerator is ongeldig.");
    REQUIRE(voertuigbaan != nullptr && banenMap.find(voertuigbaan) != banenMap.end(),
            "Verkeersituatie is inconsistent. Een van de voertuiggeneratoren heeft geen bestaande baan.");
    if (frequentie < 0)
    {
        cerr << "De frequentie van een voertuiggenerator is ongeldig!" << endl;
        return;
    }

    // Maak de generator met de verzamelde gegevens
    auto* generator = new Voertuiggenerator(banenMap[voertuigbaan], frequentie, type);
    // Voeg de generator toe aan de simulatie
    banenMap[voertuigbaan]->addVoertuiggenerator(generator);
}

void Parser::parseBushaltes(TiXmlElement* elem, Simulator* sim)
{
    int positie = -1;
    int wachttijd = -1;
    const char* baan = nullptr;

    for (TiXmlElement* subElem = elem->FirstChildElement(); subElem != nullptr;
         subElem = subElem->NextSiblingElement())
    {
        if (string propertyName = subElem->Value(); propertyName == "baan")
        {
            // ======= Elke bushalte staat op een bestaande baan =======
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
                REQUIRE(false,
                        "Verkeersituatie is inconsistent! De positie van een bushalte is ongeldig.");
            }
        }
        else if (propertyName == "wachttijd")
        {
            try
            {
                wachttijd = stoi(subElem->GetText());
            }
            catch (exception&)
            {
                cerr << "Er is een bushalte waarvan de wachttijd geen integer is!" << endl;
                break;
            }
        }
    }

    Baan* bushalte_baan = banenMap[baan];

    REQUIRE(positie >= 0 && positie < bushalte_baan->getLengte(),
            "Verkeersituatie is inconsistent! De positie van een van de bushaltes is ongeldig.");
    REQUIRE(baan != nullptr && banenMap.find(baan) != banenMap.end(),
            "Verkeersituatie is inconsistent. Een van de bushaltes heeft geen bestaande baan.");
    if (wachttijd < 0)
    {
        cerr << "De wachttijd van een bushalte is ongeldig" << endl;
        return;
    }

    auto* bushalte = new Bushalte();
    bushalte->setWachttijd(wachttijd);
    bushalte->setBaan(bushalte_baan);
    bushalte_baan->addBushalte(bushalte);
    bushalte->setPositie(positie);
}

void Parser::parseKruisPunten(TiXmlElement* elem, Simulator* sim)
{
    vector<pair<int, Baan*>> kruisPunten;

    for (TiXmlElement* baanElem = elem->FirstChildElement(
             "baan"); baanElem; baanElem = baanElem->NextSiblingElement("baan"))
    {
        // Attributen ophalen
        const string positie = baanElem->Attribute("positie");
        const int positieInt = stoi(positie);

        const string baanNaam = baanElem->GetText();

        // ========== Here we're not told to make het verkeersituatie inconsistent ============
        // ====== so we're just gonna parse other things and ignore this if it's invalid ======
        if (banenMap.find(baanNaam) == banenMap.end() || positieInt > banenMap[baanNaam]->getLengte())
            continue;

        pair<int, Baan*> kruis_punt;
        kruis_punt.first = positieInt;
        kruis_punt.second = banenMap[baanNaam];
        kruisPunten.push_back(kruis_punt);
    }

    for (auto [position, baan] : kruisPunten)
    {
        for (auto [position_2, baan_2] : kruisPunten)
        {
            if (baan_2 != baan)
                baan->addKruispunt(position, baan_2);
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
//        cerr << "Failed to load file: No root element." << endl;
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

    // =========== Only then parse the other elements =============
    assert(sim->getBanen().size() > 0);
    for (auto ti_xml_element = root->FirstChildElement(); ti_xml_element != nullptr; ti_xml_element =
         ti_xml_element->NextSiblingElement())
    {
        if (string elementType = ti_xml_element->Value(); elementType == "VOERTUIG")
            parseVoertuigen(ti_xml_element, sim);
        else if (elementType == "VOERTUIGGENERATOR")
            parseVoertuiggeneratoren(ti_xml_element, sim);
        else if (elementType == "VERKEERSLICHT")
            parseVerkeerslichten(ti_xml_element, sim);
        else if (elementType == "BUSHALTE")
            parseBushaltes(ti_xml_element, sim);
        else if (elementType == "KRUISPUNT")
            parseKruisPunten(ti_xml_element, sim);
    }
    VerkeerslichtenOpKruispunten();


    // Checking the consistency of verkeerslichten
    for (const auto b : sim->getBanen())
    {
        b->sortVerkeerslichtenByPosition();
        for (int i = 0, n = b->getVerkeerslichten().size(); i < n - 1; i++)
        {
            REQUIRE(
                abs(b->getVerkeerslichten()[i]->getPositie() - b->getVerkeerslichten()[i+1]->getPositie()) >
                VERTRAAG_AFSTAND,
                "Verkeerssituatie is niet consistent! Een verkeerslicht mag zich niet in de vertraagafstand van een ander verkeerslicht bevinden");
        }
    }

    return true;
}



void Parser::VerkeerslichtenOpKruispunten()
{
    for (auto& banenPair : banenMap)
    {
        Baan* currentbaan = banenPair.second;

        Verkeerslicht* verkeerslichtKP1 = nullptr;
        Verkeerslicht* verkeerslichtKP2 = nullptr;

        if (currentbaan->getVerkeerslichten().empty() || currentbaan->getKruispunten().empty())
            break;

        auto kruispunten = currentbaan->getKruispunten();

        for (auto& KP1 : kruispunten)
        {
            // Zoek verkeerslicht op currentbaan dat matcht met KP1
            for (Verkeerslicht* licht : currentbaan->getVerkeerslichten())
            {
                if ((licht->getPositie() == KP1.first))
                {
                    verkeerslichtKP1 = licht;
                    break; // Stop na eerste match
                }
            }

            if (!verkeerslichtKP1) continue;

            for (auto& KP2 : KP1.second[0]->getKruispunten())
            {
                // Zoek verkeerslicht op de verbonden baan dat matcht met KP2
                for (Verkeerslicht* licht2 : KP1.second[0]->getVerkeerslichten())
                {
                    if (licht2->getPositie() == KP2.first)
                    {
                        verkeerslichtKP2 = licht2;
                        break; // Stop na eerste match
                    }
                }

                // Als beide lichten gevonden zijn en verschillend zijn verander kleur en cyclus
                if (verkeerslichtKP2 != nullptr && verkeerslichtKP1 != verkeerslichtKP2)
                {
                    verkeerslichtKP2->setCyclus(verkeerslichtKP1->getCyclus());

                    if (verkeerslichtKP1->isGroen() == verkeerslichtKP2->isGroen()) { verkeerslichtKP2->switchColor(); }

                    verkeerslichtKP2->OpKruisPunt();
                    verkeerslichtKP1->OpKruisPunt();

                    verkeerslichtKP2->KruispuntPair(verkeerslichtKP2,verkeerslichtKP1);
                    verkeerslichtKP1->KruispuntPair(verkeerslichtKP1,verkeerslichtKP2);
                }
            }
        }
    }
}

bool Parser::geldigeTypen(const string& type)
{
    return type == "auto" || type == "bus" || type == "brandweerwagen" || type == "politiecombi" || type ==
        "ziekenwagen";
}
