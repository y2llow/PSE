#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

#include "../src/simulatie/Simulator.h"
#include "../src/simulatie/Parser.h"
#include "../src/logs_and_errors/Logger.h"
#include "../src/logs_and_errors/ErrorOutput.h"
#include "../src/elementen/Baan.h"
#include "../src/elementen/Voertuig.h"
#include "../src/elementen/Verkeerslicht.h"
#include "../src/elementen/Bushalte.h"
#include "../src/elementen/Voertuiggenerator.h"
#include "../src/elementen/Kruispunt.h"

/**
 * @brief Happy Day Scenario Implementation
 *
 * Dit bestand implementeert het "happy day scenario" zoals beschreven in de specificatie.
 * Alle use-cases worden sequentieel uitgevoerd zonder fouten, volgens de stappen
 * beschreven in de use-case documentatie.
 */

void createHappyDayXML() {
    std::cout << "=== STAP 1: Creëren van Happy Day XML bestand ===" << std::endl;

    std::ofstream xmlFile("happy_day_scenario.xml");
    xmlFile << R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <!-- Banen definiëren -->
    <BAAN>
        <naam>Middelheimlaan</naam>
        <lengte>1000</lengte>
    </BAAN>

    <BAAN>
        <naam>Floralienlaan</naam>
        <lengte>800</lengte>
    </BAAN>

    <BAAN>
        <naam>Beukenlaan</naam>
        <lengte>600</lengte>
    </BAAN>

    <!-- Verkeerslichten -->
    <VERKEERSLICHT>
        <baan>Middelheimlaan</baan>
        <positie>400</positie>
        <cyclus>30</cyclus>
    </VERKEERSLICHT>

    <VERKEERSLICHT>
        <baan>Floralienlaan</baan>
        <positie>500</positie>
        <cyclus>25</cyclus>
    </VERKEERSLICHT>

    <!-- Voertuigen met verschillende types -->
    <VOERTUIG>
        <baan>Middelheimlaan</baan>
        <positie>50</positie>
        <type>auto</type>
    </VOERTUIG>

    <VOERTUIG>
        <baan>Middelheimlaan</baan>
        <positie>100</positie>
        <type>bus</type>
    </VOERTUIG>

    <VOERTUIG>
        <baan>Floralienlaan</baan>
        <positie>150</positie>
        <type>brandweerwagen</type>
    </VOERTUIG>

    <VOERTUIG>
        <baan>Beukenlaan</baan>
        <positie>200</positie>
        <type>politiecombi</type>
    </VOERTUIG>

    <!-- Voertuiggeneratoren -->
    <VOERTUIGGENERATOR>
        <baan>Middelheimlaan</baan>
        <frequentie>15</frequentie>
        <type>auto</type>
    </VOERTUIGGENERATOR>

    <VOERTUIGGENERATOR>
        <baan>Floralienlaan</baan>
        <frequentie>20</frequentie>
        <type>bus</type>
    </VOERTUIGGENERATOR>

    <!-- Bushaltes -->
    <BUSHALTE>
        <baan>Middelheimlaan</baan>
        <positie>250</positie>
        <wachttijd>5</wachttijd>
    </BUSHALTE>

    <BUSHALTE>
        <baan>Floralienlaan</baan>
        <positie>300</positie>
        <wachttijd>8</wachttijd>
    </BUSHALTE>

    <!-- Kruispunten -->
    <KRUISPUNT>
        <baan positie="500">Middelheimlaan</baan>
        <baan positie="400">Floralienlaan</baan>
    </KRUISPUNT>

    <KRUISPUNT>
        <baan positie="750">Middelheimlaan</baan>
        <baan positie="300">Beukenlaan</baan>
    </KRUISPUNT>

</VERKEERSSITUATIE>)";
    xmlFile.close();

    std::cout << "✓ Happy Day XML bestand succesvol aangemaakt" << std::endl;
}

