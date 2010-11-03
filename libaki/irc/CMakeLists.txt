include(irc/private/CMakeLists.txt)

set(akicore_irc_SRCS
    irc/message.cxx
    irc/nickinfo.cxx
    ${akicore_ircprivate_SRCS}
)
