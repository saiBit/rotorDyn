//
// Created by tobias on 27.10.20.
//

#ifndef ROTORDYN_BLADEELEMENT_H
#define ROTORDYN_BLADEELEMENT_H

class BladeElement {
public:
    BladeElement(double radius_inner, double radius_center, double radius_outer);

private:
    double radius_center, radius_inner, radius_outer;
};

#endif //ROTORDYN_BLADEELEMENT_H
