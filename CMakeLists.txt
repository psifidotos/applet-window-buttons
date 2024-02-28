cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(applet_windowbuttons)

set(CMAKE_CXX_STANDARD 14)
set(VERSION 0.11.1)
set(AUTHOR "Michail Vourlakos")
set(EMAIL "mvourlakos@gmail.com")

set(QT_MIN_VERSION "6.6.0")
set(KF6_MIN_VERSION "5.246.0")
set(KDECORATION2_MIN_VERSION "5.90")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED NO_MODULE COMPONENTS Gui Qml Quick)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS CoreAddons Declarative Package Svg)
find_package(Plasma "5.90" REQUIRED)
find_package(KDecoration2 ${KDECORATION2_MIN_VERSION} REQUIRED)

include(ECMFindQmlModule)
include(CheckIncludeFiles)
include(KDECMakeSettings)
include(KDEInstallDirs)

set(KDECORATION2_VERSION_MAJOR ${KDecoration2_VERSION_MAJOR})
set(KDECORATION2_VERSION_MINOR ${KDecoration2_VERSION_MINOR})

message(STATUS "KDECORATION2 VERSION MAJOR : ${KDECORATION2_VERSION_MAJOR}")
message(STATUS "KDECORATION2 VERSION MINOR : ${KDECORATION2_VERSION_MINOR}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libappletdecoration/config-decoration.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libappletdecoration/config-decoration.h)

add_subdirectory(libappletdecoration)
plasma_install_package(package org.kde.windowbuttons)