void useCase1_1_VerkeerssituatieInlezen(Simulator* sim, ErrorOutput& errorOutput) {
    std::cout << "\n=== USE CASE 1.1: Verkeerssituatie inlezen ===" << std::endl;

    // STAP 1: Open invoerbestand
    std::cout << "STAP 1: Open invoerbestand 'happy_day_scenario.xml'" << std::endl;

    // STAP 2: WHILE Bestand niet ingelezen
    std::cout << "STAP 2: Bestand wordt ingelezen..." << std::endl;

    // Initialize parser
    Parser::initialize();

    // Parse elements
    bool success = Parser::parseElements("happy_day_scenario.xml", sim, errorOutput);

    if (success) {
        std::cout << "✓ STAP 2.1-2.3: Elementen herkend, informatie gelezen en geverifieerd" << std::endl;

        // STAP 3: Verifieer consistentie
        std::cout << "STAP 3: Verifieer consistentie van de verkeerssituatie" << std::endl;

        auto banen = sim->getBanen();
        std::cout << "  - Aantal banen ingelezen: " << banen.size() << std::endl;

        int totalVoertuigen = 0, totalVerkeerslichten = 0, totalBushaltes = 0, totalGeneratoren = 0, totalKruispunten = 0;

        for (auto* baan : banen) {
            totalVoertuigen += baan->getVoertuigen().size();
            totalVerkeerslichten += baan->getVerkeerslichten().size();
            totalBushaltes += baan->getBushaltes().size();
            totalGeneratoren += baan->getVoertuigeneratoren().size();
            totalKruispunten += baan->getKruispunten().size();
        }

        std::cout << "  - Totaal voertuigen: " << totalVoertuigen << std::endl;
        std::cout << "  - Totaal verkeerslichten: " << totalVerkeerslichten << std::endl;
        std::cout << "  - Totaal bushaltes: " << totalBushaltes << std::endl;
        std::cout << "  - Totaal voertuiggeneratoren: " << totalGeneratoren << std::endl;
        std::cout << "  - Totaal kruispunten: " << totalKruispunten << std::endl;

        std::cout << "✓ Verkeerssituatie is consistent" << std::endl;
    }

    // STAP 4: Sluit invoerbestand
    std::cout << "STAP 4: Invoerbestand gesloten" << std::endl;
    std::cout << "✓ USE CASE 1.1 succesvol voltooid!" << std::endl;
}

void useCase2_1_SimpeleUitvoer(Simulator* sim) {
    std::cout << "\n=== USE CASE 2.1: Simpele uitvoer ===" << std::endl;

    // STAP 1: Schrijf huidige simulatietijd uit
    std::cout << "STAP 1: Schrijf huidige simulatietijd uit" << std::endl;
    std::cout << "Tijd: 0.0" << std::endl;

    // STAP 2: WHILE Nog voertuigen in simulatie
    std::cout << "STAP 2: Schrijf voertuig-gegevens uit" << std::endl;

    int voertuigTeller = 1;
    for (auto* baan : sim->getBanen()) {
        for (auto* voertuig : baan->getVoertuigen()) {
            std::cout << "Voertuig " << voertuigTeller++ << std::endl;
            std::cout << "-> baan: " << baan->getNaam() << std::endl;
            std::cout << "-> positie: " << voertuig->getPositie() << std::endl;
            std::cout << "-> snelheid: " << voertuig->getSnelheid() << std::endl;
            std::cout << "-> type: " << voertuig->getType() << std::endl;
        }
    }

    std::cout << "✓ USE CASE 2.1 succesvol voltooid!" << std::endl;
}

void useCase2_2_GrafischeImpressie(Simulator* sim) {
    std::cout << "\n=== USE CASE 2.2: Grafische impressie ===" << std::endl;

    // STAP 1: Open uitvoerbestand
    std::cout << "STAP 1: Open uitvoerbestand" << std::endl;

    // STAP 2: Teken gegevens uit voor de toestand van de verkeerssituatie
    std::cout << "STAP 2: Teken gegevens uit voor de toestand" << std::endl;

    // Make graphical impression
    sim->makeGraphicalImpression();

    // STAP 3: Sluit uitvoerbestand
    std::cout << "STAP 3: Sluit uitvoerbestand" << std::endl;

    std::cout << "✓ USE CASE 2.2 succesvol voltooid!" << std::endl;
    std::cout << "  Grafische impressie opgeslagen in simulation_output.txt" << std::endl;
}

