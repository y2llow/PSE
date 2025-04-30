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

using namespace std;

class Simulator {
    double current_time = 0;
    vector<Baan*> banen;

    string graphical_impression;

public:
    Simulator() = default;
    bool is_consistent = true;


    void addBaan(Baan* b);
    [[nodiscard]] const vector<Baan*> getBanen() const;

    void setConsistency(const bool c) { is_consistent = c; }
    void geldigeTypen(const string& type);

    void simulationRun();
    void makeGraphicalImpression();
    void generateGraphicsFile() const;

    void print();
    void printStatus(Voertuig const* voertuig) const;

    void simulate(int times);

};


#endif //SIMULATION_H
