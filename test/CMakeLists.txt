include_directories (
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

link_directories (
    ${CMAKE_SOURCE_DIR}/libaki/
    ${CMAKE_BINARY_DIR}/libaki/
)

set(jsontest_SRCS jsontest.cxx)
kde4_add_unit_test(jsontest ${jsontest_SRCS})
target_link_libraries(jsontest ${KDE4_KDECORE_LIBS} akicore ${QT_QTTEST_LIBRARY})

set(encrypttest_SRCS encrypttest.cxx)
kde4_add_unit_test(encrypttest ${encrypttest_SRCS})
target_link_libraries(encrypttest ${KDE4_KDECORE_LIBS} ${QCA2_LIBRARIES} ${QT_QTTEST_LIBRARY})

add_subdirectory(database)
add_subdirectory(serverlist)
add_subdirectory(socket)
add_subdirectory(upnp)
add_subdirectory(userlist)