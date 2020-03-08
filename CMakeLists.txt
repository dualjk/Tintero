cmake_minimum_required(VERSION 3.12)
project(Tintero)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5Sql REQUIRED)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts)
list(REMOVE_DUPLICATES LIBRARIES)

include_directories(.)
include_directories(client)
include_directories(client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug)
include_directories(client/Tintero)
include_directories(server)
include_directories(Tintero)

add_executable(Tintero
        client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug/moc_client.cpp
        client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug/moc_mainwindow.cpp
        client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug/moc_predefs.h
        client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug/qrc_resources.cpp
        client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug/ui_client.h
        client/build-Tintero-Desktop_Qt_5_14_1_clang_64bit-Debug/ui_mainwindow.h
        client/Tintero/client.cpp
        client/Tintero/client.h
        client/Tintero/main.cpp
        client/Tintero/mainwindow.cpp
        client/Tintero/mainwindow.h
        client/Tintero/mainwindow.ui
        client/Tintero/client.ui
        client/Tintero/resources.qrc
        "server/server folder"
        Tintero/Tintero.pro.user
        README.md)

target_link_libraries(Tintero Qt5::Widgets Qt5::Sql)