include(interfaces/private/CMakeLists.txt)
set(akicore_interfaces_SRCS
    ${akicore_interfacesprivate_SRCS}
    interfaces/ibasewindow.cxx
)

install(
    FILES
        interfaces/ibasewindow.hpp
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/interfaces
    COMPONENT
        Devel
)
