//
// Created by tobias on 27.10.20.
//
#include "Blade.h"
#include <stdio.h>

const int Blade::AEQUIDISTANT_DISCRETIZATION = 0x01;

int Blade::initNet(unsigned int numberOfElements, int discretizationMethod) {
    // Liste der Blattelemente initialisieren
    printf("Initialisierung der Blattelemente, N = %u , R = %f m\n", numberOfElements, this->radius);
    this->bladeElements = {};
    switch (discretizationMethod) {
        case Blade::AEQUIDISTANT_DISCRETIZATION:
        default:
            printf("Aequidistante Diskretisierung.");
            // Durch Elemente iterieren, für jedes Element ein Blattelement generieren
            for (int i = 0; i < numberOfElements; i++) {
                double r_i = this->radius * i / numberOfElements;
                double r = this->radius / (numberOfElements * 2) + r_i;
                double r_a = this->radius * (i + 1) / numberOfElements;
                gsl_vector *translation = gsl_vector_alloc(3);
                gsl_vector_set_zero(translation);
                BladeElement b = BladeElement(r_i, r, r_a, translation);
                this->bladeElements.push_back(b);
            }

            break;

    }
    return 0;
}

Blade::Blade(double radius) : radius(radius) {}