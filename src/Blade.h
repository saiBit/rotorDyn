//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADE_H
#define ROTORDYN_BLADE_H

#include <list>
#include "BladeElement.h"

class Blade {
private:
    double radius;
    double head_radius;
    std::list<BladeElement> bladeElements;
public:
    static const int AEQUIDISTANT_DISCRETIZATION;

    Blade(double radius, double head_radius);


    int initNet(unsigned int numberOfElements, int discretizationMethod);
};


#endif //ROTORDYN_BLADE_H
