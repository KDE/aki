set (akiscriptingplugin_SRCS
    script.cxx
    scriptmodule.cxx
    scriptingplugin.cxx
)

set (akiscriptingplugin_UIS
)

#kde4_add_ui_files(akiscriptingplugin_SRCS ${akiscriptingplugin_UIS})
#kde4_add_kcfg_files(akiscriptingplugin_SRCS akiscripting.kcfgc)
kde4_add_plugin(akiscriptingplugin ${akiscriptingplugin_SRCS})
target_link_libraries(
    akiscriptingplugin
    akicore
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KNEWSTUFF3_LIBS}
    ${KDE4_KROSSCORE_LIBS}
    ${KDE4_KROSSUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
)

install(
    TARGETS
        akiscriptingplugin
    DESTINATION
        ${PLUGIN_INSTALL_DIR}
)

install(
    FILES
        akiscriptingplugin.desktop
    DESTINATION
        ${SERVICES_INSTALL_DIR}
)
