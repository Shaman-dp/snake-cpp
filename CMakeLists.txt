cmake_minimum_required(VERSION 3.22)
project(snake_cpp)

set(CMAKE_CXX_STANDARD 20)

# Находим Qt5
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

add_executable(snake_cpp main.cpp
        src/snake.cpp
        src/snake.h
        src/food.cpp
        src/food.h)

# Линковка с Qt5
target_link_libraries(snake_cpp PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)
