cmake_minimum_required(VERSION 3.14)
project(doomfire)

set(CMAKE_CXX_STANDARD 14)

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
)

target_link_libraries(
        doomfire
        sfml-graphics
        sfml-window
        sfml-system
)
