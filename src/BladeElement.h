//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADEELEMENT_H
#define ROTORDYN_BLADEELEMENT_H
#include <gsl/gsl_vector.h>

class BladeElement {
public:
    BladeElement(double radius_inner, double radius_center, double radius_outer,
                 gsl_vector *translation // Abstand von Mittelpunkt der t/4 Linie zwischen
            // diesem und vorherigen Element bzw. Mast
    );

private:
    double radius_center, radius_inner, radius_outer;
    gsl_vector *translation;
};

#endif //ROTORDYN_BLADEELEMENT_H
