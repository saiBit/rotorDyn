//
// Created by tobias on 27.10.20.
//

#include "BladeElement.h"

BladeElement::BladeElement(double radius_inner, double radius_center, double radius_outer,
                           gsl_vector *translation)
        : radius_inner(radius_inner), radius_center(radius_center), radius_outer(radius_outer),
          translation(translation) // translation = (u, v, w)^t bei radius_center
{}
