add_definitions(-DMAKE_LIBAKI_EXPORT)

set(akicore_SRCS
    chatparser.cpp
    config/configfile.cpp
    config/identityconfig.cpp
    config/serverconfig.cpp
    dialogs/charselectordialog.cpp
    dialogs/colorpage.cpp
    dialogs/generalpage.cpp
    dialogs/identitydialog.cpp
    dialogs/identitywelcomepage.cpp
    dialogs/introwelcomepage.cpp
    dialogs/ircpage.cpp
    dialogs/logpage.cpp
    dialogs/messagelog.cpp
    dialogs/pluginpage.cpp
    dialogs/quickconnectiondialog.cpp
    dialogs/serverpage.cpp
    dialogs/serverwelcomepage.cpp
    dialogs/settingsdialog.cpp
    dialogs/welcomedialog.cpp
    identity.cpp
    interfaces/maininterface.cpp
    interfaces/settingspageinterface.cpp
    interfaces/windowinterface.cpp
    logfile.cpp
    networkexporter.cpp
    networkimporter.cpp
    nicklistmodel.cpp
    nicksort.cpp
    notifications.cpp
    plugin/plugin.cpp
    plugin/pluginmanager.cpp
    server.cpp
    ui/basewindow.cpp
    ui/channelmodebar.cpp
    ui/channelview.cpp
    ui/channelwindow.cpp
    ui/chatinput.cpp
    ui/chatview.cpp
    ui/mainwindow.cpp
    ui/nicklistdelegate.cpp
    ui/nickview.cpp
    ui/querywindow.cpp
    ui/serverview.cpp
    ui/serverwindow.cpp
    ui/statuswindow.cpp
    ui/systemtray.cpp
    ui/tabbar.cpp
)

set(akicore_UIS
    dialogs/colorpage.ui
    dialogs/generalpage.ui
    dialogs/identitydialog.ui
    dialogs/identitywelcomepage.ui
    dialogs/introwelcomepage.ui
    dialogs/ircpage.ui
    dialogs/logpage.ui
    dialogs/quickconnection.ui
    dialogs/serverpage.ui
    dialogs/serverwelcomepage.ui
    ui/channelmodebar.ui
    ui/channelwindow.ui
    ui/querywindow.ui
    ui/statuswindow.ui
)

set(akicore_LIBS
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${KDE4_KNOTIFYCONFIG_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTNETWORK_LIBRARY}
    akiirc
)

kde4_add_kcfg_files(akicore_SRCS settings.kcfgc)
kde4_add_ui_files(akicore_SRCS ${akicore_UIS})
kde4_add_library(akicore SHARED ${akicore_SRCS})
target_link_libraries(akicore ${akicore_LIBS})
set_target_properties(akicore PROPERTIES VERSION ${AKI_VERSION} SOVERSION ${AKI_SOVERSION})
install(TARGETS akicore ${INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES
        chatparser.h
        identity.h
        libaki_export.h
        logfile.h
        networkexporter.h
        networkimporter.h
        nicklistmodel.h
        nicksort.h
        notifications.h
        server.h
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki
    COMPONENT
        Devel
)

install(
    FILES
        Server
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/Aki
    COMPONENT
        Devel
)

install(
    FILES
        config/configfile.h
        config/identityconfig.h
        config/serverconfig.h
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/config
    COMPONENT
        Devel
)

install(
    FILES
        config/ConfigFile
        config/IdentityConfig
        config/ServerConfig
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/Aki/Config
    COMPONENT
        Devel
)

install(
    FILES
        interfaces/maininterface.h
        interfaces/settingspageinterface.h
        interfaces/windowinterface.h
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/interfaces/
    COMPONENT
        Devel
)

install(
    FILES
        plugin/plugin.h
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/plugin
    COMPONENT
        Devel
)

install(
    FILES
        ui/basewindow.h
        ui/serverview.h
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/ui
    COMPONENT
        Devel
)

install(
    FILES
        interfaces/MainInterface
        interfaces/SettingsPageInterface
        interfaces/WindowInterface
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/Aki/Interfaces/
    COMPONENT
        Devel
)

install(
    FILES
        plugin/Plugin
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/Aki/Plugin
    COMPONENT
        Devel
)

install(FILES akiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

add_subdirectory(config)
add_subdirectory(interfaces)
add_subdirectory(plugin)
add_subdirectory(ui)
add_subdirectory(dialogs)