#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "main.h"

// Handles window events. SFML handles events internally, and asynchronously. Events will accumulate until pollEvent is
// called which will load the next event into our `event` object which we can use to handle events such as resizing the
// window, or even key-presses and mouse clicks. This also handles SIGNALS from the OS.
void handle_window_events(sf::RenderWindow &window) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

// Takes the simulation, retrieves the image data and feeds it through our Image->Texture->RectangleShape pipeline.
void drawFire(DoomFire &fire, sf::Image &img, sf::Texture &tex, sf::RectangleShape &rect) {
    fire.getImage(img); // Writes the pixel data directly to the img.
    tex.loadFromImage(img); // Loads image into tex.
    rect.setTexture(&tex); // Applies that texture to the rect.
}

// Initializes our size dependent objects.
void init_drawing(size_t w, size_t h) {
    demo_fire.resize(w, h); // We resize our simulation. This resets our pixel data.

    // On first call fire_image and fire_texture are uninitialized objects and must be created.
    fire_image.create(w, h);

    fire_texture.create(w, h);

    // screen_rect exists and has been initialized but needs to have its attributes set.
//    screen_rect.setSize(sf::Vector2f(w * SCALE, h * SCALE));
    screen_rect.setSize(sf::Vector2f(w, h));
    screen_rect.setScale(1, 1);
    screen_rect.setPosition(0, 0);
}

// Our entry point
int main() {
    // Creates our actual window with our scaled dimensions and a window title.
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "DemoEffects");

    init_drawing(WIDTH, HEIGHT); // Initializes our drawing surfances and simulation

    clock_t last_tick_at = clock(); // We store the last time a tick occurred so we can calculate the tick time.

    // Here's our main loop. It runs as long as the window is open.
    while (window.isOpen()) {
        // Calls our window handling code at the start in order.
        // Without handling events the OS will think the app has hung.
        handle_window_events(window);

        const clock_t tick_time = clock() - last_tick_at; // Calculates the time between our last tick and now.

        // Compares our current tick_time to the time between frames and runs if it's time.
        if (tick_time >= TICK_NS) {
            demo_fire.doFire(); // Runs one iteration of our fire simulation.
            // Calls our drawing code above to load the pixel data into the texture
            drawFire(demo_fire, fire_image, fire_texture, screen_rect);
            // Saves our current time to feed back into the above time tick time calculations.
            last_tick_at = clock();

            // These three lines:
            // 1) Clear the screen
            // 2) Draw our simulation
            // 3) Blit the window to the screen
            window.clear();
            window.draw(screen_rect);
            window.display();
        }
    }

    // Returns success if we closed the program and didn't crash.
    return EXIT_SUCCESS;
}
