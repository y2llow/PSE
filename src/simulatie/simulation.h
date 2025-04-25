//
// Created by eraya on 13/03/2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "../../src/elementen/Baan.h"
#include "../../src/elementen/Voertuig.h"
#include "../../src/elementen/Verkeerslicht.h"
#include "../../src/elementen/Bushalte.h"
#include "../../src/elementen/Voertuiggenerator.h"
#include "../../src/simulatie/SimPrinter.h"

using namespace std;

class simulation {
    // Vectors to store objects created from XML
    vector<Baan *> banen;
    vector<Voertuig *> voertuigen;
    vector<Verkeerslicht *> verkeerslichten;
    vector<Bushalte *> bushaltes;
    vector<Voertuiggenerator *> voertuiggeneratoren;

    //dit bevant gewoon vanaf wanneer begint de lijn van de voertuigen en van de verkeerslichten op de baan voor het genereren van grafische simulatie
    pair<int,int> vtXvlIndex;
    map<Baan*,map<int,Voertuig*>> voertuigenOpBaanSIM;
    map<Baan*, map<int, Verkeerslicht*>> verkeerslichtenOpBaanSIM;
    int simulatieSchaal = 5;
    double simulationTime = 0;
    double simulationincreasedTime = 0;
    double lastGeneretedVoertuigTime = 0;
    int voertuigLastId = 1;

    //de simulatie grafisch gegeven (als string)
    vector<string> Gsim;

    // SimPrinter simPrinter;

public:
//    simulation(SimPrinter*): simPrinter(SimPrinter()) {
//    };

    simulation();
    int getVoertuigLastId()const;
    void increaseVoertuigLastId();

    [[nodiscard]] SimPrinter * getSimPrinter() const;

    void setSimPrinter(SimPrinter * const sim_printer);


    [[nodiscard]] vector<Baan *> getBanen() const;

    [[nodiscard]] vector<Voertuig *> getVoertuigen() const;

    [[nodiscard]] vector<Verkeerslicht *> getVerkeerslichten() const;

    [[nodiscard]] vector<Bushalte *> getBushaltes() const;

    [[nodiscard]] vector<Voertuiggenerator *> getVoertuiggeneratoren() const;

    [[nodiscard]] bool isConsistent() const;

    void ToString();

    double getSimulationTime() const;

    double getincSimulationTime() const;

    double incSimulationTime();

    double UpdateSimulationTime() const;

    void simulationRun();

    void sortVoertuigenByPosition();

    void sortVerkeersLichtByPosition();

    void berekenPositie(Voertuig *v) const;

    void BerekenVersnelling(Voertuig *v, int counter) const;

    void BerekenSnelheidNaVertraging(Voertuig *v);

    void BerekenSnelheidNaVersnelling(Voertuig *v);

    void updateVoertuig(Voertuig *V, int counter) const;

    bool isVoertuigOpBaan(const Voertuig *v);

    void updateVoertuigAanVerkeerslichtSituatie(Verkeerslicht *l, int VerkeerslichtCounter);

    bool isVoertuigInVertraagZone(Voertuig *v, Verkeerslicht *l);

    bool isVoertuigInStopZone(Voertuig *v, Verkeerslicht *l);

    vector<Voertuig *> voertuigenTussenVerkeerslichten(Verkeerslicht *lichtVoor, Verkeerslicht *lichtAchter);

    vector<Verkeerslicht *> verkeerslichtenOpBaan(Verkeerslicht *licht);

    void addVoertuig(Voertuig *v);
    void addVerkeerslicht(Verkeerslicht *v);
    void addBushalte(Bushalte *b);
    void addBaan(Baan *b);
    void addVoertuiggenerator(Voertuiggenerator *v);

    //dit maakt de simulatie string
    void generateSimulation();

    //dit zal de simulatie string na elk tijd updaten
    void updateSimulation();

    //dit zal de simulatie printen
    void printSimulation();

    void voertuigenGenereren();

    // Removing the created pointers
    ~simulation() {
        for (const Baan *b: banen)
            delete b;

        for (const Voertuig *v: voertuigen)
            delete v;

        for (const Voertuiggenerator *v: voertuiggeneratoren)
            delete v;

        for (const Verkeerslicht *v: verkeerslichten)
            delete v;
    }

};


#endif //SIMULATION_H
