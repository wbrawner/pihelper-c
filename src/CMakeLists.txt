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
option(PIHELPER_SHARED "Build Pi-Helper as a shared library" OFF)
option(PIHELPER_EXECUTABLE "Build Pi-Helper as an executable" OFF)
option(PIHELPER_DEV "Install Pi-Helper header files for development" OFF)
option(PIHELPER_RPM "Apply custom config for building RPM packages" OFF)

if (PIHELPER_STATIC)
    add_library(libpihelperstatic STATIC
        ${PIHELPER_SOURCES}
        )

    set_target_properties(libpihelperstatic PROPERTIES OUTPUT_NAME "pihelper"
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER pihelper.h
        )

    target_link_libraries(libpihelperstatic ${CURL}
        ${JSONC}
        ${CRYPTO}
        ${OPENSSL}
        )

    install(TARGETS libpihelperstatic
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
endif()

if (PIHELPER_SHARED OR PIHELPER_EXECUTABLE)
    add_library(libpihelpershared SHARED
        ${PIHELPER_SOURCES}
        )

    set_target_properties(libpihelpershared PROPERTIES
        OUTPUT_NAME "pihelper"
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        )
    message("shared major version: ${PROJECT_VERSION_MAJOR}")
    if (PIHELPER_DEV)
        set_target_properties(libpihelpershared PROPERTIES PUBLIC_HEADER pihelper.h)
    endif()

    target_link_libraries(libpihelpershared ${CURL}
        ${JSONC}
        ${CRYPTO}
        ${OPENSSL}
        )

    install(TARGETS libpihelpershared)
endif()

if (PIHELPER_EXECUTABLE)
    if (NOT PIHELPER_RPM)
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
        set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
    endif()

    add_executable(pihelper
        cli.c
        )

    target_link_libraries(pihelper libpihelpershared
        ${CURL}
        ${JSONC}
        ${CRYPTO}
        ${OPENSSL}
        )

    install(TARGETS pihelper
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
endif()

