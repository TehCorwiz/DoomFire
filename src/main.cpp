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


int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "DemoFire");

    DemoFire fire(WIDTH, HEIGHT, false);

    sf::Image fire_image;
    fire_image.create(WIDTH, HEIGHT);

    sf::Texture fire_texture;
    fire_texture.create(WIDTH, HEIGHT);

    sf::RectangleShape screen_rect;
    {
        screen_rect.setSize(sf::Vector2f(WIDTH * SCALE, HEIGHT * SCALE));
        screen_rect.setPosition(0, -2); // Why is there a border on top + left without this?
    }

//    fire.drawCheck();

    clock_t last_tick_at = clock(); // In nanoseconds

    while (window.isOpen()) {
        handle_window_events(window);

        const clock_t tick_time = clock() - last_tick_at; // In nanoseconds

        if (tick_time >= TICK_NS) {
            fire.doFire();
            drawFire(fire, fire_image, fire_texture, screen_rect);
            last_tick_at = clock();

            window.clear();
            window.draw(screen_rect);
            window.display();
        }
    }

    return 0;
}