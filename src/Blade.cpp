//
// Created by tobias on 27.10.20.
//
#include "Blade.h"
#include "../lib/matplotlib-cpp-master/matplotlibcpp.h"
#include <stdio.h>

#define _USE_MATH_DEFINES

#include <cmath>

namespace plt = matplotlibcpp;

const int Blade::AEQUIDISTANT_DISCRETIZATION = 0x01;

int Blade::initNet(unsigned int numberOfElements, int discretizationMethod, gsl_matrix *I) {
    // Blattradius ohne Rotorkopf
    double effective_radius = this->radius - this->head_radius;
    // Liste der Blattelemente initialisieren
    printf("Initialisierung der Blattelemente, N = %u , R = %f m\n", numberOfElements, this->radius);
    this->element_positions_x = new double[numberOfElements];
    this->element_positions_y = new double[numberOfElements];
    this->element_positions_z = new double[numberOfElements];
    this->bladeElements = {};
    switch (discretizationMethod) {
        case Blade::AEQUIDISTANT_DISCRETIZATION:
        default:
            printf("Aequidistante Diskretisierung.\n");
            // Durch Elemente iterieren, für jedes Element ein Blattelement generieren
            for (int i = 0; i < numberOfElements; i++) {
                double r_i = effective_radius * i / numberOfElements + this->head_radius;
                double r_a = effective_radius * (i + 1) / numberOfElements + this->head_radius;
                double r = (r_i + r_a) / 2;

                gsl_vector *translation = gsl_vector_alloc(3);
                gsl_vector_set_zero(translation);
                double translation_u = r_a - r_i;
                double translation_v = 0;
                double translation_w = 0;
                gsl_vector_set(translation, 0, translation_u);                // delta_u
                gsl_vector_set(translation, 1, translation_v);                      // delta_v
                gsl_vector_set(translation, 2, translation_w);                      // delta_w

                BladeElement b = BladeElement(r_i, r, r_a, translation, 5, I);
                // Van der Wall says flapping polynomial order has to be at least 5
                this->bladeElements.push_back(b);
                if (i > 0) {
                    element_positions_x[i] = element_positions_x[i - 1] + translation_u;
                    element_positions_y[i] = element_positions_y[i - 1] + translation_v;
                    element_positions_z[i] = element_positions_z[i - 1] + translation_w;
                } else {
                    element_positions_x[i] = r;
                    element_positions_y[i] = 0;
                    element_positions_z[i] = 0;
                }
            }

            break;
    }
    printf("Blattelemente initialisiert.\n Generiere Blattlinie...\n");
    this->compute_blattlinie();
    printf("Blattlinie generiert.\n");

    return 0;
}

void Blade::print_blattlinie() {
    printf("Blattlinie: \n x: ");
    for (int i = 0; i < this->bladeElements.size(); i++)
        printf("%f ", this->element_positions_x[i]);
    printf("\n y: ");
    for (int i = 0; i < this->bladeElements.size(); i++)
        printf("%f ", this->element_positions_y[i]);
    printf("\n z: ");
    for (int i = 0; i < this->bladeElements.size(); i++)
        printf("%f ", this->element_positions_z[i]);
};

Blade::Blade(double radius, double head_radius) : radius(radius), head_radius(head_radius) {}

void Blade::compute_blattlinie() {
    int n = this->bladeElements.size();
    this->blattlinie_xy_accel = gsl_interp_accel_alloc();
    this->blattlinie_xz_accel = gsl_interp_accel_alloc();
    // xy-Ebene (Schwenken)
    this->blattlinie_xy_spline = gsl_spline_alloc(gsl_interp_cspline, this->bladeElements.size());
    gsl_spline_init(this->blattlinie_xy_spline, this->element_positions_x,
                    this->element_positions_y, this->bladeElements.size());
    // xz-Ebene (Schlagen)
    this->blattlinie_xz_spline = gsl_spline_alloc(gsl_interp_cspline, this->bladeElements.size());
    gsl_spline_init(this->blattlinie_xz_spline, this->element_positions_x,
                    this->element_positions_z, this->bladeElements.size());
}

void Blade::plot_schlaglinie(double delta_x, char *filename) {
    printf("Zeichne Schlaglinie...\n");
    int number_of_elements = this->bladeElements.size();
    double x_min = this->bladeElements.front().get_radius_center();
    double x_max = this->bladeElements.back().get_radius_center();
    int n = (int) ((x_max - x_min) / delta_x);
    // Prepare data.
    std::vector<double> x(0), y(0);
    for (int i = 0; i < n; i++) {
        double xi = i * delta_x + x_min;
        x.push_back(xi);
        y.push_back(gsl_spline_eval(this->blattlinie_xz_spline, xi, this->blattlinie_xz_accel));
    }
    plt::figure_size(1200, 780);
    plt::title("Schlaglinie");
    plt::xlim((int) floor(0),
              (int) ceil(this->radius)); // This is a workaround since xlim does only take ints
    plt::plot(x, y);
    //plt::plot(this->element_positions_x, this->element_positions_y);
    std::cout << "Saving result to " << filename << std::endl;;
    plt::save(filename);
}