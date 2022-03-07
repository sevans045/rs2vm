cmake_minimum_required(VERSION 3.15)

project(rs2vm)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
        Qt6
        COMPONENTS
        Core REQUIRED
        Network REQUIRED
)

add_library(
        rs2vm
        STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RCServer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RCIOTask.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RCPacket.cpp
)

target_include_directories(
        rs2vm
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
        rs2vm
        Qt6::Core
        Qt6::Network
)
