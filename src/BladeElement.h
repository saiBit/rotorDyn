//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADEELEMENT_H
#define ROTORDYN_BLADEELEMENT_H
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

class BladeElement {
public:
    BladeElement(double radius_inner, double radius_center, double radius_outer,
                 gsl_vector *translation,// Abstand von Mittelpunkt der t/4 Linie zwischen
            // diesem und vorherigen Element bzw. Mast
                 unsigned int flapping_polynomial_order,
                 gsl_matrix *I
    );

private:
    double radius_center, radius_inner, radius_outer;

    double *flapping_polynomial;

    gsl_vector *translation;    // in local coordinate system

    gsl_vector *force;          // in local coordinate system
    gsl_vector *force_from_left;
    gsl_vector *force_from_right;

    gsl_vector *moment;         // in local coordinate system
    gsl_vector *moment_from_left;
    gsl_vector *moment_from_right;

    gsl_matrix *I;              // Flächenträgheitsmoment um t/4-Linie
};

#endif //ROTORDYN_BLADEELEMENT_H
