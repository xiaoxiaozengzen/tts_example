project(ekho_example)

set(INSTALL_DIR /mnt/workspace/cgz_workspace/INSTALL)

add_executable(ekho_example ekho_example.cpp)
target_include_directories(ekho_example PUBLIC
  ${INSTALL_DIR}/include
)
target_link_directories(ekho_example PUBLIC
  ${INSTALL_DIR}/lib
)
target_link_libraries(ekho_example PUBLIC
  ekho
  espeak
  sndfile
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