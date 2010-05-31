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
