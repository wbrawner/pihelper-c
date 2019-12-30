cmake_minimum_required (VERSION 3.15.5)

set(PIHELPER_VERSION 0.1.0)

project(
    pihelper
    VERSION ${PIHELPER_VERSION}
    )

add_subdirectory(PiHelper)

install(TARGETS pihelper libpihelper)

#target_link_libraries(PiHelper SHARED cJSON)
