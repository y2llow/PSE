#include <iostream>

#include "specificatie_1/1_1_Verkeerssituatie_inlezen/Verkeerssituatie_inlezen.h"

using namespace std;

int main() {
    Verkeerssituatie_inlezen vi;
    vi.parseXMLAndCreateObjects("../src/voorbeeld.xml");

    // for (auto &[baan, positie] : vi.get_voertuigen()) {
    //     cout << "Baan: " << baan << ", Baan Lengte: " << baan.length() << ", Positie: " << positie << endl;
    // }

    for (auto&[naam, lengte] : vi.get_banen()) {
        cout << "Baan: " << naam << ", Lengte: " << lengte << endl;
    }
    return 0;
}
