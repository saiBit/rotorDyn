//
// Created by tobias on 27.10.20.
//

#include "BladeElement.h"

BladeElement::BladeElement(double radius_inner, double radius_center, double radius_outer,
                           gsl_vector *translation, unsigned int flapping_polynomial_order,
                           double k_alpha, double k_beta, double k_gamma)
        : radius_inner(radius_inner), radius_center(radius_center), radius_outer(radius_outer),
          translation(translation), // translation = (u, v, w)^t bei radius_center
          k_alpha(k_alpha),          // Steifigkeit Torsion
          k_beta(k_beta),
          k_gamma(k_gamma)
{
    // We initialize the flapping polynomial as a straight line
    this->flapping_polynomial = new double[flapping_polynomial_order];
    for (int i = 0; i < flapping_polynomial_order; i++) {
        this->flapping_polynomial[i] = 0;

        this->force = gsl_vector_calloc(3);
        this->force_from_left = gsl_vector_calloc(3);
        this->force_from_right = gsl_vector_calloc(3);

        this->moment = gsl_vector_calloc(3);
        this->moment_from_left = gsl_vector_calloc(3);
        this->moment_from_right = gsl_vector_calloc(3);
    }


}

double BladeElement::get_radius_center() {
    return this->radius_center;
}
