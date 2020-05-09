#include <iostream>

#include <SFML/Graphics.hpp>

#include "main.h"
#include "libs/ParseArguments.h"
#include "effects/DoomFire.h"

// Handles window events. SFML handles events internally, and asynchronously. Events will accumulate until pollEvent is
// called which will load the next event into our `event` object which we can use to handle events such as resizing the
// window, or even key-presses and mouse clicks. This also handles SIGNALS from the OS.
void handle_window_events(sf::RenderWindow &window, sf::Event &event) {
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
void
init_drawing(const size_t w, const size_t h, DoomFire &df, sf::Image &img, sf::Texture &tex, sf::RectangleShape &rect) {
    //df.resize(w, h); // We resize our simulation. This resets our pixel data.

    // On first call fire_image and fire_texture are uninitialized objects and must be created.
    img.create(w, h);

    tex.create(w, h);

    // screen_rect exists and has been initialized but needs to have its attributes set.
    rect.setSize(sf::Vector2f(w, h));
    rect.setScale(1, 1);
    rect.setPosition(0, 0);
}

// Our entry point
int main(int argc, char **argv) {
    // Parse cli arguments
    auto params = parseArguments(argc, argv);

    if (typeid(params.error) == typeid(args::Help)) {
        std::cout << params.error_message;
        return 0;
    } else if (params.error != nullptr) {
        std::cerr << params.error_message;
        return 1;
    }

    // Our actual code below
    sf::Image fire_image; // Construct Image to write pixels onto.
    sf::Texture fire_texture; // Constructs Texture onto which we can draw our Image.
    sf::RectangleShape screen_rect; // Constructs a rectangle which takes our texture and can be draw to our window.

    if (params.palette_size == 0) {
        const double palette_size_ratio = (double) DEFAULT_PALETTE_SIZE / DEFAULT_HEIGHT;
        params.palette_size = params.height * palette_size_ratio;
    }

    const double tick_ratio = (double) DEFAULT_TARGET_TICK_RATE / DEFAULT_HEIGHT;
    const double target_tick_rate = params.height * tick_ratio;

    const size_t target_tick_ns = SECOND_NS / target_tick_rate;

    // Initialize the fire sim
    DoomFire doom_fire(
            params.width,
            params.height,
            params.palette_size,
            params.hsv,
            params.interpolation_function
    ); // Custom virtual palette size

    // Creates our actual window with our dimensions and a window title.
    sf::RenderWindow window(sf::VideoMode(params.width, params.height), "DoomFire");

    // Initializes our drawing surfaces and simulation
    init_drawing(params.width, params.height, doom_fire, fire_image, fire_texture, screen_rect);

    clock_t last_event_tick_at = clock(); // We store the last time a tick occurred so we can calculate the tick time.
    clock_t last_tick_at = clock(); // We store the last time a tick occurred so we can calculate the tick time.

    sf::Event event{}; // used to hold data about triggered events. SFML example code had this as a global.

    // Here's our main loop. It runs as long as the window is open.
    while (window.isOpen()) {
        // Calculates the time between our last ticks and now.
        const clock_t event_tick_time = clock() - last_event_tick_at;
        const clock_t tick_time = clock() - last_tick_at;

        // Calls our window handling code at the start in order.
        // Without handling events the OS will think the app has hung.
        if ((event_tick_time >= (long) EVENT_TICK_NS)) handle_window_events(window, event);

        // Compares our current tick_time to the time between frames and runs if it's time.
        if (params.capped && (tick_time < (long) target_tick_ns)) continue;

        // Runs one iteration of our fire simulation.
        doom_fire.doFire();

        // Calls our drawing code above to load the pixel data into the texture
        drawFire(doom_fire, fire_image, fire_texture, screen_rect);

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

    // Returns success if we closed the program and didn't crash.
    return EXIT_SUCCESS;
}
