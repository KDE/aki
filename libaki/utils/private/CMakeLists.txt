set(akicore_utilsprivate_SRCS
    utils/private/database_p.cxx
    utils/private/indicationsystem_p.cxx
    utils/private/knotifications_p.cxx
    utils/private/sqladdress_p.cxx
    utils/private/sqlchannel_p.cxx
    utils/private/sqlidentity_p.cxx
    utils/private/sqlnetwork_p.cxx
    utils/private/sqlnickname_p.cxx
)

if(INDICATEQT_FOUND)
    set(akicore_utilsprivate_SRCS
        ${akicore_utilsprivate_SRCS}
        utils/private/indicatenotification_p.cxx
    )
endif(INDICATEQT_FOUND)
