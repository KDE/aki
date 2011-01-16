set(upnptest_SRCS
    main.cxx
    upnptest.cxx
)
kde4_add_unit_test(upnptest ${upnptest_SRCS})
target_link_libraries(upnptest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})