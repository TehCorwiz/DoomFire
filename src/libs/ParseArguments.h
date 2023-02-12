//
// Created by corwin on 5/7/20.
//

#ifndef DOOMFIRE_PARSEARGUMENTS_H
#define DOOMFIRE_PARSEARGUMENTS_H

#include <args.hxx>
#include "DefaultValues.h"

struct parameters {
    unsigned int height = 0;
    unsigned int width = 0;

    unsigned int palette_size = 0;

    bool capped = false;
    unsigned int fps = 30;
    bool hsv = false;

    InterpolationFunction::InterpolationFunction interpolation_function = DEFAULT_INTERPOLATION_FUNCTION;

    args::Error *error = nullptr;
    std::string error_message;
};

static auto parseInterpolationFunction(const std::string &interpolation_function) {
    if (interpolation_function == "Cosine") return InterpolationFunction::Cosine;
    else if (interpolation_function == "Linear") return InterpolationFunction::Linear;

    return DEFAULT_INTERPOLATION_FUNCTION;
}

static parameters parseArguments(int argc, char **argv) {
    auto params = parameters();

    args::ArgumentParser parser(
            "This program is designed to simulate the fire effect from Doom that shipped on the PSX and N64.");
    args::HelpFlag help(parser,
                        "help",
                        "Display this help menu",
                        {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});

    // Rendering options
    args::ValueFlag<unsigned int> height(
            parser,
            "height",
            "height of fire simulation in pixels",
            {'g', "height"},
            DEFAULT_HEIGHT);
    args::ValueFlag<unsigned int> width(
            parser,
            "width",
            "Width of fire simulation in pixels",
            {'w', "width"},
            DEFAULT_WIDTH);
    args::ValueFlag<unsigned int> palette_size(
            parser,
            "palette_size",
            "Size of simulation's palette",
            {'p', "palette_size"}, 0);
    args::ValueFlag<std::string> interpolation_function(
            parser,
            "interpolation_function",
            "The interpolation function of palette scaling. Currently supports `Linear` or `Cosine`",
            {'i', "interpolation_function"}, "Linear");
    args::Flag uncapped(
            parser,
            "uncapped",
            "Toggles tick limiting. Takes no arguments.",
            {'u', "uncapped"}, false);
    args::ValueFlag<unsigned int> fps(
            parser,
            "fps",
            "Sets max FPS. Accepts an integer.",
            {'f', "fps"}, 30);
    args::Flag hsv(
            parser,
            "hsv",
            "Toggles interpolating in the HSV colorspace. Takes no arguments.",
            {"hsv"}, false);
    try {
        parser.ParseCLI(argc, argv);

        params.height = height.Get();
        params.width = width.Get();
        params.palette_size = palette_size.Get();
        params.capped = !uncapped.Get();
        params.fps = fps.Get();
        params.hsv = hsv.Get();

        params.interpolation_function = parseInterpolationFunction(interpolation_function.Get());

    }
    catch (args::Help &h) {
        params.error = &h;
        params.error_message = parser.Help();
    }
    catch (args::ParseError &e) {
        std::stringstream msg;
        msg << e.what() << std::endl;
        msg << parser;

        params.error = &e;
        params.error_message = msg.str();
    }
    catch (args::ValidationError &e) {
        std::stringstream msg;
        msg << e.what() << std::endl;
        msg << parser;

        params.error = &e;
        params.error_message = msg.str();
    }

    return params;
}

#endif //DOOMFIRE_PARSEARGUMENTS_H
