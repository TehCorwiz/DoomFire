//
// Created by corwin on 5/7/19.
// Cribbed shamelessly from:
// https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

#ifndef DEMOFIRE_HSVCONV_H
#define DEMOFIRE_HSVCONV_H

#include <SFML/Graphics/Color.hpp>
#include <cmath>

typedef struct {
    unsigned int h;       // angle in degrees
    unsigned int s;       // a fraction between 0 and 1
    unsigned int v;       // a fraction between 0 and 1
} HsvColor;

static HsvColor rgb2hsv(sf::Color rgb) {
    HsvColor hsv;
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

static sf::Color hsv2rgb(HsvColor hsv) {
    sf::Color rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> (uint) 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> (uint) 8))) >> (uint) 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> (uint) 8))) >> (uint) 8;

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

static sf::Color _lerpColorHsv(sf::Color c0, sf::Color c1, const double t) {
//    if (t == 0) return c0;
//    else if (t == 1) return c1;

    auto a = rgb2hsv(c0);
    auto b = rgb2hsv(c1);

    HsvColor out;

    out.h = a.h + (b.h - a.h) * t;
    out.s = a.s + (b.s - a.s) * t;
    out.v = a.v + (b.v - a.v) * t;

    return hsv2rgb(out);
}

static sf::Color _lerpColorRgb(sf::Color c0, sf::Color c1, const double t) {
    if (t == 0) return c0;
    else if (t == 1) return c1;

    return sf::Color(
            c0.r + (c1.r - c0.r) * t,
            c0.b + (c1.b - c0.b) * t,
            c0.g + (c1.g - c0.g) * t
    );

// Cool, but not correct
//    return sf::Color(
//            c0.r + (c1.r - c0.r) * t,
//            c0.b + (c1.b - c0.b) * t,
//            c0.g + (c1.g - c0.g) * t
//    );
}


#endif //DEMOFIRE_HSVCONV_H
