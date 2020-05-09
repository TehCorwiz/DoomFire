//
// Created by corwin on 5/7/20.
//

#ifndef DOOMFIRE_INTERPOLATIONFUNCTIONS_H
#define DOOMFIRE_INTERPOLATIONFUNCTIONS_H

#include <cmath>

namespace InterpolationFunction {
    enum InterpolationFunction {
        Linear,
        Cosine
    };
}

double interpolateLinear(double v1, double v2, double mu);

double interpolateCosine(double v1, double v2, double mu);

#endif //DOOMFIRE_INTERPOLATIONFUNCTIONS_H
