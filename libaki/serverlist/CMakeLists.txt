include(serverlist/private/CMakeLists.txt)
set(akicore_serverlist_SRCS
    ${akicore_serverlistprivate_SRCS}
    serverlist/jsonparser.cxx
    serverlist/serverlistserializer.cxx
    serverlist/xchatparser.cxx
)