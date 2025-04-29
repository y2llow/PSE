//
// Created by Abdellah on 4/28/2025.
//

#ifndef BRANDWEERWAGEN_H
#define BRANDWEERWAGEN_H
#include "../Voertuig.h"


class Brandweerwagen: public Voertuig {

    public:
    Brandweerwagen(): Voertuig(10, 14.6, 1.33, 4.56, 10, true){}

    [[nodiscard]] string getType() const override;

};



#endif //BRANDWEERWAGEN_H
