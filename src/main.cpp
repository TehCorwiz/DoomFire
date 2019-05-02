#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "DemoFire.h"

sf::Event event;

const static size_t HEIGHT = 168;
const static size_t WIDTH = (size_t) floor(HEIGHT * (16.0 / 9.0));
const static size_t SCALE = 4;
const static uint TARGET_TICK_RATE = 28;
const static uint SECOND_NS = 1000000;
const static uint TICK_NS = SECOND_NS / TARGET_TICK_RATE;

DemoFire demo_fire(WIDTH, HEIGHT, false);
sf::Image fire_image;
sf::Texture fire_texture;
sf::RectangleShape screen_rect;

void handle_window_events(sf::RenderWindow &window) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void drawFire(DemoFire &fire, sf::Image &img, sf::Texture &tex, sf::RectangleShape &rect) {
    fire.getImage(img);
    tex.loadFromImage(img);
    rect.setTexture(&tex);
}

void init_drawing(size_t w, size_t h) {
    demo_fire.resize(w, h);

    fire_image.create(w, h);

    fire_texture.create(w, h);

    screen_rect.setSize(sf::Vector2f(w * SCALE, h * SCALE));
    screen_rect.setScale(1, 1);
    screen_rect.setPosition(0, 0);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "DemoFire");

    init_drawing(WIDTH, HEIGHT);

    clock_t last_tick_at = clock(); // In nanoseconds

    while (window.isOpen()) {
        handle_window_events(window);

        const clock_t tick_time = clock() - last_tick_at; // In nanoseconds

        if (tick_time >= TICK_NS) {
            demo_fire.doFire();
            drawFire(demo_fire, fire_image, fire_texture, screen_rect);
            last_tick_at = clock();

            window.clear();
            window.draw(screen_rect);
            window.display();
        }
    }

    return 0;
}
