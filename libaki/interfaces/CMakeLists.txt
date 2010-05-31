include(interfaces/private/CMakeLists.txt)
set(akicore_interfaces_SRCS
    interfaces/ibasewindow.cxx
    interfaces/imaincontroller.cxx
    interfaces/isettingspage.cxx
    ${akicore_interfacesprivate_SRCS}
)

install(
    FILES
        interfaces/ibasewindow.hpp
        interfaces/imaincontroller.hpp
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/interfaces
    COMPONENT
        Devel
)
