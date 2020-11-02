//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADE_H
#define ROTORDYN_BLADE_H

#include <list>
#include "BladeElement.h"
#include <gsl/gsl_spline.h>

class Blade {
private:
    double radius;
    double head_radius;
    double *element_positions_x;
    double *element_positions_y;
    double *element_positions_z;
    std::list<BladeElement> bladeElements;

    // interpolation of the Blattlinie
    gsl_spline *blattlinie_xz_spline;
    gsl_interp_accel *blattlinie_xz_accel;
    gsl_spline *blattlinie_xy_spline;
    gsl_interp_accel *blattlinie_xy_accel;

    void compute_blattlinie();

public:
    static const int AEQUIDISTANT_DISCRETIZATION;

    void print_blattlinie();

    // TODO: Change this so that we can input a I that changes over the radius
    Blade(double radius, double head_radius);

    int initNet(unsigned int numberOfElements, int discretizationMethod, gsl_matrix *I);

    void plot_schlaglinie(double delta_x, char *filename);
};


#endif //ROTORDYN_BLADE_H
