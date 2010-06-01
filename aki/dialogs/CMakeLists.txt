include(dialogs/configdialogpages/CMakeLists.txt)
include(dialogs/ui/CMakeLists.txt)

set (aki_dialogs_SRCS
    dialogs/addressdialog.cxx
    dialogs/channeldialog.cxx
    dialogs/configurationdialog.cxx
    dialogs/identitydialog.cxx
    dialogs/networkdialog.cxx
    ${aki_dialogs_configdialogpages_SRCS}
)