//
// Created by corwin on 5/1/19.
//

#include "DemoFire.h"

void DemoFire::_initRng() {
    this->_rnd_gen = std::minstd_rand(std::random_device{}());
    this->_rnd_dist = std::uniform_real_distribution<double>(0.0, 1.0);
}


void DemoFire::_initFire(const bool random_seed) {
    this->_fireCells = std::vector<size_t>(this->_fireSize);

    // Fill vector with defaults
    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            if (y == this->_height - 1) // Bottom row is white (max palette index).
                if (random_seed)
                    this->_fireCells[y * this->_width + x] = this->_rndColor();
                else
                    this->_fireCells[y * this->_width + x] = PALETTE_SIZE - 1;

            else // Rest of cells are black (palette index 0).
                this->_fireCells[y * this->_width + x] = 0;
        }
    }
}

DemoFire::DemoFire(const size_t w, const size_t h, const bool random_seed = false) {
    this->_width = w;
    this->_height = h;
    this->_fireSize = w * h;

    this->_initRng();

    this->_initFire(random_seed);
}

// Parses our vector of cells and produces an image.
sf::Image DemoFire::getImage() {
    sf::Image img;
    img.create(this->_width, this->_height);

    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            size_t palette_idx = this->_fireCells[y * this->_width + x];
            sf::Color pixel_color = fire_palette[palette_idx];
            img.setPixel(x, y, pixel_color);
        }
    }

    return img;
}

void DemoFire::getImage(sf::Image &img) {
    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            size_t palette_idx = this->_fireCells[y * this->_width + x];
            sf::Color pixel_color = fire_palette[palette_idx];
            img.setPixel(x, y, pixel_color);
        }
    }
}


void DemoFire::doFire() {
    // Starting at horizontal line 1 prevents overwriting the bottom source line of pixels and prevents an integer
    // underflow from occuring later in spreadFire
    for (int y = 1; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            this->spreadFire(y * this->_width + x);
        }
    }
}

void DemoFire::spreadFire(size_t idx) {
    size_t src_idx = idx;
    size_t pixel = this->_fireCells[src_idx];

    if (pixel == 0) { // Black
        src_idx = src_idx - this->_width;
        this->_fireCells[src_idx] = 0;
    } else {
        size_t rnd_idx = (size_t) floor(this->_rnd() * 3.0) & (uint) 3;
        size_t dst = src_idx - rnd_idx + 1;
        size_t dst_idx = dst - this->_width;
        this->_fireCells[dst_idx] = pixel - (rnd_idx & (uint) 1);
    }
}

double DemoFire::_rnd() {
    return this->_rnd_dist(this->_rnd_gen);
}

size_t DemoFire::_rndColor() {
    size_t color_idx = floor((double) this->_rnd() * (double) PALETTE_SIZE);
    return color_idx;
}

void DemoFire::drawCheck() {
    bool is_color_pixel = false;
    int color_index = 0;

    for (int y = 0; y < (this->_height - 1); y++) {
        for (int x = 0; x < this->_width; x++) {
            if (is_color_pixel)
                this->_fireCells[y * this->_width + x] = (PALETTE_SIZE - 1);
            else
                this->_fireCells[y * this->_width + x] = color_index;

            // Every 8th flip colour
            if (!(x % 8)) {
                is_color_pixel = !is_color_pixel;
            }
        }

        // Every 8th flip colour
        if (!(y % 8)) {
            color_index = ++color_index % PALETTE_SIZE;
            is_color_pixel = !is_color_pixel;
        }
    }
}

void DemoFire::resize(size_t w, size_t h) {
    this->_width = w;
    this->_height = h;
    this->_fireSize = w * h;

    this->_initFire(false);
}

