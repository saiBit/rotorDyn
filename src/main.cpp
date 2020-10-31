//
// Created by tobias on 27.10.20.
//


#include "Blade.h"

int main() {
    Blade *b = new Blade(3.0, 0.2);
    b->initNet(100, Blade::AEQUIDISTANT_DISCRETIZATION);
    return 0;
}