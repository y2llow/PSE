//
// Created by Abdellah on 4/28/2025.
//

#ifndef AUTO_H
#define AUTO_H
#include "../Voertuig.h"


class Auto : public Voertuig
{
public:
    Auto() : Voertuig(4, 16.6, 1.44, 4.61, 4, false)
    {
    }

    [[nodiscard]] string getType() const override;
};


#endif //AUTO_H
