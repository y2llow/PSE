#include <iostream>

#include "specificatie_1/1_1_verkeerssituatie_inlezen/VerkeersSituatieInlezen.h"

using namespace std;
/*
int main() {
    VerkeersSituatieInlezen vi;
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
*/

int main() {
    VerkeersSituatieInlezen vi;
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
        cout<<"Voertuig "<< v.getId()<<"\n"
        << "->Baan: " << v.getBaan() << "\n"
        "->Positie: " << v.getPositie() << "\n"
        <<"->snelheid: " << v.getSnelheid() << "\n" << endl;
    }

    cout <<"\n"<< "BEGIN VAN DE SIMULATIE:"<<endl;




    return 0;
}