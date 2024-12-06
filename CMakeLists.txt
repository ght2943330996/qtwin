cmake_minimum_required(VERSION 3.16)
project(FireDetectionSystem VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(FireDetectionSystem
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    # 添加其他源文件
)

target_link_libraries(FireDetectionSystem PRIVATE
    Qt6::Widgets
)