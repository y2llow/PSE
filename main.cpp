#include <iostream>

#include "src/simulatie/Parser.h"
#include "src/simulatie/Simulator.h"

using namespace std;


int main() {

    // Om de ingelezen verkeerssituatie te kunnen simuleren moet de informatie consistent zijn
    // if (!sim->isConsistent()) {
    //     cerr << "Het verkeerssituatie is inconsistent" << endl;
    //     return 1;
    // }

    auto* sim = new Simulator();
    if (Parser::parseElements("../src/voorbeeldXML/voorbeeld10.xml", sim))
    {
        sim->simulate(10);
    }


    return 0;
}
