#ifndef BUSHALTE_H
#define BUSHALTE_H
#include <vector>

class Baan;
class Bus;
using namespace std;

class Bushalte {
    vector<Bus*> waited_busses;
    Baan *baan{};
    double positie{};
    double wachttijd{};

public:
    Bushalte() = default;

    [[nodiscard]] Baan* getBaan() const;
    void setBaan(Baan* baan);

    [[nodiscard]] double getPositie() const;
    void setPositie(double positie);

    [[nodiscard]] double getWachttijd() const;
    void setWachttijd(double cyclus);

    void stopBus();
};



#endif //BUSHALTE_H
