//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>


class Voertuig {
    std::string baan;
    double positie;
    double snelheid;
    int id;

public:
    const std::string &getBaan() const;

    void setBaan(const std::string &baan);

    double getPositie() const;

    void setPositie(double positie);

    double getSnelheid() const;

    void setSnelheid(double snelheid);

    int getId() const;

    void setId(int id);
};



#endif //VOERTUIG_H
