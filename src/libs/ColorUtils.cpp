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

std::vector<sf::Color> ColorUtils::upsamplePalette(
        const std::vector<sf::Color> &old_palette,
        double new_palette_size,
        bool use_hsv,
        double (*_interpolation_function)(double, double, double)
) {
    std::vector<sf::Color> new_palette;

    auto step_count = ceil(new_palette_size / old_palette.size());

    for (u_long old_idx = 0; old_idx < old_palette.size() - 1; old_idx++) {
        auto c0 = old_palette[old_idx];
        auto c1 = old_palette[old_idx + 1];

        if (old_idx == 0) {
            new_palette.push_back(old_palette.front());
            continue;
        } else if (old_idx == old_palette.size() - 2) {
            new_palette.push_back(old_palette.back());
            continue;
        }

        for (auto c = 0; c <= step_count; c++) { // ;)
            auto fraction = c / step_count;

            auto new_color = lerpColor(c0, c1, fraction, use_hsv, _interpolation_function);

            new_palette.push_back(new_color);
        }
    }

    return new_palette;
}

std::vector<sf::Color>
ColorUtils::downsamplePalette(
        const std::vector<sf::Color> &old_palette,
        double step_count, bool use_hsv,
        double (*_interpolation_function)(double, double, double)) {
    std::vector<sf::Color> new_palette;


    for (u_long step = 0; step < step_count; step++) {
        double ratio = step_count / old_palette.size();
        double sample_point = ratio * step;

        if (sample_point == 0) {
            new_palette.push_back(old_palette.front());
            continue;
        } else if (step == step_count - 1) {
            new_palette.push_back(old_palette.back());
            continue;
        }

        size_t sample_idx = floor(sample_point);
        double fraction = sample_point - sample_idx;

        auto c0 = old_palette[sample_idx];
        auto c1 = old_palette[sample_idx + 1];

        auto new_color = lerpColor(c0, c1, fraction, use_hsv, _interpolation_function);

        new_palette.push_back(new_color);
    }

    return new_palette;
}

std::vector<sf::Color>
ColorUtils::resamplePalette(
        const std::vector<sf::Color> &old_palette,
        double new_palette_size,
        bool use_hsv,
        double (*_interpolation_function)(double, double, double)) {
    if (old_palette.size() > new_palette_size)
        return downsamplePalette(old_palette, new_palette_size, use_hsv, _interpolation_function);
    else
        return upsamplePalette(old_palette, new_palette_size, use_hsv, _interpolation_function);
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

    p = (uint) (hsv.v * (255 - hsv.s)) >> (uint) 8;
    q = (uint) (hsv.v * (255 - ((uint) (hsv.s * remainder) >> (uint) 8))) >> (uint) 8;
    t = (uint) (hsv.v * (255 - (((uint) hsv.s * (255 - remainder)) >> (uint) 8))) >> (uint) 8;

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
