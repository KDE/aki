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
    #dialogs/charselectordialog.cpp
    #dialogs/colorpage.cpp
    #dialogs/generalpage.cpp
    #dialogs/identitydialog.cpp
    #dialogs/identitywelcomepage.cpp
    #dialogs/introwelcomepage.cpp
    #dialogs/ircpage.cpp
    #dialogs/logpage.cpp
    #dialogs/pluginpage.cpp
    #dialogs/quickconnectiondialog.cpp
    #dialogs/serverpage.cpp
    #dialogs/serverwelcomepage.cpp
    #dialogs/settingsdialog.cpp
    #dialogs/welcomedialog.cpp
    main.cpp
    #mainwindow.cpp
)

set(aki_UIS
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

kde4_add_ui_files(aki_SRCS ${aki_UIS})
kde4_add_app_icon(aki_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hi*-apps-aki.png")
kde4_add_executable(aki ${aki_SRCS})
target_link_libraries(aki ${aki_LIBS})
install(TARGETS aki ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES akiui.rc DESTINATION ${DATA_INSTALL_DIR}/aki)
install(FILES aki.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

add_subdirectory(icons)
add_subdirectory(dialogs)
add_subdirectory(plugins)
