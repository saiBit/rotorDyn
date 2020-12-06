//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADE_H
#define ROTORDYN_BLADE_H

#include <list>
#include "BladeElement.h"
#include <gsl/gsl_spline.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_math.h>
#include <vector>

class Blade {
private:
    double radius;
    double head_radius;
    double *element_positions_x;
    double *element_positions_y;
    double *element_positions_z;
    std::vector<BladeElement> bladeElements;

    // interpolation of the Blattlinie
    /** interpolation options are:
     * gsl_interp_cspline
     * gsl_interp_cspline_periodic (dont use this one, here y_first has to be equal to y_las)
     * gsl_interp_linear (dont use this one)
     * gsl_interp_polynomial (caution, may cause oscillations)
     * gsl_interp_akima
     * gsl_interp_akima_periodic (periodic boundary conditions)
     * gsl_interp_steffen (Steffen’s method guarantees the monotonicity of the interpolating function between
     *  the given data points. Therefore, minima and maxima can only occur exactly at the data points, and there
     *  can never be spurious oscillations between data points. The interpolated function is piecewise cubic in each
     *  interval. The resulting curve and its first derivative are guaranteed to be continuous, but the second
     *  derivative may be discontinuous.)
    */
    const gsl_interp_type *interpolation_method;

    gsl_spline *blattlinie_xz_spline;
    gsl_interp_accel *blattlinie_xz_accel;
    gsl_spline *blattlinie_xy_spline;
    gsl_interp_accel *blattlinie_xy_accel;

    void differentiate_blattlinie();

    gsl_interp_accel *blattlinie_xy_dy_dx_accel;
    gsl_interp_accel *blattlinie_xz_dz_dx_accel;
    gsl_spline *blattlinie_xy_dy_dx_spline;
    gsl_spline *blattlinie_xz_dz_dx_spline;

    // Randbedingungen
    double dy_dx_o = 0;
    double dz_dx_o = 0;
    // derivatives of the blattlinie


    void compute_blattlinie();

public:
    static const int AEQUIDISTANT_DISCRETIZATION;

    void print_blattlinie();

    // TODO: Change this so that we can input a I that changes over the radius
    Blade(double radius, double head_radius);

    int initNet(unsigned int numberOfElements, int discretizationMethod, double k_alpha, double k_beta, double k_gamma,
                const gsl_interp_type *interpolation_method);


    void plot_schlaglinie(double delta_x, char *filename);

    void print_blade_elements();

    double get_Element_Coord(double sw, char dimension);

};

#endif //ROTORDYN_BLADE_H
