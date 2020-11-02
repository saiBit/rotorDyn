cmake_minimum_required(VERSION 3.7)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(./src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

project(rotorDyn)

set(CMAKE_CXX_STANDARD_REQUIRED YES)

# ADD YOUR SOURCE FILES HERE
set(SOURCE_FILES src/main.cpp src/BladeElement.cpp src/Blade.cpp)

add_executable(rotorDyn ${SOURCE_FILES})

find_package(GSL REQUIRED)
target_link_libraries(rotorDyn GSL::gsl)

# PYTHON LIB
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(rotorDyn ${PYTHON_LIBRARIES})