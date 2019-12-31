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

