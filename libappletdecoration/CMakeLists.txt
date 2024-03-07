cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(QT_MIN_VERSION "6.6.0")
set(KF6_MIN_VERSION "5.26.0")

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

set(appletdecoration_SRCS
    appletdecorationplugin.cpp
    auroraetheme.cpp
    buttonsmodel.cpp
    commontools.cpp
    decorationsmodel.cpp
    decorationpalette.cpp
    environment.cpp
    padding.cpp
    previewbutton.cpp
    previewbridge.cpp
    previewclient.cpp
    previewsettings.cpp
    previewshareddecoration.cpp
    schemecolors.cpp
    schemesmodel.cpp
    themeextended.cpp
    types.cpp
    windowsystem.cpp
)

add_library(appletdecorationplugin SHARED ${appletdecoration_SRCS})

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED Quick DBus Widgets)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS CoreAddons WindowSystem I18n Service ConfigWidgets KCMUtils)
find_package(KDecoration2 REQUIRED)

target_link_libraries(appletdecorationplugin
                      Qt6::Core
                      Qt6::DBus
                      Qt6::Widgets
                      Qt6::Quick
                      KF6::CoreAddons
                      KF6::ConfigWidgets
                      KF6::I18n
                      Plasma::Plasma
                      KF6::WindowSystem
                      KF6::Service
                      KF6::Svg
                      KF6::KCMUtils
                      KDecoration2::KDecoration
                      KDecoration2::KDecoration2Private)

# install plugin
install(TARGETS appletdecorationplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/appletdecoration)

install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/appletdecoration)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/appletdecoration)
