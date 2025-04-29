//
// Created by Abdellah on 4/28/2025.
//

#ifndef POLITIECOMBI_H
#define POLITIECOMBI_H
#include "../Voertuig.h"


class Politiecombi: public Voertuig{

    public:
    Politiecombi(): Voertuig(6, 17.2, 1.55, 4.92, 6, true){}

    [[nodiscard]] string getType() const override;

};



#endif //POLITIECOMBI_H
