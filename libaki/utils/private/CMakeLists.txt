set(akicore_utilsprivate_SRCS
    utils/private/indicationsystem_p.cxx
    utils/private/knotifications_p.cxx
)

if(INDICATEQT_FOUND)
    set(akicore_utilsprivate_SRCS
        ${akicore_utilsprivate_SRCS}
        utils/private/indicatenotification_p.cxx
    )
endif(INDICATEQT_FOUND)
