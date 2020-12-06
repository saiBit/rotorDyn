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

int Blade::initNet(unsigned int numberOfElements, int discretizationMethod, double k_alpha, double k_beta,
                   double k_gamma, const gsl_interp_type *interpolation_method) {
    this->interpolation_method = interpolation_method;

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
                double translation_w =
                        (0 + 0.0 * r_i + 0.01 * r_i * r_i - 0.02 * r_i * r_i * r_i + 0.005 * r_i * r_i * r_i * r_i) /
                        (numberOfElements); // TODO: CHANGE THIS
                gsl_vector_set(translation, 0, translation_u);                // delta_u
                gsl_vector_set(translation, 1, translation_v);                      // delta_v
                gsl_vector_set(translation, 2, translation_w);                      // delta_w

                BladeElement b = BladeElement(r_i, r, r_a, translation, 5, k_alpha, k_beta, k_gamma);
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
    this->differentiate_blattlinie();
    printf("Erste Ableitungen der Blattlinie generiert.\n");
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
    printf("\n");
};

Blade::Blade(double radius, double head_radius) : radius(radius), head_radius(head_radius) {}

void Blade::compute_blattlinie() {
    int n = this->bladeElements.size();
    this->blattlinie_xy_accel = gsl_interp_accel_alloc();
    this->blattlinie_xz_accel = gsl_interp_accel_alloc();
    // xy-Ebene (Schwenken)
    this->blattlinie_xy_spline = gsl_spline_alloc(this->interpolation_method, this->bladeElements.size());
    gsl_spline_init(this->blattlinie_xy_spline, this->element_positions_x,
                    this->element_positions_y, this->bladeElements.size());
    // xz-Ebene (Schlagen)
    this->blattlinie_xz_spline = gsl_spline_alloc(this->interpolation_method, this->bladeElements.size());
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
    std::vector<double> x(0), y(0), y_dot(0);
    for (int i = 0; i < n; i++) {
        double xi = i * delta_x + x_min;
        x.push_back(xi);
        y.push_back(gsl_spline_eval(this->blattlinie_xz_spline, xi, this->blattlinie_xz_accel));
        y_dot.push_back(gsl_spline_eval(this->blattlinie_xz_dz_dx_spline, xi, this->blattlinie_xz_dz_dx_accel));
    }
    plt::figure_size(1200, 780);
    plt::title("Schlaglinie");
    plt::xlim((int) floor(0),
              (int) ceil(this->radius)); // This is a workaround since xlim does only take ints
    plt::named_plot("z", x, y);
    plt::named_plot("dz/dx", x, y_dot);
    plt::legend();
    //plt::plot(this->element_positions_x, this->element_positions_y);
    std::cout << "Saving result to " << filename << std::endl;;
    plt::save(filename);
}

void Blade::print_blade_elements() {
    int N = this->bladeElements.size();
    printf("i| Mschlag | Mtorsion | Mschwenk");
    for (int i = 0; i < N; i++) {
        BladeElement e = this->bladeElements[i];
        printf("%f)| 0 | ", i);
    }
}

static double get_y(double spanwidth, void *b) {
    auto *blade = (Blade *) b;
    return blade->get_Element_Coord(spanwidth, 1);
}

static double get_z(double spanwidth, void *b) {
    auto *blade = (Blade *) b;
    return blade->get_Element_Coord(spanwidth, 2);
}

void Blade::differentiate_blattlinie() {

    gsl_function dz;
    gsl_function dy;
    double result, abserr;

    dz.function = &get_z;
    dz.params = this;
    dy.function = &get_y;
    dy.params = this;

    auto dz_dx = [this, &dz](double x) {
        double result, abserr;
        gsl_deriv_central(&dz, x, 1e-8, &result, &abserr);
        return result;
    };

    auto dy_dx = [this, &dy](double x) {
        double result, abserr;
        gsl_deriv_central(&dy, x, 1e-8, &result, &abserr);
        return result;
    };

    int n = this->bladeElements.size();
    this->blattlinie_xy_dy_dx_accel = gsl_interp_accel_alloc();
    this->blattlinie_xz_dz_dx_accel = gsl_interp_accel_alloc();

    // xy-Ebene (Schwenken)

    this->blattlinie_xy_dy_dx_spline = gsl_spline_alloc(this->interpolation_method, this->bladeElements.size());
    int N = this->bladeElements.size();
    double x_o = this->element_positions_x[0];
    double x_max = this->element_positions_x[N - 1];
    double dy_dx_val[N];
    // Randwerte
    gsl_deriv_forward(&dz, x_o, 1e-8, &result, &abserr);
    dy_dx_val[0] = result;
    gsl_deriv_backward(&dz, x_max, 1e-8, &result, &abserr);
    dy_dx_val[N - 1] = result;
    for (int i = 1; i < N - 1; i++) {
        dy_dx_val[i] = dy_dx(this->element_positions_x[i]);
    }
    gsl_spline_init(this->blattlinie_xy_dy_dx_spline, this->element_positions_x,
                    dy_dx_val, N);

    // xz-Ebene (Schlagen)
    this->blattlinie_xz_dz_dx_spline = gsl_spline_alloc(this->interpolation_method, this->bladeElements.size());
    double dz_dx_val[N];
    // Randwerte
    gsl_deriv_forward(&dz, x_o, 1e-8, &result, &abserr);
    dz_dx_val[0] = result;
    gsl_deriv_backward(&dz, x_max, 1e-8, &result, &abserr);
    dz_dx_val[N - 1] = result;
    for (int i = 1; i < N - 1; i++) {
        dz_dx_val[i] = dz_dx(this->element_positions_x[i]);
    }
    // An Blattspitze ist dz/dx = const
    dz_dx_val[N - 1] = dz_dx_val[N - 2];

    gsl_spline_init(this->blattlinie_xz_dz_dx_spline, this->element_positions_x,
                    dz_dx_val, N);
};


/**
 *
 * @param span width
 * @return the (x y z) vector of the element with given spanwidth-position
 */
double Blade::get_Element_Coord(double sw, char dimension) { // 0= x, 1= y, 3=z
    switch (dimension) {
        case 0:
            return sw; // Das ist eine Näherung, die das Schwenken unterschlägt!
        case 1:
            return (double) gsl_spline_eval(this->blattlinie_xy_spline, sw, this->blattlinie_xy_accel);
        case 2:
            return (double) gsl_spline_eval(this->blattlinie_xz_spline, sw, this->blattlinie_xz_accel);
        default:
            return 0;
    }
};

