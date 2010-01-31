set(eventmonitordockplugin_SRCS
    eventitem.cpp
    eventmonitordock.cpp
    eventmonitordockplugin.cpp
)

kde4_add_plugin(aki_eventmonitordockplugin ${eventmonitordockplugin_SRCS})
target_link_libraries(
    aki_eventmonitordockplugin
    akicore
    akiirc
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KPARTS_LIBS}
)

install(TARGETS aki_eventmonitordockplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aki_eventmonitordockplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES aki_eventmonitordockpluginui.rc DESTINATION ${DATA_INSTALL_DIR}/aki_eventmonitordockplugin)
