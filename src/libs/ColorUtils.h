//
// Created by corwin on 5/8/19.
//

#ifndef DOOMFIRE_COLORUTILS_H
#define DOOMFIRE_COLORUTILS_H

#include <memory>

namespace ColorSpace {
    enum ColorSpace {
        RGB,
        HSV
    };
}

namespace InterpolationFunction {
    enum InterpolationFunction {
        Linear,
        Cosine
    };
}


typedef struct {
    size_t h;       // angle in degrees
    size_t s;       // a fraction between 0 and 1
    size_t v;       // a fraction between 0 and 1
} HsvColor;

typedef struct {
    size_t r;
    size_t g;
    size_t b;
} RgbColor;

class ColorUtils {

public:
    static HsvColor rgb2hsv(RgbColor);

    static HsvColor lerpColorHsv(HsvColor c0, HsvColor c1, double t, size_t (*f_pointer)(double, double, double));

    static RgbColor hsv2rgb(HsvColor);

    static RgbColor lerpColorRgb(RgbColor c0, RgbColor c1, double t, size_t (*f_pointer)(double, double, double));
};


#endif //DOOMFIRE_COLORUTILS_H
