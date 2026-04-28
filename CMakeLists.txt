cmake_minimum_required(VERSION 3.10)
project(PPMFun)

# Add executable for ppmviewer
add_executable(ppmviewer WIN32 ppmviewer.cpp)

# Add executable for rand_ppm
add_executable(rand_ppm rand_ppm.cpp frame_utils.cpp)

add_executable(wave_ppm wave_ppm.cpp frame_utils.cpp)

add_executable(sphere sphere.cpp frame_utils.cpp)

add_executable(rand_animation rand_animation.cpp frame_utils.cpp)