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

class Simulator {
    double current_time = 0;
    vector<Baan*> banen;

    string graphical_impression;
    bool isConsistent = true;

public:
    void addBaan(Baan* b);
    [[nodiscard]] const vector<Baan*> getBanen() const;

    void setConsistency(const bool c) { isConsistent = c; }
    void geldigeTypen(const string& type);

    void simulationRun();
    void makeGraphicalImpression();
    void generateGraphicsFile();

    void print();
    void printStatus(Voertuig const* voertuig) const;

    void simulate(int times);



    //2.2
    //de simulatie grafisch gegeven (als string)
    // SimPrinter simPrinter;
    SimPrinter simPr;

public:
//    simulation(SimPrinter*): simPrinter(SimPrinter()) {
//    };
    bool is_consistent = true;

    Simulator() = default;
    int getVoertuigLastId()const;
    void increaseVoertuigLastId();

    [[nodiscard]] SimPrinter * getSimPrinter() const;

    void setSimPrinter(SimPrinter * const sim_printer);



    [[nodiscard]] vector<Voertuig *> getVoertuigen() const;

    [[nodiscard]] vector<Verkeerslicht *> getVerkeerslichten() const;

    [[nodiscard]] vector<Bushalte *> getBushaltes() const;

    [[nodiscard]] vector<Voertuiggenerator *> getVoertuiggeneratoren() const;


    void ToString();

    double getSimulationTime() const;

    double getincSimulationTime() const;

    double incSimulationTime();

    double UpdateSimulationTime() const;


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
    void addVoertuiggenerator(Voertuiggenerator *v);

    //dit maakt de simulatie string
    void generateSimulation();

    //dit zal de simulatie string na elk tijd updaten
    void updateSimulation();

    void voertuigenGenereren();


};


#endif //SIMULATION_H
