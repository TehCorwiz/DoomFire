//
// Created by corwin on 5/7/20.
//

#ifndef DOOMFIRE_INTERPOLATIONFUNCTIONS_H
#define DOOMFIRE_INTERPOLATIONFUNCTIONS_H

#include <cmath>

size_t interpolateLinear(const double v1, const double v2, const double mu) {
    return ceil(v1 * (1 - mu) + v2 * mu);
}

size_t interpolateCosine(const double y1, const double y2, const double mu) {
    double mu2;

    mu2 = (1 - cos(mu * M_PI)) / 2;
    return ceil(y1 * (1 - mu2) + y2 * mu2);
}

#endif //DOOMFIRE_INTERPOLATIONFUNCTIONS_H
