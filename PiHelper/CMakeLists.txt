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
    JSONC
    NAMES json-c libjson-c
    HINTS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64
    )
find_library (
    CRYPTO
    NAMES crypto libcrypto
    HINTS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64
    )
find_library (
    OPENSSL
    NAMES ssl libssl
    HINTS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64
    )

if (NOT JSONC)
    message(SEND_ERROR "Did not find json-c")
endif()

if (NOT CRYPTO)
    message(SEND_ERROR "Did not find OpenSSL")
endif()

if (NOT OPENSSL)
    message(SEND_ERROR "Did not find OpenSSL")
endif()

target_link_libraries(libpihelper curl)
target_link_libraries(pihelper curl)

target_link_libraries(libpihelper ${JSONC})
target_link_libraries(pihelper ${JSONC})

target_link_libraries(libpihelper ${CRYPTO})
target_link_libraries(pihelper ${CRYPTO})

target_link_libraries(libpihelper ${OPENSSL})
target_link_libraries(pihelper ${OPENSSL})