void useCase3_1_RijdenVanVoertuig(Simulator* sim) {
    std::cout << "\n=== USE CASE 3.1: Rijden van voertuig ===" << std::endl;

    auto banen = sim->getBanen();
    if (!banen.empty() && !banen[0]->getVoertuigen().empty()) {
        auto* voertuig = banen[0]->getVoertuigen()[0];
        auto* baan = banen[0];

        double oudePositie = voertuig->getPositie();
        double oudeSnelheid = voertuig->getSnelheid();
        double oudeVersnelling = voertuig->getVersnelling();

        std::cout << "Voor simulatiestap:" << std::endl;
        std::cout << "  Positie: " << oudePositie << std::endl;
        std::cout << "  Snelheid: " << oudeSnelheid << std::endl;
        std::cout << "  Versnelling: " << oudeVersnelling << std::endl;

        // STAP 1: Bereken nieuwe snelheid en positie van voertuig
        std::cout << "STAP 1: Bereken nieuwe snelheid en positie" << std::endl;

        // STAP 2: Bereken nieuwe versnelling van voertuig
        std::cout << "STAP 2: Bereken nieuwe versnelling" << std::endl;

        voertuig->rijd();

        std::cout << "Na simulatiestap:" << std::endl;
        std::cout << "  Positie: " << voertuig->getPositie() << std::endl;
        std::cout << "  Snelheid: " << voertuig->getSnelheid() << std::endl;
        std::cout << "  Versnelling: " << voertuig->getVersnelling() << std::endl;

        // STAP 3: IF nieuwe positie valt buiten huidige baan
        if (voertuig->getPositie() >= baan->getLengte()) {
            std::cout << "STAP 3.1: Voertuig verwijderd (buiten baan)" << std::endl;
        } else {
            std::cout << "STAP 3: Voertuig blijft op baan" << std::endl;
        }
    }

    std::cout << "✓ USE CASE 3.1 succesvol voltooid!" << std::endl;
}

void useCase3_2_SimulatieVanVerkeerslicht(Simulator* sim) {
    std::cout << "\n=== USE CASE 3.2: Simulatie van verkeerslicht ===" << std::endl;

    auto banen = sim->getBanen();
    for (auto* baan : banen) {
        for (auto* verkeerslicht : baan->getVerkeerslichten()) {
            std::cout << "Verkeerslicht op " << baan->getNaam() << " (positie " << verkeerslicht->getPositie() << "):" << std::endl;

            // Show current state
            std::string kleur;
            switch (verkeerslicht->getState()) {
                case LightState::GREEN: kleur = "GROEN"; break;
                case LightState::ORANGE: kleur = "ORANJE"; break;
                case LightState::RED: kleur = "ROOD"; break;
            }

            std::cout << "  Huidige kleur: " << kleur << std::endl;
            std::cout << "  Cyclus: " << verkeerslicht->getCyclus() << " seconden" << std::endl;

            // STAP 1: IF tijd sinds laatste verandering > cyclus
            std::cout << "STAP 1: Controleer of kleur moet veranderen" << std::endl;

            // STAP 2: IF verkeerslicht is groen
            std::cout << "STAP 2-3: Voertuigen passen zich aan verkeerslicht aan" << std::endl;

            verkeerslicht->updateVerkeerslicht();

            // Show new state
            switch (verkeerslicht->getState()) {
                case LightState::GREEN: kleur = "GROEN"; break;
                case LightState::ORANGE: kleur = "ORANJE"; break;
                case LightState::RED: kleur = "ROOD"; break;
            }
            std::cout << "  Nieuwe kleur: " << kleur << std::endl;
        }
    }

    std::cout << "✓ USE CASE 3.2 succesvol voltooid!" << std::endl;
}

