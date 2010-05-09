include_directories (
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

link_directories (
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

set(chatviewtest_SRCS chatviewtest.cxx)
kde4_add_unit_test(chatviewtest ${chatviewtest_SRCS})
target_link_libraries(chatviewtest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY} ${KDE4_KDEUI_LIBS} ${QT_QTGUI_LIBRARY})

set(jsontest_SRCS jsontest.cxx)
kde4_add_unit_test(jsontest ${jsontest_SRCS})
target_link_libraries(jsontest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})

set(databasetest_SRCS databasetest.cxx)
kde4_add_unit_test(databasetest ${databasetest_SRCS})
target_link_libraries(databasetest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})
