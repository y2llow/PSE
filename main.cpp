#include <iostream>

#include "specificatie_1/simulation.h"

using namespace std;


int main() {
    simulation sim;

    sim.parseXMLAndCreateObjects("../src/voorbeeld.xml"); // implemented 1.1 & 1.2

    // Om de ingelezen verkeerssituatie te kunnen simuleren moet de informatie consistent zijn
    // 3. Verifieer consistentie van de verkeerssituatie
    if (!sim.isConsistent()) {
        cerr << "Het verkeerssituatie is inconsistent" << endl;
        return 1;
    }

    sim.ToString(); // implmented 2.1

    while (sim.getVoertuigen().size() < 10) {
        sim.simulationRun(); // implemnted 3.1
        sim.ToString();
    }


    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();
    // sim.simulationRun(); // implemnted 3.1
    // sim.ToString();


    return 0;
}
