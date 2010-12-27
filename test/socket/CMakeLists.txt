set(sockettest_SRCS main.cxx sockettest.cxx)
kde4_add_unit_test(sockettest ${sockettest_SRCS})
target_link_libraries(sockettest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})
