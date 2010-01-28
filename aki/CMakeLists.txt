include(MacroOptionalAddSubdirectory)

include_directories(
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

link_directories(
    ${CMAKE_SOURCE_DIR}/lib/
    ${CMAKE_BINARY_DIR}/lib/
)

set(aki_SRCS
    akiapplication.cpp
    akistatusbar.cpp
    dbus.cpp
    main.cpp
)

set(aki_LIBS
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KNOTIFYCONFIG_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${QT_QTXML_LIBRARY}
    akicore
    akiirc
)

kde4_add_app_icon(aki_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hi*-apps-aki.png")
kde4_add_executable(aki ${aki_SRCS})
target_link_libraries(aki ${aki_LIBS})
install(TARGETS aki ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES akiui.rc DESTINATION ${DATA_INSTALL_DIR}/aki)
install(FILES aki.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES akiirc.protocol akiirc6.protocol DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
add_subdirectory(dialogs)
macro_optional_add_subdirectory(plugins)
