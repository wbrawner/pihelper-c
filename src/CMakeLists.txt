# Copyright © 2019, 2020 William Brawner.
#
# This file is part of PiHelper.
#
# PiHelper is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PiHelper is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PiHelper.  If not, see <https://www.gnu.org/licenses/>.

include(GNUInstallDirs)

set(PIHELPER_SOURCES
    pihelper.c
    log.c
    network.c
    config.c
    )

include_directories(/usr/local/include)
find_library (
    CURL
    NAMES curl libcurl
    HINTS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64
    )
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

if (NOT CURL)
    message(SEND_ERROR "Did not find curl")
endif()

if (NOT JSONC)
    message(SEND_ERROR "Did not find json-c")
endif()

if (NOT CRYPTO)
    message(SEND_ERROR "Did not find OpenSSL")
endif()

if (NOT OPENSSL)
    message(SEND_ERROR "Did not find OpenSSL")
endif()

option(PIHELPER_STATIC "Build Pi-Helper as a static library" ON)

if (PIHELPER_STATIC)
    add_library(libpihelperstatic STATIC
        ${PIHELPER_SOURCES}
        )

    set_target_properties(libpihelperstatic PROPERTIES OUTPUT_NAME "pihelper")

    target_link_libraries(libpihelperstatic ${CURL})
    target_link_libraries(libpihelperstatic ${JSONC})
    target_link_libraries(libpihelperstatic ${CRYPTO})
    target_link_libraries(libpihelperstatic ${OPENSSL})

    install(TARGETS libpihelperstatic )
endif()

option(PIHELPER_SHARED "Build Pi-Helper as a shared library" OFF)

if (PIHELPER_SHARED)
    add_library(libpihelpershared SHARED
        ${PIHELPER_SOURCES}
        )

    set_target_properties(libpihelpershared PROPERTIES OUTPUT_NAME "pihelper")

    target_link_libraries(libpihelpershared ${CURL})
    target_link_libraries(libpihelpershared ${JSONC})
    target_link_libraries(libpihelpershared ${CRYPTO})
    target_link_libraries(libpihelpershared ${OPENSSL})

    install(TARGETS libpihelpershared)
endif()

option(PIHELPER_DEV "Install Pi-Helper header files for development" OFF)
if (PIHELPER_DEV)
    install(FILES PiHelper/pihelper.h DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/pihelper")
endif()

option(PIHELPER_EXECUTABLE "Build Pi-Helper as an executable" OFF)

if (PIHELPER_EXECUTABLE)
    add_executable(pihelper
        ${PIHELPER_SOURCES}
        cli.c
        )

    target_link_libraries(pihelper ${CURL})
    target_link_libraries(pihelper ${JSONC})
    target_link_libraries(pihelper ${CRYPTO})
    target_link_libraries(pihelper ${OPENSSL})

    install(TARGETS pihelper)
endif()

