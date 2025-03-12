//
// Created by s0243673@ad.ua.ac.be on 2/27/25.
//

#ifndef VOERTUIG_H
#define VOERTUIG_H
#include <string>


class Voertuig {
    std::string baan;
    int positie = 0;

public:
    void setBaan(const std::string &baan);
    [[nodiscard]] std::string getBaan() const;

    void setPositie(int p);
    [[nodiscard]] int getPositie() const;
};



#endif //VOERTUIG_H
