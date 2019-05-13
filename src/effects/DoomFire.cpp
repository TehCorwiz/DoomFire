//
// Created by corwin on 5/1/19.
//

#include <stdio.h>

#include "../libs/ColorUtils.h"

#include "DoomFire.h"

// Initializes the random number generator.
void DoomFire::_initRng() {
    _rnd_gen = std::minstd_rand(std::random_device{}());
    _rnd_dist = std::uniform_real_distribution<double>(0.0, 1.0);
}

// Generates a random number.
double DoomFire::_rnd() {
    return _rnd_dist(_rnd_gen);
}

// Initializes our fire.
// At its core our fire generator is basically cellular automata. So we keep those cells in a vector of size
// DEFAULT_WIDTH * DEFAULT_HEIGHT. While we could use multidimensional arrays or a 2d vector the original algorithm uses some old-school
// pointer math to propagate the flames, so in this implementation we're using a 1d vector of length DEFAULT_WIDTH * DEFAULT_HEIGHT.
// This method accepts a boolean to determine if we should use a single white line of pixels as our source, or if we
// should use randomly colored pixels as our source. From there it iterates over the vector and pre-fills it with our
// starting colors.
void DoomFire::_initFire() {
    _fireCells = std::vector<size_t>(_fire_size);
    size_t r_p_idx;

    // Fill vector with defaults
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            if (y == _height - 1) // Bottom row is white (max palette index).
                // "Hottest" color, in our case white.
                _fireCells[y * _width + x] = _paletteSize - 1;

            else // Rest of cells are black (palette index 0).
                _fireCells[y * _width + x] = 0;
        }
    }
}

// Our constructor. Stores our parameters and bootstraps our rng and cells.
DoomFire::DoomFire(const size_t w, const size_t h, const size_t palette_size, const ColorSpace::ColorSpace colorspace,
                   const InterpolationFunction::InterpolationFunction interpolation_function) {
    _width = w;
    _height = h;
    _fire_size = w * h;
    _paletteSize = palette_size;
    _colorspace = colorspace;
    _interpolation_function = interpolation_function;

    _initRng();

    _initFire();
}

// Parses our vector of cells and produces an image. This variant returns a new image.
sf::Image DoomFire::getImage() {
    sf::Image img;
    img.create(_width, _height);

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            size_t palette_idx = _fireCells[y * _width + x];

            sf::Color pixel_color = _getDynamicColor(palette_idx);

            img.setPixel(x, y, pixel_color);
        }
    }

    return img;
}

// Parses our vector of cells and produces an image. This variant writes to an existing image by reference.
void DoomFire::getImage(sf::Image &img) {
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            const size_t palette_idx = _fireCells[y * _width + x];
            const sf::Color pixel_color = _getDynamicColor(palette_idx);

            img.setPixel(x, y, pixel_color);
        }
    }
}

// This simple iterates over our cells and calls our actual update function.
void DoomFire::doFire() {
    // Starting at horizontal line 1 prevents overwriting the bottom source line of pixels and prevents an integer
    // underflow from occuring later in spreadFire
    for (int y = 1; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            spreadFire(y * _width + x);
        }
    }
}

// This is where the flames happen! This logic is mostly cribbed directly from the source material.
void DoomFire::spreadFire(size_t src_idx) {
    // src_idx: this is the location of the current cell in _fireCells
    const size_t palette_idx = _fireCells[src_idx]; // palette_idx: the actual color value of the src palette_idx.

    if (palette_idx == 0) { // Black
        // If our palette_idx is already black then we propagate the value down one row.
        src_idx = src_idx - _width;
        _fireCells[src_idx] = 0;
    } else {
        // rnd_idx: this is a random index within 3 pixels.
        // The way this works is as follows:
        // 1) We get a random number between 0.0 and 1.0 inclusive.
        // 2) We multiply that by 3 to get a number between 0.0 and 3.0 inclusive.
        // 3) We floor that value to get an integer.
        // 4) We logically AND that value with 3 which clamps our value.
        const size_t rnd_idx = (size_t) floor(_rnd() * 3.0) & (uint) 3;
        // We then use this random index to offset our destination value. We add 1 here to avoid negative indices.
        const size_t dst = src_idx - rnd_idx + 1;
        // We move up one row
        const size_t dst_idx = dst - _width;
        // Finally we set the palette_idx value to either 1 less than the current color, or the current color.
        _fireCells[dst_idx] = palette_idx - (rnd_idx & (size_t) 1);
    }
}

