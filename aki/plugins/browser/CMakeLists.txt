set(browserplugin_SRCS
    browser.cpp
    browserplugin.cpp
    browserpage.cpp
)

kde4_add_ui_files(browserplugin_SRCS browserpage.ui)
kde4_add_plugin(aki_browserplugin ${browserplugin_SRCS})
target_link_libraries(
    aki_browserplugin
    akicore
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTWEBKIT_LIBRARY}
)

install(TARGETS aki_browserplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aki_browserplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES aki_browserpluginui.rc DESTINATION ${DATA_INSTALL_DIR}/aki_browserplugin)
