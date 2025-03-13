#include <iostream>

#include "specificatie_1/simulation.h"

using namespace std;


int main() {
    simulation vi;
    vi.parseXMLAndCreateObjects("../src/voorbeeld.xml");

    vi.ToString();


    return 0;
}
