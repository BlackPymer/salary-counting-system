find_path(UNITTEST++_INCLUDE_DIR UnitTest++/UnitTest++.h PATHS /usr/include)
find_library(UNITTEST++_LIBRARY NAMES UnitTest++ PATHS /usr/lib/x86_64-linux-gnu)

if(UNITTEST++_INCLUDE_DIR AND UNITTEST++_LIBRARY)
  if(NOT TARGET UnitTest++)
    add_library(UnitTest++ UNKNOWN IMPORTED)
    set_target_properties(UnitTest++ PROPERTIES
      IMPORTED_LOCATION "${UNITTEST++_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${UNITTEST++_INCLUDE_DIR}")
  endif()
  set(UnitTest++_FOUND TRUE)
else()
  set(UnitTest++_FOUND FALSE)
endif()
