#include <iostream>

#include "specificatie_1/simulation.h"

using namespace std;


int main() {
    simulation vi;
    vi.parseXMLAndCreateObjects("../src/voorbeeld.xml"); // implemented 1.1 & 1.2

    vi.ToString(); // implmented 2.1

    vi.simulationRun(); // implemnted 3.1

    return 0;
}
