//
// Created by eraya on 13/03/2025.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "../../src/elementen/Baan.h"
#include "../../src/elementen/Voertuig.h"

using namespace std;

class Simulator {
    double current_time = 0;
    vector<Baan*> banen;

    string graphical_impression;
    map<string, string> banen_3d_content;

    bool isConsistent = true;

    Simulator* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Standaard constructor
     * @post properlyInit() == true
     */
    Simulator() {
        _initCheck = this;
    }

    /**
     * @brief Controleert of het object correct geïnitialiseerd is
     * @return true als dit object correct is geïnitialiseerd
     */
    bool properlyInit() const { return _initCheck == this; }

    bool is_consistent = true;

    /**
     * @brief Voegt een baan toe aan de simulator
     * @pre properlyInit() == true && b != nullptr
     * @post getBanen() bevat b
     */
    void addBaan(Baan* b);

    /**
     * @brief Geeft alle banen terug
     * @pre properlyInit() == true
     * @return een vector met alle banen
     */
    [[nodiscard]] const vector<Baan*> getBanen() const;

    /**
     * @brief Stelt de consistentie van de simulator in
     * @pre properlyInit() == true
     * @post is_consistent == c
     */
    void setConsistency(const bool c) { is_consistent = c; }

    /**
     * @brief Voert één simulatiestap uit
     * @pre properlyInit() == true
     * @post current_time is bijgewerkt
     */
    void simulationRun();

    /**
     * @brief Maakt een grafische weergave van de huidige simulatiestatus
     * @pre properlyInit() == true
     * @post graphical_impression en banen_3d_content zijn bijgewerkt
     */
    void makeGraphicalImpression();

    /**
     * @brief Genereert een bestand met de grafische weergave
     * @pre properlyInit() == true
     */
    void generateGraphicsFile() const;

    /**
     * @brief Drukt de huidige simulatiestatus af
     * @pre properlyInit() == true
     */
    void print();

    /**
     * @brief Drukt de status van een voertuig af
     * @pre voertuig != nullptr
     */
    static void printStatus(Voertuig const* voertuig);

    /**
     * @brief Voert de simulatie meerdere keren uit
     * @pre properlyInit() == true && times > 0
     * @post current_time is bijgewerkt met times * SIMULATIE_TIJD
     */
    void simulate(int times);

    /**
     * @brief Genereert een 3D-bestand voor een specifieke baan
     * @pre properlyInit() == true && baan is niet leeg
     */
    void generate3dfile(const string& baan);

    void geldigeTypen(const string& type);
};

#endif //SIMULATION_H