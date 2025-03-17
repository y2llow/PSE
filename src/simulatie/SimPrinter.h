//
// Created by s0246031@ad.ua.ac.be on 3/17/25.
//

#ifndef SIMPRINTER_H
#define SIMPRINTER_H


class Voertuig;

class SimPrinter {

    public:
    void printStatus(Voertuig* v, double time) const;
};



#endif //SIMPRINTER_H
