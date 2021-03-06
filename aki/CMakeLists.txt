add_subdirectory(icons)
include(dialogs/CMakeLists.txt)
include(docks/CMakeLists.txt)
include(ui/CMakeLists.txt)
include(utils/CMakeLists.txt)

include(MacroOptionalAddSubdirectory)

include_directories (
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

link_directories (
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

set (aki_SRCS
    ${aki_dialogs_SRCS}
    ${aki_docks_SRCS}
    ${aki_ui_SRCS}
    ${aki_utils_SRCS}
    application.cxx
    dbus.cxx
    main.cxx
    mainwindow.cxx
   
)

set (aki_LIBS
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KDEWEBKIT_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${KDE4_KNOTIFYCONFIG_LIBS}
    ${QT_QTXML_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    akicore
)

set (aki_UIS_SRCS
    ${aki_dialogui_FILES}
)

kde4_add_ui_files(aki_SRCS ${aki_UIS_SRCS})
kde4_add_app_icon(aki_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hi*-apps-aki.png")
kde4_add_executable(aki ${aki_SRCS})
target_link_libraries(aki ${aki_LIBS})

install (
    TARGETS
        aki
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)

install(
    FILES
        akiplugin.desktop
    DESTINATION
        ${SERVICETYPES_INSTALL_DIR}
)

install (
    FILES
        akiui.rc
    DESTINATION
        ${DATA_INSTALL_DIR}/aki
)

install (
    FILES
        aki.desktop
    DESTINATION
        ${XDG_APPS_INSTALL_DIR}
)

add_subdirectory(plugins)
