//
// Created by corwin on 5/8/19.
//

#ifndef DOOMFIRE_COLORUTILS_H
#define DOOMFIRE_COLORUTILS_H

#include <memory>
#include <vector>

#include <SFML/Graphics/Color.hpp>

namespace InterpolationFunction {
    enum InterpolationFunction {
        Linear,
        Cosine
    };
}

class ColorUtils {

public:
    static sf::Color lerpColor(sf::Color c0, sf::Color c1, const double t, double (*f_pointer)(double, double, double));

    static std::vector<sf::Color> expandPalette(const std::vector<sf::Color> &, size_t new_length,
                                                double (*)(double, double, double));
};


#endif //DOOMFIRE_COLORUTILS_H
