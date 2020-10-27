//
// Created by tobias on 27.10.20.
//
#include "Blade.h"

#define AEQUIDISTANT_DISCRETIZATION 0

int Blade::initNet(int numberOfElements, int discretizationMethod) {
    switch (discretizationMethod) {
        case AEQUIDISTANT_DISCRETIZATION:
        default:
            printf("Aequidistant");
            break;

    }
    return 0;
}
