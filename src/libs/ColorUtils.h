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

class ColorUtils {

public:
    static HsvColor rgb2hsv(sf::Color);

    static sf::Color hsv2rgb(HsvColor hsv);

    static sf::Color lerpColorHsv(sf::Color, sf::Color, double, InterpolationFunction::InterpolationFunction);

    static sf::Color lerpColorRgb(sf::Color, sf::Color, double, InterpolationFunction::InterpolationFunction);
};


#endif //DOOMFIRE_COLORUTILS_H
