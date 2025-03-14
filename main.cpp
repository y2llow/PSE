#include <iostream>

#include "specificatie_1/simulation.h"

using namespace std;


int main() {
    simulation sim;
    sim.parseXMLAndCreateObjects("../src/voorbeeld.xml"); // implemented 1.1 & 1.2

    sim.ToString(); // implmented 2.1

    sim.simulationRun(); // implemnted 3.1
    sim.ToString();

    return 0;
}
