cmake_minimum_required (VERSION 3.15.5)

include(GNUInstallDirs)

set(PIHELPER_VERSION 0.1.0)

project(
    pihelper
    VERSION ${PIHELPER_VERSION}
    )

add_subdirectory(PiHelper)

install(TARGETS pihelper libpihelper)

install(FILES PiHelper/pihelper.h DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/pihelper")

