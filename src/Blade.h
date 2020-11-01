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
    double *element_positions_x;
    double *element_positions_y;
    double *element_positions_z;
    std::list<BladeElement> bladeElements;
public:
    static const int AEQUIDISTANT_DISCRETIZATION;

    void print_blattlinie();

    // TODO: Change this so that we can input a I that changes over the radius
    Blade(double radius, double head_radius);

    int initNet(unsigned int numberOfElements, int discretizationMethod, gsl_matrix *I);
};


#endif //ROTORDYN_BLADE_H
