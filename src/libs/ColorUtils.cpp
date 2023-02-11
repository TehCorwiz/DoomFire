//
// Created by corwin on 5/8/19.
//

#include <cmath>
#include "ColorUtils.h"
#include "DefaultValues.h"

sf::Color ColorUtils::lerpColor(sf::Color c0, sf::Color c1,
                                const double t,
                                const bool use_hsv,
                                double (*f_pointer)(double, double, double)) {
    if (t == 0) return c0;
    else if (t == 1) return c1;

    if (use_hsv) {
        auto hsv0 = color2Hsv(c0);
        auto hsv1 = color2Hsv(c1);
        auto h = f_pointer(hsv0.h, hsv1.h, t);
        auto s = f_pointer(hsv0.s, hsv1.s, t);
        auto v = f_pointer(hsv0.s, hsv1.v, t);

        auto int_hsv = Hsv{h, s, v,};

        return hsv2Color(int_hsv);
    } else {
        return sf::Color{
                static_cast<sf::Uint8>(f_pointer(c0.r, c1.r, t)),
                static_cast<sf::Uint8>(f_pointer(c0.g, c1.g, t)),
                static_cast<sf::Uint8>(f_pointer(c0.b, c1.b, t))
        };
    }
}

std::vector<sf::Color> ColorUtils::expandPalette(
        const std::vector<sf::Color> &old_palette,
        size_t new_length,
        bool use_hsv,
        double (*_interpolation_function)(double, double, double)
) {
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

        new_palette.push_back(lerpColor(c0, c1, scaled_fraction, use_hsv, _interpolation_function));

        step += step_size;
    }

    return new_palette;
}

ColorUtils::Hsv ColorUtils::color2Hsv(sf::Color rgb) {
    Hsv hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

sf::Color ColorUtils::hsv2Color(ColorUtils::Hsv hsv) {
    sf::Color rgb;

    uint64_t region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (unsigned int) (hsv.v * (255 - hsv.s)) >> (unsigned int) 8;
    q = (unsigned int) (hsv.v * (255 - ((unsigned int) (hsv.s * remainder) >> (unsigned int) 8))) >> (unsigned int) 8;
    t = (unsigned int) (hsv.v * (255 - (((unsigned int) hsv.s * (255 - remainder)) >> (unsigned int) 8))) >> (unsigned int) 8;

    switch (region) {
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
        default:
            rgb.r = hsv.v;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}

