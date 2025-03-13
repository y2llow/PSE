#ifndef VERKEERSS_INLEZEN_H
#define VERKEERSS_INLEZEN_H

#include <vector>
#include "../../src/elementen/Baan.h"
#include "../../src/elementen/Voertuig.h"
#include "../../src/elementen/Verkeerslicht.h"
#include "../../src/elementen/Voertuiggenerator.h"

using namespace std;

class VerkeersSituatieInlezen {
    // Vectors to store objects created from XML
    vector<Baan*> banen;
    vector<Voertuig*> voertuigen;
    vector<Verkeerslicht*> verkeerslichten;
    vector<Voertuiggenerator*> voertuiggeneratoren;

public:
    VerkeersSituatieInlezen() = default;

    void parseXMLAndCreateObjects(const std::string &filename);

    [[nodiscard]] vector<Baan*> getBanen() const;
    [[nodiscard]] vector<Voertuig*> getVoertuigen() const;
    [[nodiscard]] vector<Verkeerslicht*> getVerkeerslichten() const;
    [[nodiscard]] vector<Voertuiggenerator*> getVoertuiggeneratoren() const;

    [[nodiscard]] bool isConsistent() const;

    Baan* getBaan(const string &name) const;

    // Removing the created pointers
    ~VerkeersSituatieInlezen() {
        for (const auto b : banen)
            delete b;

        for (const auto v : voertuigen)
            delete v;

        for (const auto v : voertuiggeneratoren)
            delete v;

        for (const auto v : verkeerslichten)
            delete v;


    }
};
#endif
