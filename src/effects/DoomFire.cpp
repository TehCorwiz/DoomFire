//
// Created by corwin on 5/1/19.
//

#include <cstdio>
#include <SFML/Graphics/Image.hpp>

#include "DoomFire.h"

// Generates a random number.
double DoomFire::_rnd() {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return (double) r;
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

    // Fill vector with defaults
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            if (y == _height - 1) // Bottom row is white (max palette index).
                // "Hottest" color, in our case white.
                _fireCells[y * _width + x] = _palette_size - 1;

            else // Rest of cells are black (palette index 0).
                _fireCells[y * _width + x] = 0;
        }
    }
}

// Our constructor. Stores our parameters and bootstraps our rng and cells.
DoomFire::DoomFire(const size_t w, const size_t h, const size_t palette_size,
                   const InterpolationFunction::InterpolationFunction interpolation_function) {
    _width = w;
    _height = h;
    _fire_size = w * h;
    _palette_size = palette_size;
    _classic_palette = _generateClassicPalette();

    switch (interpolation_function) {
        case InterpolationFunction::Linear:
            _interpolation_function = interpolateLinear;
            break;
        case InterpolationFunction::Cosine:
            _interpolation_function = interpolateCosine;
            break;
    }

    _palette = _generatePalette();

    _initFire();
}

// Parses our vector of cells and produces an image. This variant returns a new image.
sf::Image DoomFire::getImage() {
    sf::Image img;
    img.create(_width, _height);

    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            size_t palette_idx = _fireCells[y * _width + x];

            sf::Color pixel_color = _palette[palette_idx];

            img.setPixel(x, y, pixel_color);
        }
    }

    return img;
}

// Parses our vector of cells and produces an image. This variant writes to an existing image by reference.
void DoomFire::getImage(sf::Image &img) {
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            const size_t palette_idx = _fireCells[y * _width + x];
            const sf::Color pixel_color = _palette[palette_idx];

            img.setPixel(x, y, pixel_color);
        }
    }
}

// This simple iterates over our cells and calls our actual update function.
void DoomFire::doFire() {
    // Starting at horizontal line 1 prevents overwriting the bottom source line of pixels and prevents an integer
    // underflow from occuring later in spreadFire
    for (size_t y = 1; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
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
    size_t color_index = 0;

    for (size_t y = 0; y < (_height - 1); y++) {
        for (size_t x = 0; x < _width; x++) {
            if (is_color_pixel)
                _fireCells[y * _width + x] = (_palette_size - 1);
            else
                _fireCells[y * _width + x] = color_index;

            // Every 8th flip colour
            if (!(x % 8)) {
                is_color_pixel = !is_color_pixel;
            }
        }

        // Every 8th flip colour
        if (!(y % 8)) {
            color_index = ++color_index % _palette_size;
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

// I have plans to replace with a multi-color gradient palette generator which can generate this or any other
// color palette of an arbitrary length.
std::vector<sf::Color> DoomFire::_generatePalette() {
    if (_palette_size == CLASSIC_PALETTE_SIZE) return _classic_palette;

    return ColorUtils::expandPalette(_classic_palette, _palette_size, _interpolation_function);
}
