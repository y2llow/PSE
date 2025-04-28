//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#ifndef SIMPRINTER_H
#define SIMPRINTER_H


#include "../elementen/Voertuig.h"
#include "../elementen/Bushalte.h"
#include "../elementen/Verkeerslicht.h"
#include "../elementen/Voertuig.h"

class SimPrinter {
private:
    vector<string> Gsim;
    pair<int,int> vtXvlIndex;
    map<Baan*,map<int,Voertuig*>> voertuigenOpBaanSIM;
    map<Baan*, map<int, Verkeerslicht*>> verkeerslichtenOpBaanSIM;
    int simulatieSchaal = 5;
    int simCounter = 0;

public:
    static void printStatus(Voertuig const* vehicle, double status);
    //dit maakt de simulatie string
    void generateSimulation(vector<Voertuig *> &voertuigen, vector<Verkeerslicht *> &verkeerslichten,
                            vector<Bushalte *> &bushaltes, vector<Baan *> &banen);

    //dit zal de simulatie string na elk tijd updaten
    void updateSimulation();

    void generateTXT();

    void updateTXT();

    void generateHTML();

    void updateHTML();
};



#endif //SIMPRINTER_H
