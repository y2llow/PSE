#ifndef BAAN_H
#define BAAN_H

#include <string>
#include <utility>
#include <vector>
#include <map>

class Voertuiggenerator;
class Voertuig;
class Verkeerslicht;
class Bushalte;

using namespace std;

class Baan {
    string naam;
    int lengte{};

    vector<Bushalte*> bushaltes;
    vector<Verkeerslicht*> verkeers;
    vector<Voertuig*> voertuigen;
    vector<Voertuiggenerator*> voertuiggeneratoren;

    vector<double> bushaltes_positions;
    map<int, vector<Baan*>> kruispunten;

    Baan* _initCheck; // Voor properlyInit

public:
    /**
     * @brief Constructor zonder parameters.
     * @post properlyInit() == true
     */
    Baan() { _initCheck = this; }

    /**
     * @brief Constructor met naam en lengte.
     * @post getNaam() == naam && getLengte() == lengte
     * @post properlyInit() == true
     */
    Baan(string naam, const int lengte)
            : naam(std::move(naam)), lengte(lengte)
    {
        _initCheck = this;
    }

    /**
     * @brief Check of het object correct geïnitialiseerd is.
     * @return true als dit object correct is geïnitialiseerd
     */
    bool properlyInit() const { return _initCheck == this; }

    /**
     * @brief Geeft de naam van de baan terug.
     * @pre properlyInit() == true
     */
    [[nodiscard]] string getNaam() const;

    /**
     * @brief Zet de naam van de baan.
     * @pre properlyInit() == true
     * @post getNaam() == naam
     */
    void setNaam(const string& naam);

    /**
     * @brief Geeft de lengte van de baan.
     * @pre properlyInit() == true
     */
    [[nodiscard]] int getLengte() const;

    /**
     * @brief Zet de lengte van de baan.
     * @pre lengte > 0
     * @pre properlyInit() == true
     * @post getLengte() == lengte
     */
    void setLengte(int lengte);

    /**
     * @brief Voegt een bushalte toe.
     * @pre b != nullptr
     * @pre properlyInit() == true
     * @post getBushaltes().back() == b
     */
    void addBushalte(Bushalte* b);

    /**
     * @brief Voegt een verkeerslicht toe.
     * @pre v != nullptr
     * @pre properlyInit() == true
     * @post getVerkeerslichten().back() == v
     */
    void addVerkeerslicht(Verkeerslicht* v);

    /**
     * @brief Voegt een voertuig toe.
     * @pre v != nullptr
     * @pre properlyInit() == true
     * @post getVoertuigen().back() == v
     */
    void addVoertuig(Voertuig* v);

    /**
     * @brief Voegt een voertuiggenerator toe.
     * @pre vg != nullptr
     * @pre properlyInit() == true
     * @post getVoertuigeneratoren().back() == vg
     */
    void addVoertuiggenerator(Voertuiggenerator* vg);

    /**
     * @brief Voegt een kruispunt toe aan deze baan.
     * @pre value != nullptr
     * @pre properlyInit() == true
     * @post getKruispunten().at(key) bevat value
     */
    void addKruispunt(int key, Baan* value);

    /**
     * @brief Verwijdert een voertuig en delete het.
     * @pre v != nullptr
     * @pre properlyInit() == true
     * @post v is verwijderd uit getVoertuigen()
     */
    void removeVoertuig(Voertuig* v);

    /**
     * @brief Haalt een voertuig uit de lijst (zonder delete).
     * @pre v != nullptr
     * @pre properlyInit() == true
     * @post v is verwijderd uit getVoertuigen()
     */
    void takeOutVoertuig(Voertuig* v);

    /**
     * @brief Geeft de verkeerslichten op deze baan.
     * @pre properlyInit() == true
     */
    const vector<Verkeerslicht*>& getVerkeerslichten() const;

    /**
     * @brief Geeft de bushaltes op deze baan.
     * @pre properlyInit() == true
     */
    const vector<Bushalte*>& getBushaltes() const;

    /**
     * @brief Geeft de voertuigen op deze baan.
     * @pre properlyInit() == true
     */
    const vector<Voertuig*>& getVoertuigen() const;

    /**
     * @brief Geeft de voertuiggeneratoren op deze baan.
     * @pre properlyInit() == true
     */
    const vector<Voertuiggenerator*>& getVoertuigeneratoren() const;

    /**
     * @brief Geeft de kruispuntenstructuur.
     * @pre properlyInit() == true
     */
    const map<int, vector<Baan*>>& getKruispunten() const;

    /**
     * @brief Sorteert voertuigen op positie (aflopend).
     * @pre properlyInit() == true
     * @post Voor alle geldige i: getVoertuigen()[i]->getPositie() >= getVoertuigen()[i+1]->getPositie()
     */
    void sortVoertuigenByPosition();

    /**
     * @brief Sorteert verkeerslichten op positie (oplopend).
     * @pre properlyInit() == true
     * @post Voor alle geldige i: getVerkeerslichten()[i]->getPositie() <= getVerkeerslichten()[i+1]->getPositie()
     */
    void sortVerkeerslichtenByPosition();
};

#endif // BAAN_H
