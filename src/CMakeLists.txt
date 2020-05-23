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

include_directories(/usr/local/include ${CMAKE_INCLUDE_PATH})
if (NOT TARGET CURL)
    find_library(
        CURL
        NAMES curl libcurl
        PATHS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64 /lib
        )
    if (NOT CURL)
        message(SEND_ERROR "Did not find curl")
    endif()
endif()
if (NOT TARGET JSONC)
    find_library(
        JSONC
        NAMES json-c libjson-c
        PATHS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64 /lib
        )
    if (NOT JSONC)
        message(SEND_ERROR "Did not find json-c")
    endif()
endif()
if (NOT TARGET CRYPTO)
    find_library(
        CRYPTO
        NAMES crypto libcrypto
        PATHS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64 /lib
        )
    if (NOT CRYPTO)
        message(SEND_ERROR "Did not find OpenSSL")
    endif()
endif()
if (NOT TARGET OPENSSL)
    find_library(
        OPENSSL
        NAMES ssl libssl
        PATHS /usr/local/lib /usr/local/lib64 /usr/lib /usr/lib64 /lib
        )
    if (NOT OPENSSL)
        message(SEND_ERROR "Did not find OpenSSL")
    endif()
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
        )
    if (PIHELPER_DEV)
        set_target_properties(libpihelperstatic PROPERTIES PUBLIC_HEADER pihelper.h)
    endif()

    target_link_libraries(libpihelperstatic ${CURL}
        ${JSONC}
        ${CRYPTO}
        ${OPENSSL}
        )

    install(TARGETS libpihelperstatic
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
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

    install(TARGETS libpihelpershared
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
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
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
endif()

