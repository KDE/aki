set(amarokremotecontrolplugin_SRCS
    amarokconfig.cpp
    amarokremote.cpp
    amarokremotecontrolplugin.cpp
)

set(amarokremotecontrolplugin_UIS
    controlpage.ui
)

kde4_add_ui_files(amarokremotecontrolplugin_SRCS ${amarokremotecontrolplugin_UIS})
kde4_add_kcfg_files(amarokremotecontrolplugin_SRCS amarok.kcfgc)
kde4_add_plugin(aki_amarokremotecontrolplugin ${amarokremotecontrolplugin_SRCS})
target_link_libraries(
    aki_amarokremotecontrolplugin
    akicore
    akiirc
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTDBUS_LIBRARY}
)
install(TARGETS aki_amarokremotecontrolplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES aki_amarokremotecontrolplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
