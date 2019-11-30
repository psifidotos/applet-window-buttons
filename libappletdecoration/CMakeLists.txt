cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.6.0")
set(KF5_MIN_VERSION "5.26.0")

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

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED  Quick DBus Widgets)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma WindowSystem I18n Service ConfigWidgets)
find_package(KDecoration2 REQUIRED)

target_link_libraries(appletdecorationplugin
                      Qt5::Core
                      Qt5::DBus
                      Qt5::Widgets
                      Qt5::Quick
                      KF5::ConfigWidgets
                      KF5::I18n
                      KF5::Plasma
                      KF5::WindowSystem
                      KF5::I18n
                      KF5::Service
                      KDecoration2::KDecoration
                      KDecoration2::KDecoration2Private)

# install plugin
install(TARGETS appletdecorationplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/appletdecoration)

install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/appletdecoration)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/appletdecoration)
