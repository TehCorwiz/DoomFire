//
// Created by corwin on 5/7/20.
//

#ifndef DOOMFIRE_DEFAULTVALUES_H
#define DOOMFIRE_DEFAULTVALUES_H

#include <cmath>
#include <cstdio>
#include <vector>
#include <chrono>
#include "InterpolationFunctions.h"

/* Simulation constants */


// PALETTE_SIZE: In order for the fire to fit on screen we need to constrain the palette size to 1/4 of the height.
// Otherwise, the fire will run off the top of the screen. In some situations that may be useful,
// but by default it's not.
const static unsigned int CLASSIC_PALETTE_SIZE = 38;
constexpr static unsigned int DEFAULT_PALETTE_SIZE = 60;

// DEFAULT_HEIGHT: The value was chosen due to how the original algo works. CLASSIC_PALETTE_SIZE (defined in DoomFire.h)
// affects how high the flames are drawn. Thus, we need a value that isn't too short or too tall. The original value,
// 168 looks perfect with some but not too much vertical space to allow for little `sparks`. However, since the
// resolution is dependent on the CLASSIC_PALETTE_SIZE in the first place, I'm adopting a calculated value in order to
// support additional resolutions down the road.
constexpr static unsigned int DEFAULT_HEIGHT = 240;

// DEFAULT_WIDTH: The value uses the above to choose a width that results in a 16:9 ratio which is the current most common
// screen size.
constexpr static unsigned int DEFAULT_WIDTH = (size_t) (DEFAULT_HEIGHT * (16.0 / 9.0));

// DEFAULT_INTERPOLATION_FUNCTION:
// TODO: describe
const static auto DEFAULT_INTERPOLATION_FUNCTION = InterpolationFunction::Cosine;

#endif //DOOMFIRE_DEFAULTVALUES_H
