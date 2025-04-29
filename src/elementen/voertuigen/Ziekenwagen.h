//
// Created by Abdellah on 4/28/2025.
//

#ifndef ZIEKENWAGEN_H
#define ZIEKENWAGEN_H
#include "../Voertuig.h"


class Ziekenwagen: public Voertuig {

    public:
    Ziekenwagen(): Voertuig(8, 15.5, 1.44, 4.47, 8, true){}

    [[nodiscard]] string getType() const override;
};



#endif //ZIEKENWAGEN_H
