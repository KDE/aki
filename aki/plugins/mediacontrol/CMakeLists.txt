set(mediacontrolplugin_SRCS
    mediaconfig.cpp
    mediacontrol.cpp
    mediaremote.cpp
)

set(mediacontrolplugin_UIS
    mediacontrolpage.ui
)

kde4_add_ui_files(mediacontrolplugin_SRCS ${mediacontrolplugin_UIS})
kde4_add_kcfg_files(mediacontrolplugin_SRCS media.kcfgc)
kde4_add_plugin(akimediacontrolplugin ${mediacontrolplugin_SRCS})
target_link_libraries(
    akimediacontrolplugin
    akicore
    akiirc
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTDBUS_LIBRARY}
)
install(TARGETS akimediacontrolplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES akimediacontrolplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
