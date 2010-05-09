# Try to find Aki's library
#
# Example:
# find_package(Aki 0.0.3 REQUIRED) - Version and REQUIRED are optional
#
# AKI_FOUND       - System has Aki
# AKI_INCLUDE_DIR - Aki include directory
# AKI_LIBRARY     - Aki library
#

set (AKI_FIND_QUIETLY ${AKI_FIND_QUIETLY})
find_package(Aki ${AKI_FIND_VERSION} NO_MODULE)
set (AKI_FIND_QUIETLY ${AKI_FIND_QUIETLY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Aki DEFAULT_MSG AKI_INCLUDE_DIR AKI_LIBRARY)
