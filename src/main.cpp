//
// Created by tobias on 27.10.20.
//


#include "Blade.h"

int main() {
    Blade *b = new Blade(1.0, 0);
    b->initNet(100, Blade::AEQUIDISTANT_DISCRETIZATION, 1, 1, 1, gsl_interp_akima);
    b->print_blattlinie();
    b->plot_schlaglinie(0.001, "./Schlaglinie.png");
    return 0;
}