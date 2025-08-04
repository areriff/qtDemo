cmake_minimum_required(VERSION 3.31)
project(qtDemo)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(qtDemo
        main.cpp
        mainwindow.h
        mainwindow.cpp)

target_link_libraries(qtDemo PRIVATE Qt6::Widgets)
