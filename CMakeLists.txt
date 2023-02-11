cmake_minimum_required(VERSION 3.14)
project(doomfire)

set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Optimize it!
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#set(CMAKE_CXX_FLAGS_RELEASE "-flto")

# External Dependencies
include(cmake/CPM.cmake)
CPMAddPackage("gh:Taywee/args#6.4.6")
CPMAddPackage("gh:SFML/SFML#2.5.1")

# Build it!
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

# Link it!
target_link_libraries(
        doomfire
        args
        sfml-system
        sfml-window
        sfml-graphics
)

# Collect DLLs on Windows
if (WIN32)
    add_custom_command(
            TARGET doomfire POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:doomfire> $<TARGET_FILE_DIR:doomfire>
            COMMAND_EXPAND_LISTS
    )
endif ()