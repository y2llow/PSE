#ifndef VERKEERSS_INLEZEN_H
#define VERKEERSS_INLEZEN_H

#include <vector>
#include "../../src/Elementen/Baan.h"
#include "../../src/Elementen/Voertuig.h"
#include "../../src/Elementen/Verkeerslicht.h"
#include "../../src/Elementen/Voertuiggenerator.h"

using namespace std;

class VerkeerssituatieInlezen {
    // Vectors to store objects created from XML
    vector<Baan> banen;
    vector<Voertuig> voertuigen;
    vector<Verkeerslicht> verkeerslichten;
    vector<Voertuiggenerator> voertuiggeneratoren;

public:
    void parseXMLAndCreateObjects(const std::string &filename);

    [[nodiscard]] vector<Baan> getBanen() const;
    [[nodiscard]] vector<Voertuig> getVoertuigen() const;
    [[nodiscard]] vector<Verkeerslicht> getVerkeerslichten() const;
    [[nodiscard]] vector<Voertuiggenerator> getVoertuiggeneratoren() const;

};
#endif
