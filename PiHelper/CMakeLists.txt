set(PIHELPER_SOURCES
    pihelper.c
    log.c
    network.c
    config.c
    )

add_library(libpihelper
    ${PIHELPER_SOURCES}
    )

set_target_properties(libpihelper PROPERTIES OUTPUT_NAME "libpihelper")

add_executable(pihelper
    ${PIHELPER_SOURCES}
    cli.c
    )

include_directories(/usr/local/include)
find_library (
    CJSON
    NAMES cjson libcjson
    HINTS /usr/local/lib /usr/lib
    )
find_library (
    CRYPTO
    NAMES crypto libcrypto
    HINTS /usr/local/lib /usr/lib
    )
find_library (
    OPENSSL
    NAMES ssl libssl
    HINTS /usr/local/lib /usr/lib
    )

if (NOT CJSON)
    message(SEND_ERROR "Did not find cJSON")
endif()

if (NOT CRYPTO)
    message(SEND_ERROR "Did not find OpenSSL")
endif()

if (NOT OPENSSL)
    message(SEND_ERROR "Did not find OpenSSL")
endif()

target_link_libraries(libpihelper curl)
target_link_libraries(pihelper curl)

target_link_libraries(libpihelper ${CJSON})
target_link_libraries(pihelper ${CJSON})

target_link_libraries(libpihelper ${CRYPTO})
target_link_libraries(pihelper ${CRYPTO})

target_link_libraries(libpihelper ${OPENSSL})
target_link_libraries(pihelper ${OPENSSL})

