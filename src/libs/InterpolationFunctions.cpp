//
// Created by corwin on 5/7/20.
//

#include "InterpolationFunctions.h"

double interpolateLinear(const double v1, const double v2, const double mu) {
    return ceil(v1 * (1 - mu) + v2 * mu);
}

double interpolateCosine(const double v1, const double v2, const double mu) {
    double mu2;

    mu2 = (1 - cos(mu * M_PI)) / 2;
    return ceil(v1 * (1 - mu2) + v2 * mu2);
}
