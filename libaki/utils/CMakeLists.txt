include(utils/private/CMakeLists.txt)
set(akicore_utils_SRCS
    utils/indicationsystem.cxx
    utils/knotifications.cxx
    utils/messagehandle.cxx
    ${akicore_utilsprivate_SRCS}
)

if(INDICATEQT_FOUND)
    set(akicore_utils_SRCS
        ${akicore_utils_SRCS}
        utils/indicatenotification.cxx
    )
endif(INDICATEQT_FOUND)

install(
    FILES
        utils/indicatenotification.hpp
        utils/messagequeue.hpp
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/utils
    COMPONENT
        Devel
)

install(
    FILES
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/Aki/Utils
    COMPONENT
        Devel
)
