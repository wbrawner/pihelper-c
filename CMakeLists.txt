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

