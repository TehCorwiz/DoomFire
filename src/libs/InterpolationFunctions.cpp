//
// Created by corwin on 5/7/20.
//

#include "InterpolationFunctions.h"

double interpolateLinear(const double v1, const double v2, const double mu) {
    auto v1_portion = v1 * (1 - mu);
    auto v2_portion = v2 * mu;

    auto val = v1_portion + v2_portion;

    return val;
}

double interpolateCosine(const double v1, const double v2, const double mu) {
    double mu2;

    mu2 = (1 - cos(mu * M_PI)) / 2;

    auto val = v1 * (1 - mu2) + v2 * mu2;

    return val;
}
