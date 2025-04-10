//
// Created by eraya on 13/03/2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "../../src/elementen/Baan.h"
#include "../../src/elementen/Voertuig.h"
#include "../../src/elementen/Verkeerslicht.h"
#include "../../src/elementen/Voertuiggenerator.h"
#include "../../src/simulatie/SimPrinter.h"
#include "../../src/elementen/Constants.h"

using namespace std;

class simulation {
    // Vectors to store objects created from XML
    vector<Baan *> banen;
    vector<Voertuig *> voertuigen;
    vector<Verkeerslicht *> verkeerslichten;
    vector<Voertuiggenerator *> voertuiggeneratoren;
    double simulationTime = 0;
    double simulationincreasedTime = 0;
    double lastGeneretedVoertuigTime = 0;
    int voertuigLastId = 1;
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
    void addBaan(Baan *b);
    void addVoertuiggenerator(Voertuiggenerator *v);


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
