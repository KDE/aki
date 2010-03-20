set(krossplugin_SRCS
    akimodule.cpp
    akiscript.cpp
    script.cpp
    scriptlistmodel.cpp
    scriptmanager.cpp
)

set(krossplugin_UIS
    scriptdialog.ui
)

kde4_add_ui_files(krossplugin_SRCS ${krossplugin_UIS})
#kde4_add_kcfg_files(krossplugin_SRCS kross.kcfgc)
kde4_add_plugin(akikrossplugin ${krossplugin_SRCS})
target_link_libraries(
    akikrossplugin
    akicore
    akiirc
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KROSSCORE_LIBS}
    ${KDE4_KROSSUI_LIBS}
    ${KDE4_KNEWSTUFF3_LIBS}
)

install(TARGETS akikrossplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES akikrossplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES akikrosspluginui.rc DESTINATION ${DATA_INSTALL_DIR}/akikrossplugin)
install(FILES aki.knsrc DESTINATION ${CONFIG_INSTALL_DIR})

add_subdirectory(scripts)