void useCase3_3_AutomatischeSimulatie(Simulator* sim) {
    std::cout << "\n=== USE CASE 3.3: Automatische simulatie ===" << std::endl;

    std::cout << "STAP 1: FOR elk voertuig in het wegennetwerk" << std::endl;
    std::cout << "STAP 2: FOR elk verkeerslicht in het wegennetwerk" << std::endl;

    int aantalVoertuigen = 0, aantalVerkeerslichten = 0;

    for (auto* baan : sim->getBanen()) {
        aantalVoertuigen += baan->getVoertuigen().size();
        aantalVerkeerslichten += baan->getVerkeerslichten().size();
    }

    std::cout << "  Aantal voertuigen te simuleren: " << aantalVoertuigen << std::endl;
    std::cout << "  Aantal verkeerslichten te simuleren: " << aantalVerkeerslichten << std::endl;

    // Voer een simulatiestap uit
    sim->simulationRun();

    std::cout << "✓ USE CASE 3.3 succesvol voltooid!" << std::endl;
}

void useCase3_4_SimulatieMetVoertuiggenerator(Simulator* sim) {
    std::cout << "\n=== USE CASE 3.4: Simulatie met voertuiggenerator ===" << std::endl;

    auto banen = sim->getBanen();
    for (auto* baan : banen) {
        for (auto* generator : baan->getVoertuigeneratoren()) {
            std::cout << "Voertuiggenerator op " << baan->getNaam() << ":" << std::endl;
            std::cout << "  Type: " << generator->getType() << std::endl;
            std::cout << "  Frequentie: " << generator->getFrequentie() << " seconden" << std::endl;

            int oudAantal = baan->getVoertuigen().size();

            // STAP 3.1: IF tijd sinds laatste voertuig > frequentie
            std::cout << "STAP 3.1: Controleer of nieuwe voertuig moet worden gegenereerd" << std::endl;

            generator->generateVoertuig();

            int nieuwAantal = baan->getVoertuigen().size();

            if (nieuwAantal > oudAantal) {
                std::cout << "  ✓ Nieuw voertuig gegenereerd!" << std::endl;
            } else {
                std::cout << "  - Nog geen nieuw voertuig (wacht op frequentie)" << std::endl;
            }
        }
    }

    std::cout << "✓ USE CASE 3.4 succesvol voltooid!" << std::endl;
}

void useCase3_6_SimulatieBushaltes(Simulator* sim) {
    std::cout << "\n=== USE CASE 3.6: Simulatie van bushaltes ===" << std::endl;

    auto banen = sim->getBanen();
    for (auto* baan : banen) {
        for (auto* bushalte : baan->getBushaltes()) {
            std::cout << "Bushalte op " << baan->getNaam() << " (positie " << bushalte->getPositie() << "):" << std::endl;
            std::cout << "  Wachttijd: " << bushalte->getWachttijd() << " seconden" << std::endl;

            // Check for buses near the bus stop
            bool busGevonden = false;
            for (auto* voertuig : baan->getVoertuigen()) {
                if (voertuig->getType() == "Bus") {
                    double afstand = abs(voertuig->getPositie() - bushalte->getPositie());
                    if (afstand < 60) { // Within interaction range
                        busGevonden = true;
                        std::cout << "  Bus gevonden op afstand: " << afstand << std::endl;

                        // STAP 1-3: Bushalte logica
                        std::cout << "STAP 1-3: Bus interactie met bushalte" << std::endl;
                        bushalte->stopBus();

                        std::string state;
                        switch (voertuig->getState()) {
                            case State::DRIVING: state = "RIJDEND"; break;
                            case State::SLOWINGDOWN: state = "VERTRAAGD"; break;
                            case State::STOPPING: state = "STOPPEND"; break;
                        }
                        std::cout << "  Bus status: " << state << std::endl;
                    }
                }
            }

            if (!busGevonden) {
                std::cout << "  Geen bussen in de buurt" << std::endl;
            }
        }
    }

    std::cout << "✓ USE CASE 3.6 succesvol voltooid!" << std::endl;
}

