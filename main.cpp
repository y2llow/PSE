#include <iostream>

#include "specificatie_1/1_1_Verkeerssituatie_inlezen/Verkeerssituatie_inlezen.h"

using namespace std;

int main() {
    Verkeerssituatie_inlezen vi;
    vi.parseXMLAndCreateObjects("../src/voorbeeld.xml");


    cout << "\n=========== Banen ===========\n";
    for (auto &[naam, lengte]: vi.get_banen()) {
        cout << "Baan: " << naam << ", Lengte: " << lengte << endl;
    }

    cout << "\n=========== Verkeerslichten ===========\n";
    for (auto &[baan, positie, cyclus]: vi.get_verkeerslichten()) {
        cout << "Baan: " << baan << ", Cyclus: " << cyclus << ", Positie: " << positie << endl;
    }

    cout << "\n=========== Voertuigen ===========\n";
    for (auto &v: vi.get_voertuigen()) {
        cout << "Baan: " << v.getBaan() << ", Positie: " << v.getPositie() << endl;
    }


    return 0;
}
