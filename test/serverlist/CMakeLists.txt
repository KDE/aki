include(aki/CMakeLists.txt)
include(konversation/CMakeLists.txt)
include(xchat/CMakeLists.txt)

kde4_add_unit_test(aki_serverlist ${serverlist_aki_srcs})
target_link_libraries(aki_serverlist ${serverlist_aki_libs})

kde4_add_unit_test(konversation_serverlist ${serverlist_konversation_srcs})
target_link_libraries(konversation_serverlist ${serverlist_konversation_libs})

kde4_add_unit_test(xchat_serverlist ${serverlist_xchat_srcs})
target_link_libraries(xchat_serverlist ${serverlist_xchat_libs})