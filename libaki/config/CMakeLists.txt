include(config/private/CMakeLists.txt)
set(akicore_config_SRCS
    ${akicore_configprivate_SRCS}
    config/configfile.cxx
    config/identityconfig.cxx
    config/serverconfig.cxx
)