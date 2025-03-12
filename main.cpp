#include <iostream>

#include "specificatie_1/1_1_verkeerssituatie_inlezen/VerkeerssituatieInlezen.h"

using namespace std;

int main() {
    VerkeerssituatieInlezen vi;
    vi.parseXMLAndCreateObjects("../src/voorbeeld.xml");


    cout << "\n=========== Banen ===========\n";
    for (auto baan: vi.getBanen()) {
        cout << "Baan: " << baan->getNaam() << ", Lengte: " << baan->getLengte() << endl;
    }

    cout << "\n=========== Verkeerslichten ===========\n";
    for (auto &licht: vi.getVerkeerslichten()) {
        cout << "Baan: " << licht->getBaan() << ", Cyclus: " << licht->getCyclus() << ", Positie: " << licht->getPositie() << endl;
    }

    cout << "\n=========== Voertuigen ===========\n";
    for (auto &v: vi.getVoertuigen()) {
        cout << "Baan: " << v->getBaan() << ", Positie: " << v->getPositie() << endl;
    }

    cout << "\n=========== Consistency ===========\n";
    cout << boolalpha << vi.isConsistent() << endl;

    return 0;
}
