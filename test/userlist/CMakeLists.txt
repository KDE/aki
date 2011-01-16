set(userlisttest_SRCS
    main.cxx
    userlist.cxx
    userlistdelegate.cxx
    userlistsort.cxx
    usermodel.cxx
)
kde4_add_unit_test(userlisttest ${userlisttest_SRCS})
target_link_libraries(userlisttest ${KDE4_KDECORE_LIBS} akicore  ${QT_QTGUI_LIBRARY} ${QT_QTTEST_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY})
