//
// Created by Abdellah on 4/28/2025.
//

#ifndef BUS_H
#define BUS_H
#include "../Voertuig.h"


class Bus : public Voertuig
{

    double time_sinds_stopped = 0;
public:
    Bus(): Voertuig(12, 11.4, 1.22, 4.29, 12, false)
    {
    }

    [[nodiscard]] string getType() const override;

    [[nodiscard]] double getTimeSindsStopped() const;
    void setTimeSindsStopped(double time_sinds_stopped);
};


#endif //BUS_H
