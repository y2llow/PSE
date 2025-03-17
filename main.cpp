#include <iostream>

#include "src/simulatie/simulation.h"

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

    // // for (int i =0; i < 690 ;i++) {
    // //     sim.simulationRun(); // implemnted 3.1}
    // // };
    // for (int i =0; i < 1000 ;i++) {
    //     for (int j =0; j < 10 ;i++) {
    //         sim.ToString(); // implmented 2.1
    //         sim.simulationRun(); // implemnted 3.1}
    //     }
    // };

    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();
    sim.simulationRun(); // implemnted 3.1
    sim.ToString();


    return 0;
}
