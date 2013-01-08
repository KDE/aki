set(serverlist_konversation_srcs
    konversation/main.cxx
    konversation/konversationconfiguration.cxx
    konversation/konversationconfigurationprivate.cpp
)

set(serverlist_konversation_libs
    ${KDE4_KDECORE_LIBS}
    akicore
    ${QT_QTSQL_LIBRARY}
)
