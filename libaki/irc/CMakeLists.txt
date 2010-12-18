include(irc/private/CMakeLists.txt)

set(akicore_irc_SRCS
    irc/adminreply.cxx
    irc/awayreply.cxx
    irc/basesocket.cxx
    irc/message.cxx
    irc/nickinfo.cxx
    irc/reply.cxx
    irc/replyinfo.cxx
    irc/rfc2812.cxx
    irc/socket.cxx
    ${akicore_ircprivate_SRCS}
)
