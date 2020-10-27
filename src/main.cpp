//
// Created by tobias on 27.10.20.
//

#include <cstdio>
#include <gsl/gsl_sf_bessel.h>
#include "BladeElement.h"

int main(){
    BladeElement b = BladeElement(1);
    printf("Hallo Welt!\n");
    double x = 5.0;
    double y = gsl_sf_bessel_J0 (x);
    printf ("J0(%g) = %.18e\n", x, y);
    return 0;
}