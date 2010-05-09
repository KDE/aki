include(ui/private/CMakeLists.txt)

set(akicore_ui_SRCS
    ${akicore_uiprivate_SRCS}
    ui/chatview.cxx
    ui/dockbar.cxx
    ui/dockbutton.cxx
    ui/dockwidget.cxx
    ui/mainwindow.cxx
    ui/view.cxx
    ui/viewtabbar.cxx
)

install(
    FILES
        ui/chatview.hpp
        ui/dockbar.hpp
        ui/dockbutton.hpp
        ui/dockwidget.hpp
        ui/mainwindow.hpp
        ui/view.hpp
        ui/viewtabbar.hpp
    DESTINATION
        ${INCLUDE_INSTALL_DIR}/aki/ui
    COMPONENT
        Devel
)
