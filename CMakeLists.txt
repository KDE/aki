project(aki)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(CMAKE_MODULE_PATH
    ${aki_SOURCE_DIR}/cmake/modules
)

message(STATUS "${CMAKE_MODULE_PATH}")

set(AKI_VERSION_MAJOR 0)
set(AKI_VERSION_MINOR 0)
set(AKI_VERSION_PATCH 3)
set(AKI_VERSION "${AKI_VERSION_MAJOR}.${AKI_VERSION_MINOR}.${AKI_VERSION_PATCH}")
set(AKI_SOVERSION 0)

set(QT_MIN_VERSION "4.6.0")
set(KDE_MIN_VERSION "4.4.0")

find_package(KDE4 4.4.0 REQUIRED)
include(KDE4Defaults)
include(MacroLogFeature)
include(MacroWriteBasicCMakeVersionFile)

find_package(AkiIrc REQUIRED)
find_package(QJSON REQUIRED)

set(Boost_USE_STATIC OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.40.0 REQUIRED)

SET(QXT_USE_QXTCORE TRUE)
find_package(QCA2 REQUIRED)
find_package(Qxt REQUIRED)

macro_log_feature(KDE4_FOUND "kdelibs" "The toolkit Aki uses for gui" "http://www.kde.org" TRUE ${KDE_MIN_VERSION} "")
macro_log_feature(AKIIRC_FOUND "akiirc" "IRC Library for Aki" "http://www.akiirc.org" TRUE "0.0.3" "")
macro_log_feature(QT_QTWEBKIT_FOUND "qtwebkit" "Qt Webkit Library for Aki" "http://qt.nokia.org" TRUE ${QT_MIN_VERSION} "")
macro_log_feature(QT_QTSQL_FOUND "qtsql" "Qt Sql module for settings" "http://qt.nokia.com" ${QT_MIN_VERSION} "")
macro_log_feature(QJSON_FOUND "qjson" "Network list importer/exporter for Aki" "http://qjson.sourceforge.net/" TRUE "0.7.1" "")
macro_log_feature(QCA2_FOUND "qca2" "Encryption library" "http://delta.affinix.com/" TRUE "2.0" "")
macro_log_feature(Boost_FOUND "boost" "Boost library for compatibility" "http://www.boost.org" TRUE "1.40.0" "")
macro_log_feature(QXT_FOUND_QXTCORE "qxtcore" "QxtCore library for logging" "http://www.libqxt.org" TRUE "0.6.0" "")

add_definitions(
    ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS}
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -fno-strict-aliasing
)

include_directories(
    ${KDE4_INCLUDES}
    ${AKIIRC_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${QCA2_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
    ${QXT_INCLUDE_DIRS}
)

macro_write_basic_cmake_version_file(
    AkiConfigVersion.cmake
    ${AKI_VERSION_MAJOR} 
    ${AKI_VERSION_MINOR}
    ${AKI_VERSION_PATCH}
)

configure_file(
    "${aki_SOURCE_DIR}/AkiConfig.cmake.in"
    "${aki_BINARY_DIR}/AkiConfig.cmake"
    @ONLY
)
configure_file(
    "${aki_SOURCE_DIR}/akiversion.h.cmake"
    "${aki_BINARY_DIR}/akiversion.h"
    @ONLY
)

if(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)
    set(aki_CONFIG_DEST "${LIB_INSTALL_DIR}/cmake/aki")
else(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)
    set(aki_CONFIG_DEST "${LIB_INSTALL_DIR}/aki")
endif(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)

install(
    FILES
        "${aki_BINARY_DIR}/akiversion.h"
    DESTINATION
        "${INCLUDE_NSTALL_DIR}/aki"
)

install(
    FILES
        "${aki_BINARY_DIR}/AkiConfig.cmake"
        "${aki_BINARY_DIR}/AkiConfigVersion.cmake"
    DESTINATION
        ${aki_CONFIG_DEST}
)

install(
    FILES
        aki.notifyrc
    DESTINATION
        ${DATA_INSTALL_DIR}/aki
)

add_subdirectory(aki)
add_subdirectory(cmake)
add_subdirectory(libaki)
add_subdirectory(test)

macro_display_feature_log()