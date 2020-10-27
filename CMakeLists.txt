cmake_minimum_required(VERSION 3.7)
set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

project(rotorDyn)

set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(rotorDyn main.cpp)

find_package(GSL REQUIRED)
target_link_libraries(rotorDyn GSL::gsl)