#include <ctime>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>

#include <args.hxx>
#include <SFML/Graphics.hpp>

#include "main.h"
#include "effects/DoomFire.h"
#include "libs/ColorUtils.h"


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
void init_drawing(size_t w, size_t h, DoomFire &df, sf::Image &img, sf::Texture &tex, sf::RectangleShape &rect) {
    //df.resize(w, h); // We resize our simulation. This resets our pixel data.

    // On first call fire_image and fire_texture are uninitialized objects and must be created.
    img.create(w, h);

    tex.create(w, h);

    // screen_rect exists and has been initialized but needs to have its attributes set.
    rect.setSize(sf::Vector2f(w, h));
    rect.setScale(1, 1);
    rect.setPosition(0, 0);
}

ColorSpace::ColorSpace parseColorSpace(const std::string &colorspace) {
    if (colorspace == "HSV") return ColorSpace::HSV;
    return ColorSpace::RGB;
}

InterpolationFunction::InterpolationFunction parseInterpolationFunction(const std::string &interpolation_function) {
    if (interpolation_function == "Cosine") return InterpolationFunction::Cosine;
    return InterpolationFunction::Linear;
}

// Our entry point
int main(int argc, char **argv) {
    // Parse cli arguments
    args::ArgumentParser parser(
            "This program is designed to simulate the fire effect from Doom that shipped on the PSX and N64.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});

    // Rendering options
    args::ValueFlag<size_t> height(parser, "height", "height of fire simulation in pixels", {'h', "height"},
                                   DEFAULT_HEIGHT);
    args::ValueFlag<size_t> width(parser, "width", "Width of fire simulation in pixels", {'w', "width"},
                                  DEFAULT_WIDTH);
    args::ValueFlag<size_t> palette_size(parser, "palette_size", "Size of simulation's palette",
                                         {'p', "palette_size"}, DEFAULT_PALETTE_SIZE);
    args::ValueFlag<std::string> colorspace(parser, "colorspace",
                                            "Color space of fire simulation. Currently supports RGB or HSV",
                                            {'c', "colorspace"}, "RGB");
    args::ValueFlag<std::string> interpolation_function(parser, "interpolation_function",
                                                        "The interpolation function of palette scaling. Currently supports `Linear` or `Cosine`",
                                                        {'i', "interpolation_function"}, "Linear");

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help &h) {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    const size_t sim_height = height.Get();
    const size_t sim_width = width.Get();
    const size_t sim_palette_size = palette_size.Get();

    ColorSpace::ColorSpace sim_colorspace = parseColorSpace(colorspace.Get());
    InterpolationFunction::InterpolationFunction sim_interpolation_function = parseInterpolationFunction(
            interpolation_function.Get());

    // Our actual code below
    auto *event = new sf::Event(); // used to hold data about triggered events. SFML example code had this as a global.

    sf::Image fire_image; // Construct Image to write pixels onto.
    sf::Texture fire_texture; // Constructs Texture onto which we can draw our Image.
    sf::RectangleShape screen_rect; // Constructs a rectangle which takes our texture and can be draw to our window.

    // Initialize the fire sim
    DoomFire doom_fire(sim_width, sim_height, sim_palette_size, sim_colorspace,
                       sim_interpolation_function); // Custom virtual palette size

    // Creates our actual window with our dimensions and a window title.
    sf::RenderWindow window(sf::VideoMode(sim_width, sim_height), "DoomFire");

    // Initializes our drawing surfaces and simulation
    init_drawing(sim_width, sim_height, doom_fire, fire_image, fire_texture, screen_rect);

    clock_t last_tick_at = clock(); // We store the last time a tick occurred so we can calculate the tick time.

    // Here's our main loop. It runs as long as the window is open.
    while (window.isOpen()) {
        // Calls our window handling code at the start in order.
        // Without handling events the OS will think the app has hung.
        handle_window_events(window, *event);

        const clock_t tick_time = clock() - last_tick_at; // Calculates the time between our last tick and now.

        // Compares our current tick_time to the time between frames and runs if it's time.
        if (tick_time >= TICK_NS) {
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
    }

    // Returns success if we closed the program and didn't crash.
    return EXIT_SUCCESS;
}
