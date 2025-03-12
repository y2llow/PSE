#ifndef VERKEERSS_INLEZEN_H
#define VERKEERSS_INLEZEN_H

#include <vector>
#include "../../src/Elementen/Baan.h"
#include "../../src/Elementen/Voertuig.h"
#include "../../src/Elementen/Verkeerslicht.h"
#include "../../src/Elementen/Voertuiggenerator.h"

using namespace std;

class VerkeersSituatieInlezen {
    // Vectors to store objects created from XML
    vector<Baan> banen;
    vector<Voertuig> voertuigen;
    vector<Verkeerslicht> verkeerslichten;
    vector<Voertuiggenerator> voertuiggeneratoren;

public:
    void parseXMLAndCreateObjects(const std::string &filename);

    [[nodiscard]] vector<Baan> get_banen() const;
    [[nodiscard]] vector<Voertuig> get_voertuigen() const;
    [[nodiscard]] vector<Verkeerslicht> get_verkeerslichten() const;
    [[nodiscard]] vector<Voertuiggenerator> get_voertuiggeneratoren() const;

};
#endif
