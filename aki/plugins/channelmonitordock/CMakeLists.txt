set(channelmonitordockplugin_SRCS
    channelmonitor.cpp
    channelmonitordock.cpp
    channelmonitordockplugin.cpp
)

kde4_add_plugin(aki_channelmonitordockplugin ${channelmonitordockplugin_SRCS})
target_link_libraries(
    aki_channelmonitordockplugin
    akicore
    akiirc
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
)

install(TARGETS aki_channelmonitordockplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aki_channelmonitordockplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES aki_channelmonitordockpluginui.rc DESTINATION ${DATA_INSTALL_DIR}/aki_channelmonitordockplugin)
