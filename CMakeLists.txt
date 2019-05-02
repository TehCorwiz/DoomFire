cmake_minimum_required(VERSION 3.14)
project(demofire)

set(CMAKE_CXX_STANDARD 14)

add_executable(demofire src/main.cpp src/DemoFire.cpp src/DemoFire.h)

target_link_libraries(demofire sfml-graphics sfml-window sfml-system)
