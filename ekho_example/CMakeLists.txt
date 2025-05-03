project(ekho_example)

set(INSTALL_DIR /mnt/workspace/cgz_workspace/INSTALL)

find_package(PkgConfig REQUIRED)
pkg_search_module(pulse-simple REQUIRED libpulse-simple)
pkg_search_module(espeak-ng REQUIRED espeak-ng)
pkg_search_module(sndfile REQUIRED sndfile)

find_package(ALSA REQUIRED)

find_library(MPG123_LIB mpg123)
if (NOT MPG123_LIB)
  message(FATAL_ERROR "mpg123 library not found")
endif()
message(STATUS "mpg123 library found at ${MPG123_LIB}")

add_executable(ekho_example ekho_example.cpp)
target_include_directories(ekho_example PUBLIC
  ${INSTALL_DIR}/include
  ${pulse-simple_INCLUDE_DIRS}
  ${espeak-ng_INCLUDE_DIRS}
  ${sndfile_INCLUDE_DIRS}
  ${ALSA_INCLUDE_DIRS}
)
target_link_directories(ekho_example PUBLIC
  ${INSTALL_DIR}/lib
)
target_link_libraries(ekho_example PUBLIC
  ekho
  ${pulse-simple_LIBRARIES}
  ${espeak-ng_LIBRARIES}
  ${sndfile_LIBRARIES}
  ${ALSA_LIBRARIES}
  pthread
  sonic
  mpg123
)

find_library(SNDFILE_LIB sndfile)
if (NOT SNDFILE_LIB)
  message(FATAL_ERROR "sndfile library not found")
endif()
message(STATUS "sndfile library found at ${SNDFILE_LIB}")

add_executable(sndfile_example sndfile_example.cpp)
target_link_libraries(sndfile_example PUBLIC
  sndfile
)