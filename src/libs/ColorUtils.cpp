//
// Created by corwin on 5/8/19.
//

#include <cmath>
#include "ColorUtils.h"
#include "DefaultValues.h"

sf::Color ColorUtils::lerpColorRgb(sf::Color c0, sf::Color c1,
                                   const double t,
                                   size_t (*f_pointer)(double, double, double)) {
    if (t == 0) return c0;
    else if (t == 1) return c1;

    auto out = sf::Color{
            static_cast<sf::Uint8>(f_pointer(c0.r, c1.r, t)),
            static_cast<sf::Uint8>(f_pointer(c0.g, c1.g, t)),
            static_cast<sf::Uint8>(f_pointer(c0.b, c1.b, t))
    };

    return out;
}

std::vector<sf::Color> ColorUtils::expandPalette(const std::vector<sf::Color> &old_palette, size_t new_length,
                                                 size_t (*_interpolation_function)(double, double, double)) {
    std::vector<sf::Color> new_palette;

    double step_size = (double) old_palette.size() / new_length;
    double step = 0;

    while (step < new_length - 1) {
        // First convert actual index into an index relative to our classic palette
        auto intermediate_scale = (step / new_length) * old_palette.size();

        // This is the current index of the old_palette.
        const size_t scaled_idx = floor(intermediate_scale);

        // This can be viewed as:
        //      How far away from the first value towards the second value we are expressed as a percentage.
        auto scaled_fraction = intermediate_scale - scaled_idx;

        new_palette.push_back(
                lerpColorRgb(
                        old_palette[scaled_idx],
                        old_palette[scaled_idx + 1],
                        scaled_fraction,
                        _interpolation_function
                )
        );
        step += step_size;
    }

    return new_palette;
}
