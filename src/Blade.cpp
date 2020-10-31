//
// Created by tobias on 27.10.20.
//
#include "Blade.h"
#include <stdio.h>

const int Blade::AEQUIDISTANT_DISCRETIZATION = 0x01;

int Blade::initNet(unsigned int numberOfElements, int discretizationMethod, gsl_matrix *I) {
    // Blattradius ohne Rotorkopf
    double effective_radius = this->radius - this->head_radius;
    // Liste der Blattelemente initialisieren
    printf("Initialisierung der Blattelemente, N = %u , R = %f m\n", numberOfElements, this->radius);
    this->bladeElements = {};
    switch (discretizationMethod) {
        case Blade::AEQUIDISTANT_DISCRETIZATION:
        default:
            printf("Aequidistante Diskretisierung.");
            // Durch Elemente iterieren, für jedes Element ein Blattelement generieren
            for (int i = 0; i < numberOfElements; i++) {
                double r_i = effective_radius * i / numberOfElements + this->head_radius;
                double r_a = effective_radius * (i + 1) / numberOfElements + this->head_radius;
                double r = (r_i + r_a) / 2;

                gsl_vector *translation = gsl_vector_alloc(3);
                gsl_vector_set_zero(translation);
                gsl_vector_set(translation, 0, r_a - r_i);                // delta_u
                gsl_vector_set(translation, 1, 0);                      // delta_v
                gsl_vector_set(translation, 2, 0);                      // delta_w

                BladeElement b = BladeElement(r_i, r, r_a, translation, 5, I);
                // Van der Wall says flapping polynomial order has to be at least 5
                this->bladeElements.push_back(b);
            }

            break;

    }
    return 0;
}

Blade::Blade(double radius, double head_radius) : radius(radius), head_radius(head_radius) {}