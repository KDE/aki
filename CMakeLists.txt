project(aki)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(AKI_VERSION_MAJOR 0)
set(AKI_VERSION_MINOR 0)
set(AKI_VERSION_PATCH 3)
set(AKI_VERSION "${AKI_VERSION_MAJOR}.${AKI_VERSION_MINOR}.${AKI_VERSION_PATCH}")
set(AKI_SOVERSION 0)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(AkiIrc 0.0.3 REQUIRED)

include(MacroWriteBasicCMakeVersionFile)

add_definitions(
    ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS}
)

include_directories(
    ${KDE4_INCLUDES}
    ${AKIIRC_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
)

macro_write_basic_cmake_version_file(AkiConfigVersion.cmake ${AKI_VERSION_MAJOR} ${AKI_VERSION_MINOR} ${AKI_VERSION_PATCH})
configure_file("${aki_SOURCE_DIR}/AkiConfig.cmake.in" "${aki_BINARY_DIR}/AkiConfig.cmake" @ONLY)
configure_file("${aki_SOURCE_DIR}/akiversion.h.cmake" "${aki_BINARY_DIR}/akiversion.h" @ONLY )

if(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)
    set(aki_CONFIG_DEST "${LIB_INSTALL_DIR}/cmake/aki")
else(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)
    set(aki_CONFIG_DEST "${LIB_INSTALL_DIR}/aki")
endif(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)

install(
    FILES
        "${aki_BINARY_DIR}/akiversion.h"
    DESTINATION
        "${INCLUDE_INSTALL_DIR}/aki"
)
install(
    FILES
        "${aki_BINARY_DIR}/AkiConfig.cmake"
        "${aki_BINARY_DIR}/AkiConfigVersion.cmake"
    DESTINATION
        "${aki_CONFIG_DEST}"
)
install(FILES aki.notifyrc DESTINATION ${DATA_INSTALL_DIR}/aki)

add_subdirectory(aki)
add_subdirectory(libaki)
