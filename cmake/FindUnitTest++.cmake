find_path(UNITTESTPP_INCLUDE_DIR NAMES UnitTest++/UnitTest++.h)
find_library(UNITTESTPP_LIBRARY NAMES UnitTest++ libUnitTest++)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UnitTest++
  REQUIRED_VARS UNITTESTPP_LIBRARY UNITTESTPP_INCLUDE_DIR)

if(UnitTest++_FOUND AND NOT TARGET UnitTest++)
  add_library(UnitTest++ UNKNOWN IMPORTED)
  set_target_properties(UnitTest++ PROPERTIES
    IMPORTED_LOCATION "${UNITTESTPP_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${UNITTESTPP_INCLUDE_DIR}")
endif()
