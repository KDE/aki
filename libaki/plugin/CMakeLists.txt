set(akicore_plugin_SRCS
    plugin/plugin.cxx
)

install(
    FILES
        plugin/plugin.hpp
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/plugin
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
