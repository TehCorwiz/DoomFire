cmake_minimum_required(VERSION 3.14)
project(doomfire)

set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(
        doomfire
        src/main.cpp
        src/effects/DoomFire.cpp
        src/effects/DoomFire.h
        src/main.h
        src/libs/ColorUtils.cpp
        src/libs/ColorUtils.h
        src/libs/DefaultValues.h
        src/libs/ParseArguments.h
        src/libs/InterpolationFunctions.h
)

target_link_libraries(
        doomfire
        sfml-graphics
        sfml-window
        sfml-system
)
