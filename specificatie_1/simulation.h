//
// Created by eraya on 13/03/2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "../../src/elementen/Baan.h"
#include "../../src/elementen/Voertuig.h"
#include "../../src/elementen/Verkeerslicht.h"
#include "../../src/elementen/Voertuiggenerator.h"

    using namespace std;

    class simulation {
        // Vectors to store objects created from XML
        vector<Baan*> banen;
        vector<Voertuig*> voertuigen;
        vector<Verkeerslicht*> verkeerslichten;
        vector<Voertuiggenerator*> voertuiggeneratoren;
        double simulationTime = 0.0166;
        double Vmax = 16.6;
        double amax = 1.44;
        double bmax = 4.61;
        double fmin = 4;
        double vertraagAfstand = 50;
        double stopAfstand = 15;
        double vertraagFactor = 0.4;


    public:
        simulation() = default;

        void parseXMLAndCreateObjects(const std::string &filename);

        [[nodiscard]] vector<Baan*> getBanen() const;
        [[nodiscard]] vector<Voertuig*> getVoertuigen() const;
        [[nodiscard]] vector<Verkeerslicht*> getVerkeerslichten() const;
        [[nodiscard]] vector<Voertuiggenerator*> getVoertuiggeneratoren() const;

        [[nodiscard]] bool isConsistent() const;

        [[nodiscard]] Baan* getBaan(const string &name) const;

        void ToString() const;

        double getSimulationTime() const;

        void simulationRun(double simTime);

        void sortVoertuigenByPosition();

        // Removing the created pointers
        ~simulation() {
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




#endif //SIMULATION_H
