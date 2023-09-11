cmake_minimum_required(VERSION 3.5)

project(RadarMap LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/*.cpp"
                    "${PROJECT_SOURCE_DIR}/*.h"
                    "${PROJECT_SOURCE_DIR}/*.qrc"
                    "${PROJECT_SOURCE_DIR}/*.ui")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Widgets)
