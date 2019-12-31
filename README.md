# PiHelper

PiHelper is designed to be a simple, lightweight, portable client library for the
[Pi-hole](https://pi-hole.net/). There is also a CLI tool available.

## CLI Usage

    -c, --configure              Configure Pi-Helper
    -d, --disable <duration>     Disable the Pi-hole for a given duration, or permanently if empty
    -e, --enable                 Enable the Pi-hole
    -f, --file <config-file>     Use the given config file instead of the default
    -h, --help                   Display this message
    -q, --quiet                  Don't print anything
    -v, --verbose                Print debug logs

## Library Usage

Include the `pihelper.h` file.

## Building

PiHelper depends on cmake, curl, openssl, and json-c. On Fedora you can install the dependencies with the
following command:

    sudo dnf install cmake \
        gcc \
        gcc-c++ \
        libcurl-devel \
        openssl-curl \
        json-c-devel

Once you have the dependencies installed, you can build the project with cmake:

    mkdir build
    cd build
    cmake ..
    make

If you'd like to install pihelper to your system, run the following after building:

    sudo make install # This is optional, and only needed if you want to install command line utility

## License

    Copyright © 2019, 2020 William Brawner.

    This file is part of PiHelper.

    PiHelper is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PiHelper is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PiHelper.  If not, see <https://www.gnu.org/licenses/>.