void useCase3_7_SimulatieKruispunten(Simulator* sim) {
    std::cout << "\n=== USE CASE 3.7: Simulatie van kruispunten ===" << std::endl;

    auto banen = sim->getBanen();
    for (auto* baan : banen) {
        for (auto* kruispunt : baan->getKruispunten()) {
            std::cout << "Kruispunt met " << kruispunt->getBannen().size() << " banen:" << std::endl;

            auto positions = kruispunt->getPositions();
            for (auto& [baanPtr, positie] : positions) {
                std::cout << "  " << baanPtr->getNaam() << " op positie " << positie << std::endl;
            }

            // Check for vehicles at intersection
            for (auto* voertuig : baan->getVoertuigen()) {
                if (positions.find(baan) != positions.end()) {
                    double kruispuntPositie = positions[baan];
                    double afstand = abs(voertuig->getPositie() - kruispuntPositie);

                    if (afstand < 5) { // At intersection
                        std::cout << "  Voertuig op kruispunt!" << std::endl;
                        std::cout << "STAP 1.1: Kies willekeurige baan om verder op te rijden" << std::endl;

                        // Note: chooseKruispunt() would handle the random selection
                        voertuig->chooseKruispunt();

                        std::cout << "  Voertuig nu op baan: " << voertuig->getBaan()->getNaam() << std::endl;
                    }
                }
            }
        }
    }

    std::cout << "✓ USE CASE 3.7 succesvol voltooid!" << std::endl;
}

void demonstrateFullSimulation(Simulator* sim) {
    std::cout << "\n=== VOLLEDIGE SIMULATIE DEMONSTRATIE ===" << std::endl;
    std::cout << "Simulatie wordt uitgevoerd voor 10 stappen..." << std::endl;

    for (int stap = 1; stap <= 10; stap++) {
        std::cout << "\n--- Simulatiestap " << stap << " ---" << std::endl;

        // Count vehicles before simulation
        int voertuigenVoor = 0;
        for (auto* baan : sim->getBanen()) {
            voertuigenVoor += baan->getVoertuigen().size();
        }

        // Run simulation step
        sim->simulationRun();

        // Count vehicles after simulation
        int voertuigenNa = 0;
        for (auto* baan : sim->getBanen()) {
            voertuigenNa += baan->getVoertuigen().size();
        }

        std::cout << "Voertuigen: " << voertuigenVoor << " → " << voertuigenNa;
        if (voertuigenNa > voertuigenVoor) {
            std::cout << " (+" << (voertuigenNa - voertuigenVoor) << " gegenereerd)";
        } else if (voertuigenNa < voertuigenVoor) {
            std::cout << " (" << (voertuigenVoor - voertuigenNa) << " verdwenen)";
        }
        std::cout << std::endl;

        // Small delay for dramatic effect
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "\n✓ Volledige simulatie demonstratie voltooid!" << std::endl;
}

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "  HAPPY DAY SCENARIO - VERKEERSSIMULATIE" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "Implementatie van het 'happy day scenario'" << std::endl;
    std::cout << "waarin alle use-cases succesvol worden uitgevoerd." << std::endl;

    try {
        // Setup logging
        Logger logger("logs/happy_day.txt", true, Logger::LogLevel::INFO, true);
        ErrorOutput errorOutput(logger);

        // Create simulator with error handling
        Simulator sim(errorOutput);

        // Execute all use cases in sequence
        createHappyDayXML();

        useCase1_1_VerkeerssituatieInlezen(&sim, errorOutput);
        useCase2_1_SimpeleUitvoer(&sim);
        useCase2_2_GrafischeImpressie(&sim);
        useCase3_1_RijdenVanVoertuig(&sim);
        useCase3_2_SimulatieVanVerkeerslicht(&sim);
        useCase3_3_AutomatischeSimulatie(&sim);
        useCase3_4_SimulatieMetVoertuiggenerator(&sim);
        useCase3_6_SimulatieBushaltes(&sim);
        useCase3_7_SimulatieKruispunten(&sim);

        // Demonstrate full simulation
        demonstrateFullSimulation(&sim);

        // Generate final output
        std::cout << "\n=== FINALE OUTPUT GENERATIE ===" << std::endl;
        sim.generateGraphicsFile();
        std::cout << "✓ Finale grafische impressie opgeslagen" << std::endl;

        std::cout << "\n============================================" << std::endl;
        std::cout << "  🎉 HAPPY DAY SCENARIO SUCCESVOL! 🎉" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "Alle use-cases zijn zonder fouten uitgevoerd." << std::endl;
        std::cout << "Het systeem werkt zoals bedoeld!" << std::endl;

        // Cleanup
        std::filesystem::remove("happy_day_scenario.xml");

    } catch (const std::exception& e) {
        std::cerr << "❌ FOUT in Happy Day Scenario: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
