include(dialogs/configdialogpages/CMakeLists.txt)
include(dialogs/ui/CMakeLists.txt)
include(dialogs/welcomedialogpages/CMakeLists.txt)

set (aki_dialogs_SRCS
    dialogs/addressdialog.cxx
    dialogs/channeldialog.cxx
    dialogs/configurationdialog.cxx
    dialogs/identitydialog.cxx
    #dialogs/networkdialog.cxx
    dialogs/quickconnectiondialog.cxx
    dialogs/serverlistdialog.cxx
    dialogs/welcomedialog.cxx
    ${aki_dialogs_configdialogpages_SRCS}
    ${aki_dialogs_welcomedialoggpages_SRCS}
)