// This draws a checkerboard in our color palette gradient.
// This was used earlier in development for testing various things.
void DoomFire::drawCheck() {
    bool is_color_pixel = false;
    int color_index = 0;

    for (int y = 0; y < (_height - 1); y++) {
        for (int x = 0; x < _width; x++) {
            if (is_color_pixel)
                _fireCells[y * _width + x] = (_paletteSize - 1);
            else
                _fireCells[y * _width + x] = color_index;

            // Every 8th flip colour
            if (!(x % 8)) {
                is_color_pixel = !is_color_pixel;
            }
        }

        // Every 8th flip colour
        if (!(y % 8)) {
            color_index = ++color_index % _paletteSize;
            is_color_pixel = !is_color_pixel;
        }
    }
}

// Resizes and re-initializes our simulation.
void DoomFire::resize(size_t w, size_t h) {
    _width = w;
    _height = h;
    _fire_size = w * h;

    _initFire();
}

// Performs a lerp on the CLASSIC_PALETTE to give us dynamic color values at a somewhat arbitrary resolution.
sf::Color DoomFire::_getDynamicColor(const size_t palette_idx) {
    // If we're using the default palette then we can just look the value.
    if (_paletteSize == CLASSIC_PALETTE_SIZE)
        return CLASSIC_PALETTE[palette_idx];

    // Abort if we're on the edge or outside the bounds of our palette
    if (palette_idx >= _paletteSize - 1)
        return CLASSIC_PALETTE[CLASSIC_PALETTE_SIZE - 1];
    else if (palette_idx < 0)
        return CLASSIC_PALETTE[0];

    // First convert actual index into an index relative to our classic palette
    double intermediate_scale = (((double) palette_idx / (double) _paletteSize) *
                                 ((double) CLASSIC_PALETTE_SIZE));
    // This is our actual size_t index.
    const size_t scaled_idx = floor(intermediate_scale);

    // We need to  extract this fractional component to feed into our lerp method.
    double scaled_fraction = intermediate_scale - scaled_idx;

    // We zero on small differences
    if (fabs(scaled_fraction) <= 0.00001)
        scaled_fraction = 0;

    switch (_colorspace) {
        case ColorSpace::HSV:
            return _hsv2color(ColorUtils::lerpColorHsv(_color2hsv(CLASSIC_PALETTE[scaled_idx]),
                                                       _color2hsv(CLASSIC_PALETTE[scaled_idx + 1]),
                                                       scaled_fraction,
                                                       _interpolation_function));
        default:
        case ColorSpace::RGB:
            return _rgb2color(ColorUtils::lerpColorRgb(_color2rgb(CLASSIC_PALETTE[scaled_idx]),
                                                       _color2rgb(CLASSIC_PALETTE[scaled_idx + 1]),
                                                       scaled_fraction,
                                                       _interpolation_function));
    }
}

RgbColor DoomFire::_color2rgb(const sf::Color &c) {
    return RgbColor{
            c.r,
            c.g,
            c.b
    };
}

sf::Color DoomFire::_rgb2color(const RgbColor &rgb) {
    return sf::Color{
            static_cast<sf::Uint8>(rgb.r),
            static_cast<sf::Uint8>(rgb.g),
            static_cast<sf::Uint8>(rgb.b),
    };
}

HsvColor DoomFire::_color2hsv(const sf::Color &c) {
    auto rgb = _color2rgb(c);

    return ColorUtils::rgb2hsv(rgb);
}

sf::Color DoomFire::_hsv2color(const HsvColor &hsv) {
    auto rgb = ColorUtils::hsv2rgb(hsv);

    return _rgb2color(rgb);
}
