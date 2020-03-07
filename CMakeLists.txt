cmake_minimum_required(VERSION 3.12)
project(Tintero)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


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
        client/Tintero/client.ui
        client/Tintero/mainwindow.ui
        "server/server folder"
        Tintero/Tintero.pro.user
        README.md)
