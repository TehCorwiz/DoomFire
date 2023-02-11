include(cmake/CPM.cmake)
cmake_minimum_required(VERSION 3.14)
project(doomfire)

set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#set(CMAKE_CXX_FLAGS_RELEASE "-flto")

CPMAddPackage("gh:Taywee/args#6.4.6")
CPMAddPackage("gh:SFML/SFML#2.5.1")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS window graphics system REQUIRED)

add_executable(
        doomfire
        src/main.cpp
        src/main.h
        src/effects/DoomFire.cpp
        src/effects/DoomFire.h
        src/libs/ColorUtils.cpp
        src/libs/ColorUtils.h
        src/libs/DefaultValues.h
        src/libs/ParseArguments.h
        src/libs/InterpolationFunctions.cpp
        src/libs/InterpolationFunctions.h
)

target_link_libraries(
        doomfire
        args
        sfml-graphics
        sfml-window
        sfml-system
)
