//
// Created by tobias on 27.10.20.
//


#include "Blade.h"

int main() {
    Blade *b = new Blade(1.0, 0.1);
    gsl_matrix *I = gsl_matrix_calloc(3, 3);
    gsl_matrix_set(I, 0, 0, 1);
    gsl_matrix_set(I, 1, 1, 1);
    gsl_matrix_set(I, 2, 2, 1);
    b->initNet(10, Blade::AEQUIDISTANT_DISCRETIZATION, I);
    //b->print_blattlinie();
    b->plot_schlaglinie(0.1, "./Schlaglinie.png");
    return 0;
}