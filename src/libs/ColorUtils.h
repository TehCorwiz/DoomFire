//
// Created by corwin on 5/8/19.
//

#ifndef DOOMFIRE_COLORUTILS_H
#define DOOMFIRE_COLORUTILS_H

#include <memory>
#include <vector>

#include <SFML/Graphics/Color.hpp>

class ColorUtils {

public:
    static sf::Color lerpColor(
            sf::Color c0,
            sf::Color c1,
            double t,
            bool use_hsv,
            double (*f_pointer)(double, double, double)
    );

    static std::vector<sf::Color> expandPalette(
            const std::vector<sf::Color> &,
            size_t,
            bool,
            double (*)(double, double, double)
    );

private:
    struct Hsv {
        double h{};
        double s{};
        double v{};
    };

    static Hsv color2Hsv(sf::Color color);

    static sf::Color hsv2Color(Hsv hsv);
};


#endif //DOOMFIRE_COLORUTILS_H
