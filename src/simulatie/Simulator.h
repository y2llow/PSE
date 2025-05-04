//
// Created by eraya on 13/03/2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "../../src/elementen/Baan.h"
#include "../../src/elementen/Voertuig.h"

using namespace std;

class Simulator {
    double current_time = 0;
    vector<Baan*> banen;

    string graphical_impression;
    map<string, string> banen_3d_content;


public:
    Simulator() = default;
    bool is_consistent = true;


    void addBaan(Baan* b);
    [[nodiscard]] const vector<Baan*> getBanen() const;

    void setConsistency(const bool c) { is_consistent = c; }

    void simulationRun();
    void makeGraphicalImpression();
    void generateGraphicsFile() const;

    void print();
    static void printStatus(Voertuig const* voertuig);

    void simulate(int times);

    void generate3dfile(const string& baan);



};


#endif //SIMULATION_H
