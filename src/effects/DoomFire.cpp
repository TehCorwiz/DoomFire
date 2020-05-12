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
    _fireCells = std::vector<std::vector<size_t>>(_width);

    // Fill vector with defaults
    for (auto &_fireCell : _fireCells) {
        auto new_col = std::vector<size_t>(_height);

        for (size_t row_idx = 0; row_idx < new_col.size(); row_idx++) {
            if (row_idx == new_col.size() - 1) {
                new_col[row_idx] = _palette.size() - 1;
            } else {
                new_col[row_idx] = 0;
            }

        }
        _fireCell = new_col;
    }
}

// Our constructor. Stores our parameters and bootstraps our rng and cells.
DoomFire::DoomFire(
        const size_t w,
        const size_t h,
        const size_t palette_size,
        const bool use_hsv,
        const InterpolationFunction::InterpolationFunction interpolation_function
) {
    _width = w;
    _height = h;
    _palette_size = palette_size;
    _use_hsv = use_hsv;
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

    for (size_t col_idx = 0; col_idx < _fireCells.size(); col_idx++) {
        for (size_t row_idx = 0; row_idx < _fireCells[col_idx].size(); row_idx++) {
            size_t palette_idx = _fireCells[col_idx][row_idx];

            sf::Color pixel_color = _palette[palette_idx];

            img.setPixel(col_idx, row_idx, pixel_color);
        }
    }

    return img;
}

// Parses our vector of cells and produces an image. This variant writes to an existing image by reference.
void DoomFire::getImage(sf::Image &img) {
    for (size_t col_idx = 0; col_idx < _width; col_idx++) {
        for (size_t row_idx = 0; row_idx < _height; row_idx++) {
            const size_t palette_idx = _fireCells[col_idx][row_idx];
            const sf::Color pixel_color = _palette[palette_idx];

            img.setPixel(col_idx, row_idx, pixel_color);
        }
    }
}

// This simple iterates over our cells and calls our actual update function.
void DoomFire::doFire() {
    // Starting at horizontal line 1 prevents overwriting the bottom source line of pixels.
    for (size_t col_idx = 0; col_idx < _width; col_idx++) {
        for (size_t row_idx = 1; row_idx < _height; row_idx++) {
            // src_idx: this is the location of the current cell in _fireCells
            const size_t palette_idx = _fireCells[col_idx][row_idx]; // palette_idx: the actual color value of the src palette_idx.

            if (palette_idx == 0) { // Black
                // If our palette_idx is already black then we propagate the value down one row.
                auto one_row_down = row_idx - 1;
                _fireCells[col_idx][one_row_down] = 0;
            } else {
                // distance: this is a random index within 3 pixels.
                // The way this works is as follows:
                // 1) We get a random number between 0.0 and 1.0 inclusive.
                // 2) We multiply that by 3 to get a number between 0.0 and 3.0 inclusive.
                // 3) We floor that value to get an integer.
                // 4) We logically AND that value with 3 which clamps our value.
                const size_t distance = (size_t) floor(_rnd() * 3.0) & (uint) 3;

                // We then use this random index to offset our destination value.
                // auto new_col_idx = col_idx; // Vertical
                // auto new_col_idx = (col_idx + 1) % _width; // Diagonal lines. Very cool.
                auto new_col_idx = (col_idx - distance + 1) % _width; // Classic-ish

                // We move up one row
                const size_t one_row_down = (row_idx - 1) % _height;

                size_t new_palette_idx = palette_idx;
                if ((distance & (size_t) 1) > 0) {
                    new_palette_idx = (palette_idx - 1) % _palette_size;
                }

                // Finally we set the palette_idx value to either 1 less than the current color, or the current color.
                _fireCells[new_col_idx][one_row_down] = new_palette_idx;
            }
        }
    }
}

// This draws a checkerboard in our color palette gradient.
// This was used earlier in development for testing various things.
void DoomFire::drawCheck() {
    bool is_color_pixel = false;
    auto color_index = 0;

    for (size_t col_idx = 0; col_idx < _width; col_idx++) {
        for (size_t row_idx = 0; row_idx < (_height - 1); row_idx++) {
            // Every 8th flip colour
            if (!(col_idx % 8)) {
                is_color_pixel = !is_color_pixel;
            }

            // Every 8th flip colour
            if (!(row_idx % 8)) {
                ++color_index;
                color_index = color_index % _palette_size;
                is_color_pixel = !is_color_pixel;
            }

            if (is_color_pixel)
                _fireCells[col_idx][row_idx] = (_palette_size - 1);
            else
                _fireCells[col_idx][row_idx] = color_index;
        }
    }
}

// Resizes and re-initializes our simulation.
void DoomFire::resize(size_t w, size_t h) {
    _width = w;
    _height = h;

    _initFire();
}

// I have plans to replace with a multi-color gradient palette generator which can generate this or any other
// color palette of an arbitrary length.
std::vector<sf::Color> DoomFire::_generatePalette() {
    if (_palette_size == CLASSIC_PALETTE_SIZE) return _classic_palette;

    return ColorUtils::resamplePalette(_classic_palette, _palette_size, _use_hsv, _interpolation_function);
}
