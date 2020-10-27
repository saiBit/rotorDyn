cmake_minimum_required(VERSION 3.7)

project(rotorDyn)

set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(rotorDyn main.cpp)

find_package(GSL REQUIRED)
target_link_libraries(rotorDyn GSL::gsl)