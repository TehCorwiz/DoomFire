//
// Created by corwin on 5/1/19.
//

#ifndef DOOMFIRE_DOOMFIRE_H
#define DOOMFIRE_DOOMFIRE_H


#include <SFML/Graphics/Image.hpp>
#include <random>
#include <functional>

class DoomFire {
public:
    // Here we define our palette. This is cribbed from the source material listed in the readme.
    //
    // I have plans to replace with a multi-color gradient palette generator which can generate this or any other
    // color palette of an arbitrary length. This would allow using arbitrary resolutions down the road.
    const static size_t CLASSIC_PALETTE_SIZE = 38;
    sf::Color CLASSIC_PALETTE[CLASSIC_PALETTE_SIZE] = {
            sf::Color(0x07, 0x07, 0x07),
            sf::Color(0x1F, 0x07, 0x07),
            sf::Color(0x2F, 0x0F, 0x07),
            sf::Color(0x47, 0x0F, 0x07),
            sf::Color(0x57, 0x17, 0x07),
            sf::Color(0x67, 0x1F, 0x07),
            sf::Color(0x77, 0x1F, 0x07),
            sf::Color(0x8F, 0x27, 0x07),
            sf::Color(0x9F, 0x2F, 0x07),
            sf::Color(0xAF, 0x3F, 0x07),
            sf::Color(0xBF, 0x47, 0x07),
            sf::Color(0xC7, 0x47, 0x07),
            sf::Color(0xDF, 0x4F, 0x07),
            sf::Color(0xDF, 0x57, 0x07),
            sf::Color(0xDF, 0x57, 0x07),
            sf::Color(0xD7, 0x5F, 0x07),
            sf::Color(0xD7, 0x5F, 0x07),
            sf::Color(0xD7, 0x67, 0x0F),
            sf::Color(0xCF, 0x6F, 0x00),
            sf::Color(0xCF, 0x77, 0x0F),
            sf::Color(0xCF, 0x7F, 0x0F),
            sf::Color(0xCF, 0x87, 0x17),
            sf::Color(0xC7, 0x87, 0x17),
            sf::Color(0xC7, 0x8F, 0x17),
            sf::Color(0xC7, 0x8F, 0x17),
            sf::Color(0xC7, 0x97, 0x1F),
            sf::Color(0xBF, 0x9F, 0x1F),
            sf::Color(0xBF, 0x9F, 0x1F),
            sf::Color(0xBF, 0xA7, 0x27),
            sf::Color(0xBF, 0xA7, 0x27),
            sf::Color(0xBF, 0xAF, 0x2F),
            sf::Color(0xB7, 0xAF, 0x2F),
            sf::Color(0xB7, 0xB7, 0x2F),
            sf::Color(0xB7, 0xB7, 0x37),
            sf::Color(0xCF, 0xCF, 0x6F),
            sf::Color(0xDF, 0xDF, 0x9F),
            sf::Color(0xEF, 0xEF, 0xC7),
            sf::Color(0xFF, 0xFF, 0xFF),
    };

    DoomFire(size_t w, size_t h, size_t palette_size = DoomFire::CLASSIC_PALETTE_SIZE);

    sf::Image getImage();

    void getImage(sf::Image &);

    void doFire();

    void spreadFire(size_t);

    void drawCheck();

    void resize(size_t, size_t);

private:
    size_t _width;
    size_t _height;
    size_t _fireSize;
    size_t _paletteSize;

    std::vector<size_t> _fireCells;

    std::minstd_rand _rnd_gen;
    std::uniform_real_distribution<double> _rnd_dist;

    void _initRng();

    void _initFire();

    double _rnd();

    sf::Color _getDynamicColor(size_t);
};


#endif //DOOMFIRE_DOOMFIRE_H
