include(config/CMakeLists.txt)
include(dialogs/CMakeLists.txt)
include(interfaces/CMakeLists.txt)
include(plugin/CMakeLists.txt)
include(private/CMakeLists.txt)
include(serverlist/CMakeLists.txt)
include(ui/CMakeLists.txt)
include(utils/CMakeLists.txt)

set(akicore_SRCS
    ${akicore_config_SRCS}
    ${akicore_dialogs_SRCS}
    ${akicore_interfaces_SRCS}
    ${akicore_private_SRCS}
    ${akicore_serverlist_SRCS}
    ${akicore_ui_SRCS}
    ${akicore_utils_SRCS}
    aki.cxx
    chatparser.cxx
    identity.cxx
    server.cxx
)

set(akicore_UIS
)

set(akicore_LIBS
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KDEWEBKIT_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KNOTIFYCONFIG_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${QJSON_LIBRARIES}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QXT_LIBRARIES}
    akiirc
)

#kde4_add_kcfg_files(akicore_SRCS settings.kcfgc)
#kde4_add_ui_files(akicore_SRCS ${akicore_UIS})
kde4_add_library(akicore SHARED ${akicore_SRCS})
target_link_libraries(akicore ${akicore_LIBS})
set_target_properties(akicore PROPERTIES VERSION ${AKI_VERSION} SOVERSION ${AKI_SOVERSION})
install(TARGETS akicore ${INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES
        aki.hpp
        chatparser.hpp
        identity.hpp
        server.hpp
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki
    COMPONENT
        Devel
)
