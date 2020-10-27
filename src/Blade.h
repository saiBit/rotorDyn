//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADE_H
#define ROTORDYN_BLADE_H

#include <list>
#include "BladeElement.h"

class Blade {
private:
    double radius;

    static int initNet(int a, int b);

    std::list<BladeElement> bladeElements;
};


#endif //ROTORDYN_BLADE_H
