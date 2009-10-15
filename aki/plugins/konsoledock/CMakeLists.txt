set(konsoledockplugin_SRCS
    konsoledock.cpp
    konsoledockplugin.cpp
)

kde4_add_plugin(aki_konsoledockplugin ${konsoledockplugin_SRCS})
target_link_libraries(
    aki_konsoledockplugin
    akicore
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KPARTS_LIBS}
)

install(TARGETS aki_konsoledockplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aki_konsoledockplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES aki_konsoledockpluginui.rc DESTINATION ${DATA_INSTALL_DIR}/aki_konsoledockplugin)
