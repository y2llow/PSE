#include <iostream>

#include "src/simulatie/Parser.h"
#include "src/simulatie/simulation.h"

using namespace std;


int main() {
    simulation* sim = new simulation();
    Parser parser;
    parser.parseElements("../src/voorbeeldXML/voorbeeld2.xml", sim);

    // Om de ingelezen verkeerssituatie te kunnen simuleren moet de informatie consistent zijn
    if (!sim->isConsistent()) {
        cerr << "Het verkeerssituatie is inconsistent" << endl;
        return 1;
    }

    sim->ToString();
    sim->generateSimulation();
    sim->printSimulation();
    for (int i = 0; i<100;i++){
        sim->simulationRun();
        sim->ToString();
        sim->updateSimulation();
        sim->printSimulation();
    }


    return 0;
}
