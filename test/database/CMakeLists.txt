set(databasetest_SRCS
    author.cxx
    databasetest.cxx
)
kde4_add_unit_test(databasetest ${databasetest_SRCS})
target_link_libraries(databasetest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})

set(querytest_SRCS
    querytest.cxx
)
kde4_add_unit_test(querytest ${querytest_SRCS})
target_link_libraries(querytest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})
