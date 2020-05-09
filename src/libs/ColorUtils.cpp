//
// Created by corwin on 5/8/19.
//

#include <cmath>
#include "ColorUtils.h"
#include "DefaultValues.h"

sf::Color ColorUtils::lerpColor(sf::Color c0, sf::Color c1,
                                const double t,
                                double (*f_pointer)(double, double, double)) {
    if (t == 0) return c0;
    else if (t == 1) return c1;

    auto hsv0 = color2Hsv(c0);
    auto hsv1 = color2Hsv(c1);
    auto h = f_pointer(hsv0.h, hsv1.h, t);
    auto s = f_pointer(hsv0.s, hsv1.s, t);
    auto v = f_pointer(hsv0.s, hsv1.v, t);

    auto int_hsv = Hsv{h, s, v,};

    return hsv2Color(int_hsv);
}

std::vector<sf::Color> ColorUtils::expandPalette(const std::vector<sf::Color> &old_palette, size_t new_length,
                                                 double (*_interpolation_function)(double, double, double)) {
    std::vector<sf::Color> new_palette;

    double step_size = (double) old_palette.size() / new_length;
    double step = 0;

    while (step < (double) new_length - 1) {
        // First convert actual index into an index relative to our classic palette
        auto intermediate_scale = ((double) step / new_length) * (double) old_palette.size();

        // This is the current index of the old_palette.
        const size_t scaled_idx = floor(intermediate_scale);

        // This can be viewed as:
        //      How far away from the first value towards the second value we are expressed as a percentage.
        auto scaled_fraction = intermediate_scale - scaled_idx;

        auto c0 = old_palette[scaled_idx];

        // This lookup seems to be causing a warning in valgrind:
        //      `Invalid reading of size 4`
        auto c1 = old_palette[scaled_idx + 1];

        new_palette.push_back(lerpColor(c0, c1, scaled_fraction, _interpolation_function));

        step += step_size;
    }

    return new_palette;
}

ColorUtils::Hsv ColorUtils::color2Hsv(sf::Color color) {
    Hsv hsv;

    // Convert color values to a ratio
    double r = color.r / 255.0;
    double g = color.g / 255.0;
    double b = color.b / 255.0;

    double cmax = std::max(r, std::max(g, b));
    double cmin = std::min(r, std::min(g, b));

    double delta = cmax - cmin;

    // V
    hsv.v = cmax;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    // S
    hsv.s = cmax == 0 ? 0 : delta / cmax;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }

    // H
    if (cmax == cmin) {
        hsv.h = 0;
    } else {
        if (cmax == r)
            hsv.h = (g - b) / delta + (g < b ? 6 : 0);
        else if (cmax == g)
            hsv.h = (b - r) / delta + 2;
        else if (cmax == b)
            hsv.h = (r - g) / delta + 4;

        hsv.h /= 6;
    }

    return hsv;
}

sf::Color ColorUtils::hsv2Color(ColorUtils::Hsv hsv) {
    sf::Color rgb;
    int i = int(hsv.h * 6);
    double f = hsv.h * 6 - i;
    double p = hsv.v * (1 - hsv.s);
    double q = hsv.v * (1 - f * hsv.s);
    double t = hsv.v * (1 - (1 - f) * hsv.s);

    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    switch (i % 6) {
        case 0:
            rgb.r = hsv.v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = hsv.v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = hsv.v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = hsv.v;
            break;
        case 5:
            rgb.r = hsv.v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}
