cmake_minimum_required(VERSION 3.27)
project(FireVFX)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5.1 COMPONENTS system window graphics audio CONFIG REQUIRED)

add_executable(FireVFX main.cpp
        Particle.cpp
        Particle.h
        util.h
        util.cpp
        Slider.cpp
        Slider.h)

target_link_libraries(FireVFX PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
